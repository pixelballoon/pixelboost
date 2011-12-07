#include <fstream>
#include <sstream>

#include "svgParser.h"

#include "pixelboost/parser/json/writer.h"
#include "pixelboost/parser/xml/xml.h"

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

SvgParser::SvgParser()
{
    _Xml = new pugi::xml_document();
}

SvgParser::~SvgParser()
{
    delete _Xml;    
}

bool SvgParser::Load(const std::string& filename)
{
    if (!_Xml->load_file(filename.c_str()))
        return false;
    
    _Width = atof(_Xml->root().attribute("width").value());
    _Json["width"] = json::Number(_Width);

    _Height = atof(_Xml->root().attribute("height").value());
    _Json["height"] = json::Number(_Height);
    
    return true;
}

bool SvgParser::ParseAll()
{
    pugi::xpath_node_set layers = _Xml->select_nodes("/svg/g");
    
    for (pugi::xpath_node_set::const_iterator layerIt = layers.begin(); layerIt != layers.end(); ++layerIt)
    {
        ParseLayer(layerIt->node().attribute("id").value(), layerIt->node().attribute("id").value());
    }
    
    return true;
}

bool SvgParser::ParseLayer(const std::string& src, const std::string& dst)
{
    json::Object& layers = _Json["layers"];
    
    json::Object& layer = layers[dst.c_str()];
    
    char query[256];
    
    json::Array& layerImages = layer["images"];
    
    sprintf(query, "/svg/g[@id='%s']/image", src.c_str());
    pugi::xpath_node_set images = _Xml->select_nodes(query);
    
    for (pugi::xpath_node_set::const_iterator imageIt = images.begin(); imageIt != images.end(); ++imageIt)
    {
        json::Object image;
        std::string href = imageIt->node().attribute("xlink:href").value();
        image["name"] = json::String(href.substr(href.find_last_of('/')+1, href.find_last_of(".")-href.find_last_of('/')-1));
        
        float width = atof(imageIt->node().attribute("width").value())/32.f;
        float height = atof(imageIt->node().attribute("height").value())/32.f;
        
        image["width"] = json::Number(width);
        image["height"] = json::Number(height);
        
        std::string matrix = imageIt->node().attribute("transform").value();
        
        if (matrix != "")
        {
            matrix = matrix.substr(matrix.find_first_of('(')+1, matrix.find_last_of(')')-matrix.find_first_of('(')-1);

            std::vector<std::string> items = split(matrix, ' ');
            image["sx"] = json::Number(atof(items[0].c_str()));
            image["sy"] = json::Number(atof(items[3].c_str()));
            
            float tx = atof(items[4].c_str()) / 32.f;
            image["tx"] = json::Number(tx);
            
            float ty = atof(items[5].c_str()) / 32.f;
            image["ty"] = json::Number(_Height/2.f - ty + height/2.f);
        }
        
        layerImages.Insert(image);
    }

    json::Array& layerSplines = layer["splines"];
    
    sprintf(query, "/svg/g[@id='%s']/path", src.c_str());
    pugi::xpath_node_set paths = _Xml->select_nodes(query);
    
    for (pugi::xpath_node_set::const_iterator pathIt = paths.begin(); pathIt != paths.end(); ++pathIt)
    {
        json::Object spline;
        
        json::Array& points = spline["points"];
        
        SvgPath path;
        
        PathParser parser(pathIt->node().attribute("d").value());
        parser.Parse(path);
        
        for (std::vector<SvgPath::Point>::iterator pointIt = path.points.begin(); pointIt != path.points.end(); ++pointIt)
        {
            json::Object point;
            
            point["x1"] = json::Number(pointIt->x1);
            point["y1"] = json::Number(pointIt->y1);
            point["cx1"] = json::Number(pointIt->cx1);
            point["cy1"] = json::Number(pointIt->cy1);
            point["cx2"] = json::Number(pointIt->cx2);
            point["cy2"] = json::Number(pointIt->cy2);
            point["x2"] = json::Number(pointIt->x2);
            point["y2"] = json::Number(pointIt->y2);
            
            points.Insert(point);
        }
        
        layerSplines.Insert(spline);
    }
    
    return true;
}

bool SvgParser::Export(const std::string& filename)
{
    std::fstream output(filename.c_str(), std::ios::trunc | std::ios::out);
    
    json::Writer::Write(_Json, output);
    
    return true;
}

SvgPath::Point::Point(float x1, float y1, float cx1, float cy1,  float cx2, float cy2, float x2, float y2)
    : x1(x1)
    , y1(y1)
    , cx1(cx1)
    , cy1(cy1)
    , cx2(cx2)
    , cy2(cy2)
    , x2(x2)
    , y2(y2)
{
    
}

PathTokenizer::Token::Token(TokenType type, const std::string& data)
    : data(data)
    , type(type)
{
    
}

PathTokenizer::PathTokenizer(const std::string& path)
{
    _Path = path;
}

