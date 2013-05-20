#include <fstream>
#include <sstream>
#include <queue>

#include "pugixml/pugixml.hpp"

#include "pixelboost/data/resources/svgResource.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::SvgResource)

class PathTokenizer
{
public:
    PathTokenizer(const std::string& path);
    ~PathTokenizer();
    
    enum TokenType
    {
        kTokenTypeUnknown,
        kTokenTypeMoveAbsolute,
        kTokenTypeMoveRelative,
        kTokenTypeCurveAbsolute,
        kTokenTypeCurveRelative,
        kTokenTypeSmoothAbsolute,
        kTokenTypeSmoothRelative,
        kTokenTypeLineToAbsolute,
        kTokenTypeLineToRelative,
        kTokenTypeHorizontalToAbsolute,
        kTokenTypeHorizontalToRelative,
        kTokenTypeVerticalToAbsolute,
        kTokenTypeVerticalToRelative,
        kTokenTypeNumber,
    };
    
    struct Token
    {
        Token(TokenType type = kTokenTypeUnknown, const std::string& data = "");
        
        TokenType type;
        std::string data;
    };
    
    const std::queue<Token>& GetTokens();
    
    bool Tokenize();
    
private:
    enum State
    {
        kStateMain,
        kStateNumber,
    };
    
    std::string _Path;
    State _State;
    int _Index;
    
    std::queue<Token> _Tokens;
};

class PathParser
{
public:
    PathParser(const std::string& path, float scale, glm::vec2 size);
    ~PathParser();
    
    bool Parse(SvgPath& path);
    
    struct Point
    {
        Point(float x1, float y1, float cx1, float cy1,  float cx2, float cy2, float x2, float y2);
        
        float x1;
        float y1;
        float cx1;
        float cy1;
        
        float cx2;
        float cy2;
        float x2;
        float y2;
    };
    
    std::vector<Point> Points;
    
private:
    std::queue<PathTokenizer::Token> _Tokens;
    
    float _Scale;
    glm::vec2 _Size;
    glm::vec2 _Position;
    PathTokenizer _Tokenizer;
};

SvgResource::SvgResource(ResourcePool* pool, const std::string& filename)
    : Resource(pool, filename)
{
    
}

SvgResource::~SvgResource()
{
    
}

ResourceError SvgResource::ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails)
{
    switch (process)
    {
        case kResourceProcessLoad:
        {
            return Load(filename);
        }
        case kResourceProcessProcess:
        {
            pugi::xpath_node svg = _Xml.select_single_node("svg");
            
            _Size = glm::vec2(atof(svg.node().attribute("width").value()), atof(svg.node().attribute("height").value()))/32.f;
            
            if (!ParseAll())
            {
                _Xml.reset();
                errorDetails = "Failed to parse spline";
                return kResourceErrorSystemError;
            }
            
            _Xml.reset();
            
            for (auto& group : _Groups)
            {
                for (auto& path : group.second.Paths)
                {
                    path.Curve.Parameterize();
                }
            }
            return kResourceErrorNone;
        }
        case kResourceProcessUnload:
        {
            _Groups.clear();
            _Size = glm::vec2(0,0);
            
            return kResourceErrorNone;
        }
        case kResourceProcessPostProcess:
        {
            return kResourceErrorNone;
        }
    }
}

const std::map<std::string, SvgGroup>& SvgResource::GetGroups()
{
    return _Groups;
}

glm::vec2 SvgResource::GetSize()
{
    return _Size;
}

ResourceError SvgResource::Load(const std::string& filename)
{
    std::string data;
    
    auto file = pb::FileSystem::Instance()->OpenFile(filename + ".svg", pb::kFileModeRead);
    
    if (!file)
    {
        return kResourceErrorNoSuchResource;
    }

    file->ReadAll(data);
    
    if (!_Xml.load_buffer(data.c_str(), data.length()))
    {
        return kResourceErrorSystemError;
    }
    
    return kResourceErrorNone;
}

