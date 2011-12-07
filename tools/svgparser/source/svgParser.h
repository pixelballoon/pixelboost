#pragma once

#include <string>
#include <vector>

#include "pixelboost/parser/json/elements.h"

namespace pugi
{
    class xml_document;
}

class SvgParser
{
public:
    SvgParser();
    ~SvgParser();
    
    bool Load(const std::string& filename);
    
    bool ParseAll();
    bool ParseLayer(const std::string& src, const std::string& dst);
    bool Export(const std::string& filename);
    
private:
    float _Width;
    float _Height;
    
    json::Object _Json;
    pugi::xml_document* _Xml;
};

struct SvgImage
{
    std::string name;
    std::string image;
};

struct SvgPath
{
    struct Point
    {
        float cx1, cy1;
        float x, y;
        float cx2, cy2;
    };
    
    std::vector<Point> points;
};

class PathTokenizer
{
public:
    PathTokenizer(const std::string& path);
    ~PathTokenizer();
    
    enum TokenType
    {
        kTokenTypeMoveAbsolute,
        kTokenTypeMoveRelative,
        kTokenTypeCurveAbsolute,
        kTokenTypeCurveRelative,
        kTokenTypeSmoothAbsolute,
        kTokenTypeSmoothRelative,
        kTokenType
    };
    
    struct Token
    {
        TokenType type;
        std::string data;
    };
    
    const std::vector<Token>& GetTokens();
    
private:
    enum State
    {
        kStateMain,
        kStateNumber,
    };
    
    std::string _Path;
    State _State;
    int _Index;
    
    std::vector<Token> _Tokens;
};

class PathParser
{
public:
    PathParser(const std::string& path);
    ~PathParser();
    
    bool Parse(SvgPath& path);
    
private:
    PathTokenizer _Tokenizer;
};