PathTokenizer::~PathTokenizer()
{
    
}

const std::queue<PathTokenizer::Token>& PathTokenizer::GetTokens()
{
    return _Tokens;
}

bool PathTokenizer::Tokenize()
{
    std::string tokenData;
    bool finished = false;
    _Index = 0;
    _State = kStateMain;
    while (!finished)
    {
        char character = _Index >= _Path.length() ? 0 : _Path[_Index];
        
        switch (_State)
        {
            case kStateMain:
            {
                int absolute = false;
                if (character >= 'A' && character <= 'Z')
                {
                    absolute = true;
                    character -= 'A'-'a';
                }
                
                if (character == 'm')
                {
                    _Tokens.push(Token(absolute?kTokenTypeMoveAbsolute:kTokenTypeMoveRelative));
                } else if (character == 'c')
                {
                    _Tokens.push(Token(absolute?kTokenTypeCurveAbsolute:kTokenTypeCurveRelative));
                } else if (character == 's')
                {
                    _Tokens.push(Token(absolute?kTokenTypeSmoothAbsolute:kTokenTypeSmoothRelative));
                } else if ((character >= '0' && character <= '9') || character == '-') {
                    _State = kStateNumber;
                    continue;
                } else if (character == 0)
                {
                    finished = true;
                }
                
                _Index++;
                
                break;
            }
                
            case kStateNumber:
            {
                if ((character >= '0' && character <= '9') || character == '-' || character == '.')
                {
                    if (character == '-')
                    {
                        if (tokenData.length())
                        {
                            _Tokens.push(Token(kTokenTypeNumber, tokenData));
                            tokenData = "";
                        }
                    }
                } else
                {
                    if (tokenData.length())
                    {
                        _Tokens.push(Token(kTokenTypeNumber, tokenData));
                        tokenData = "";
                    }
                    
                    _State = kStateMain;
                    continue;
                }
                
                tokenData += character;
                _Index++;
                break;
            }
        }
    }
    
    return true;
}

PathParser::PathParser(const std::string& path)
    : _Tokenizer(path)
{
    
}

PathParser::~PathParser()
{
    
}

bool PathParser::Parse(SvgPath& path)
{
    _X = 0;
    _Y = 0;
    
    _Tokenizer.Tokenize();
    _Tokens = _Tokenizer.GetTokens();
    
    std::vector<float> numbers;
    PathTokenizer::Token stateToken;
    while (_Tokens.size())
    {
        PathTokenizer::Token token = _Tokens.front();
        _Tokens.pop();
        
        if (token.type != PathTokenizer::kTokenTypeNumber)
        {
            if (numbers.size() != 0)
                return false;
            
            stateToken = token;
        } else {
            numbers.push_back(atof(token.data.c_str()));
        }
        
        switch (stateToken.type)
        {
            case PathTokenizer::kTokenTypeMoveAbsolute:
            {
                if (numbers.size() == 2)
                {
                    _X = numbers[0];
                    _Y = numbers[1];
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeMoveRelative:
            {
                if (numbers.size() == 2)
                {
                    _X += numbers[0];
                    _Y += numbers[1];
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeCurveAbsolute:
            {
                if (numbers.size() == 6)
                {
                    path.points.push_back(SvgPath::Point(_X, _Y, numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5]));
                    
                    _X = numbers[4];
                    _Y = numbers[5];
                    
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeCurveRelative:
            {
                if (numbers.size() == 6)
                {
                    path.points.push_back(SvgPath::Point(_X, _Y, numbers[0], numbers[1], numbers[2], numbers[3], _X + numbers[4], _X + numbers[5]));
                    
                    _X += numbers[4];
                    _Y += numbers[5];
                    
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeSmoothAbsolute:
            {
                if (numbers.size() == 4)
                {
                    float cx;
                    float cy;
                    
                    if (path.points.size())
                    {
                        cx = 2*_X - path.points.back().cx2;
                        cy = 2*_Y - path.points.back().cy2;
                    } else {
                        cx = _X;
                        cy = _Y;
                    }
                    
                    path.points.push_back(SvgPath::Point(_X, _Y, cx, cy, numbers[0], numbers[1], numbers[2], numbers[3]));
                    
                    _X = numbers[2];
                    _Y = numbers[3];
                    
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeSmoothRelative:
            {
                if (numbers.size() == 4)
                {
                    float cx;
                    float cy;
                    
                    if (path.points.size())
                    {
                        cx = 2*_X - path.points.back().cx2;
                        cy = 2*_Y - path.points.back().cy2;
                    } else {
                        cx = _X;
                        cy = _Y;
                    }
                    
                    path.points.push_back(SvgPath::Point(_X, _Y, cx, cy, numbers[0], numbers[1], numbers[2], numbers[3]));
                    
                    _X += numbers[2];
                    _Y += numbers[3];
                    
                    numbers.clear();
                }
                break;
            }
            default:
                break;
        }
    }
    
    return true;
}