bool SvgResource::ParseAll()
{
    pugi::xpath_node_set groups = _Xml.select_nodes("/svg/g");
    
    for (const auto& group : groups)
    {
        if (!ParseGroup(group.node().attribute("id").value()))
            return false;
    }
    
    return true;
}

bool SvgResource::ParseGroup(const std::string& name)
{
    SvgGroup group;
    
    char query[256];
    
    snprintf(query, 256, "/svg/g[@id='%s']/path", name.c_str());
    pugi::xpath_node_set paths = _Xml.select_nodes(query);
    
    for (pugi::xpath_node_set::const_iterator pathIt = paths.begin(); pathIt != paths.end(); ++pathIt)
    {
        SvgPath path;
        path.Name = pathIt->node().attribute("id").value();
        
        PathParser parser(pathIt->node().attribute("d").value(), 32.f, _Size);
        if (!parser.Parse(path))
            return false;
        
        if (parser.Points.size() > 0)
        {
            auto point = parser.Points[0];
            glm::vec2 pos = glm::vec2(point.x1, point.y1);
            path.Curve.AddPoint(HermiteCurve2D::Point(pos, glm::vec2(0,0), (glm::vec2(point.cx1, point.cy1)-pos)*3.f));
        }

        for (int i=0; i<parser.Points.size()-1; i++)
        {
            auto pointA = parser.Points[i];
            auto pointB = parser.Points[i+1];
            glm::vec2 pos = glm::vec2(pointB.x1, pointB.y1);
            path.Curve.AddPoint(HermiteCurve2D::Point(pos, (glm::vec2(pointA.cx2, pointA.cy2)-pos)*3.f, (glm::vec2(pointB.cx1, pointB.cy1)-pos)*3.f));
        }
        
        if (parser.Points.size() > 1)
        {
            auto point = parser.Points[parser.Points.size()-1];
            glm::vec2 pos = glm::vec2(point.x2, point.y2);
            path.Curve.AddPoint(HermiteCurve2D::Point(pos, (glm::vec2(point.cx2, point.cy2)-pos)*3.f, glm::vec2(0,0)));
        }
        
        group.Paths.push_back(path);
    }
    
    _Groups[name] = group;
    
    return true;
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
                } else if (character == 'l')
                {
                    _Tokens.push(Token(absolute?kTokenTypeLineToAbsolute:kTokenTypeLineToRelative));
                } else if (character == 'h')
                {
                    _Tokens.push(Token(absolute?kTokenTypeHorizontalToAbsolute:kTokenTypeHorizontalToRelative));
                } else if (character == 'v')
                {
                    _Tokens.push(Token(absolute?kTokenTypeVerticalToAbsolute:kTokenTypeVerticalToRelative));
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

PathParser::Point::Point(float x1, float y1, float cx1, float cy1,  float cx2, float cy2, float x2, float y2)
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

PathParser::PathParser(const std::string& path, float scale, glm::vec2 size)
    : _Tokenizer(path)
    , _Scale(scale)
    , _Size(size)
{
    
}

PathParser::~PathParser()
{
    
}

bool PathParser::Parse(SvgPath& path)
{
    _Position.x = 0;
    _Position.y = 0;
    
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
            {
                return false;
            }
            
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
                    _Position.x = numbers[0];
                    _Position.y = numbers[1];
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeMoveRelative:
            {
                if (numbers.size() == 2)
                {
                    _Position.x += numbers[0];
                    _Position.y += numbers[1];
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeCurveAbsolute:
            {
                if (numbers.size() == 6)
                {
                    Points.push_back(Point(_Position.x, _Position.y, numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5]));
                    
                    _Position.x = numbers[4];
                    _Position.y = numbers[5];
                    
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeCurveRelative:
            {
                if (numbers.size() == 6)
                {
                    Points.push_back(Point(_Position.x, _Position.y, _Position.x + numbers[0], _Position.y + numbers[1], _Position.x + numbers[2], _Position.y + numbers[3], _Position.x + numbers[4], _Position.y + numbers[5]));
                    
                    _Position.x += numbers[4];
                    _Position.y += numbers[5];
                    
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
                    
                    if (Points.size())
                    {
                        cx = 2*_Position.x - Points.back().cx2;
                        cy = 2*_Position.y - Points.back().cy2;
                    } else {
                        cx = _Position.x;
                        cy = _Position.y;
                    }
                    
                    Points.push_back(Point(_Position.x, _Position.y, cx, cy, numbers[0], numbers[1], numbers[2], numbers[3]));
                    
                    _Position.x = numbers[2];
                    _Position.y = numbers[3];
                    
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
                    
                    if (Points.size())
                    {
                        cx = 2*_Position.x - Points.back().cx2;
                        cy = 2*_Position.y - Points.back().cy2;
                    } else {
                        cx = _Position.x;
                        cy = _Position.y;
                    }
                    
                    Points.push_back(Point(_Position.x, _Position.y, cx, cy, _Position.x + numbers[0], _Position.y + numbers[1], _Position.x + numbers[2], _Position.y + numbers[3]));
                    
                    _Position.x += numbers[2];
                    _Position.y += numbers[3];
                    
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeHorizontalToAbsolute:
            {
                if (numbers.size() == 1)
                {
                    Points.push_back(Point(_Position.x, _Position.y, _Position.x, _Position.y, numbers[0], _Position.y, numbers[0], _Position.y));
                    _Position.x = numbers[0];
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeVerticalToAbsolute:
            {
                if (numbers.size() == 1)
                {
                    Points.push_back(Point(_Position.x, _Position.y, _Position.x, _Position.y, _Position.x, numbers[0], _Position.x, numbers[0]));
                    _Position.y = numbers[0];
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeLineToAbsolute:
            {
                if (numbers.size() == 2)
                {
                    Points.push_back(Point(_Position.x, _Position.y, _Position.x, _Position.y, numbers[0], numbers[1], numbers[0], numbers[1]));
                    _Position.x = numbers[0];
                    _Position.y = numbers[1];
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeHorizontalToRelative:
            {
                if (numbers.size() == 1)
                {
                    Points.push_back(Point(_Position.x, _Position.y, _Position.x, _Position.y, _Position.x + numbers[0], _Position.y, _Position.x + numbers[0], _Position.y));
                    _Position.x = _Position.x + numbers[0];
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeVerticalToRelative:
            {
                if (numbers.size() == 1)
                {
                    Points.push_back(Point(_Position.x, _Position.y, _Position.x, _Position.y, _Position.x, _Position.y + numbers[0], _Position.x, _Position.y + numbers[0]));
                    _Position.y = _Position.y + numbers[0];
                    numbers.clear();
                }
                break;
            }
            case PathTokenizer::kTokenTypeLineToRelative:
            {
                if (numbers.size() == 2)
                {
                    Points.push_back(Point(_Position.x, _Position.y, _Position.x, _Position.y, _Position.x + numbers[0], _Position.y + numbers[1], _Position.x + numbers[0], _Position.y + numbers[1]));
                    _Position.x = _Position.x + numbers[0];
                    _Position.y = _Position.y + numbers[1];
                    numbers.clear();
                }
                break;
            }
            default:
                break;
        }
    }
    
    for (auto& point : Points)
    {
        point.x1 /= _Scale;
        point.cx1 /= _Scale;
        point.cx2 /= _Scale;
        point.x2 /= _Scale;
        point.y1 = -point.y1/_Scale;
        point.cy1 = -point.cy1/_Scale;
        point.cy2 = -point.cy2/_Scale;
        point.y2 = -point.y2/_Scale;
        
        point.x1 -= _Size.x/2.f;
        point.cx1 -= _Size.x/2.f;
        point.cx2 -= _Size.x/2.f;
        point.x2 -= _Size.x/2.f;
        
        point.y1 += _Size.y/2.f;
        point.cy1 += _Size.y/2.f;
        point.cy2 += _Size.y/2.f;
        point.y2 += _Size.y/2.f;
    }
    
    return true;
}
