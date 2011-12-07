#pragma once

#include <string>
#include <queue>
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
    
    std::vector<Point> points;
};

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
    PathParser(const std::string& path);
    ~PathParser();
    
    bool Parse(SvgPath& path);
    
private:
    std::queue<PathTokenizer::Token> _Tokens;
    
    float _X;
    float _Y;
    PathTokenizer _Tokenizer;
};
