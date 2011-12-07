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
        
        for (std::vector<SvgPath::Point>::iterator it = path.points.begin(); it != path.points.end(); ++it)
        {
            json::Object point;
            
            points.Insert(point);
        }
        
        layerSplines.Insert(spline);
    }
    
    return true;
}

bool SvgParser::Export(const std::string& filename)
{
    std::stringstream output;
    
    json::Writer::Write(_Json, output);
    
    printf("%s", output.str().c_str());
    
    return true;
}

PathTokenizer::PathTokenizer(const std::string& path)
{
    _Path = path;
}

PathTokenizer::~PathTokenizer()
{
    
}

const std::vector<PathTokenizer::Token>& PathTokenizer::GetTokens()
{
    return _Tokens;
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
    return true;
}
