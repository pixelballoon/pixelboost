#include <sstream>
#include <string>
#include <vector>

#include "project/schema.h"
#include "project/schemaParser.h"

class Token
{
public:
    enum TokenType
    {
        kTokenInvalid,
        kTokenEntity,
        kTokenRecord,
        kTokenStruct,
        kTokenNumber,
        kTokenString,
        kTokenVariable,
        kTokenLeftBrace,
        kTokenRightBrace,
        kTokenLeftBracket,
        kTokenRightBracket,
        kTokenLeftSquare,
        kTokenRightSquare,
        kTokenAmpersand,
        kTokenSemicolon,
        kTokenColon,
        kTokenForwardSlash,
        kTokenStar,
        kTokenHat,
        kTokenComma,
    };
    
    Token(TokenType _Type = kTokenInvalid, const std::string& _Data = "")
    {
        type = _Type;
        data = _Data;
    }
    
    TokenType type;
    std::string data;
    
    unsigned long line;
    unsigned long column;
};

std::vector<Token> Tokenise(const std::string& input)
{
    enum State
    {
        kStateNormal,
        kStateVariable,
        kStateNumber,
        kStateString,
        kStateCommentLine,
        kStateCommentBlock,
    };
    
    std::vector<Token> tokens;
    
    size_t counter = 0;
    
    State state = kStateNormal;
    
    std::string data = "";
    char character;
    
    while (counter < input.length())
    {
        bool skipCounterIncrease = false;
        
        character = input[counter];
        
        switch (state)
        {
            case kStateNormal:
                switch (character)
            {
                case '{':
                    tokens.push_back(Token(Token::kTokenLeftBrace));
                    break;
                    
                case '}':
                    tokens.push_back(Token(Token::kTokenRightBrace));
                    break;
                    
                case '(':
                    tokens.push_back(Token(Token::kTokenLeftBracket));
                    break;
                    
                case ')':
                    tokens.push_back(Token(Token::kTokenRightBracket));
                    break;
                    
                case '[':
                    tokens.push_back(Token(Token::kTokenLeftSquare));
                    break;
                    
                case ']':
                    tokens.push_back(Token(Token::kTokenRightSquare));
                    break;
                    
                case '&':
                    tokens.push_back(Token(Token::kTokenAmpersand));
                    break;
                    
                case ';':
                    tokens.push_back(Token(Token::kTokenSemicolon));
                    break;
                    
                case ':':
                    tokens.push_back(Token(Token::kTokenColon));
                    break;
                    
                case '*':
                    tokens.push_back(Token(Token::kTokenStar));
                    break;
                    
                case '^':
                    tokens.push_back(Token(Token::kTokenHat));
                    break;
                    
                case ',':
                    tokens.push_back(Token(Token::kTokenComma));
                    break;
                    
                case '"':
                    data = "";
                    state = kStateString;
                    break;
                    
                case '/':
                    if (input[counter+1] == '/')
                    {
                        counter++;
                        state = kStateCommentLine;
                    }
                    else if (input[counter+1] == '*')
                    {
                        counter++;
                        state = kStateCommentBlock;
                    }
                    else
                        tokens.push_back(Token(Token::kTokenForwardSlash));
                    break;
                    
                case 'a' ... 'z':
                case 'A' ... 'Z':
                    // Check for specific keywords
                    if (character == 'e' && input.substr(counter, 6) == "entity")
                    {
                        tokens.push_back(Token(Token::kTokenEntity));
                        counter += 6;
                    }
                    else if (character == 'r' && input.substr(counter, 6) == "record")
                    {
                        tokens.push_back(Token(Token::kTokenRecord));
                        counter += 6;
                    }
                    else if (character == 's' && input.substr(counter, 6) == "struct")
                    {
                        tokens.push_back(Token(Token::kTokenStruct));
                        counter += 6;
                    }
                    else
                    {
                        data = character;
                        state = kStateVariable;
                    }
                    break;
                    
                case '0' ... '9':
                    data = character;
                    state = kStateNumber;
                    break;
                    
                case ' ':
                case '\t':
                case '\r':
                case '\n':
                    // Whitespace, ignore
                    break;
                    
                default:
                    // TODO: Error! Unknown token!
                    break;
            }
                break;
                
            case kStateVariable:
                switch (character)
            {
                case 'a' ... 'z':
                case 'A' ... 'Z':
                case '0' ... '9':
                case '_':
                    data += character;
                    break;
                    
                default:
                    tokens.push_back(Token(Token::kTokenVariable, data));
                    state = kStateNormal;
                    skipCounterIncrease = true;
                    break;
            }
                break;
                
            case kStateNumber:
                switch (character)
            {
                case '0' ... '9':
                case '.':
                    data += character;
                    break;
                    
                default:
                    tokens.push_back(Token(Token::kTokenNumber, data));
                    state = kStateNormal;
                    skipCounterIncrease = true;
                    break;
            }
                
                break;
                
            case kStateString:
                switch (character)
            {
                case '\\':
                    counter++;
                    
                    if (input[counter] == '"')
                        data += '"';
                    else if (input[counter] == 'n')
                        data += '\n';
                    else if (input[counter] == '\\')
                        data += '\\';
                    
                    break;
                    
                case '"':
                    tokens.push_back(Token(Token::kTokenString, data));
                    state = kStateNormal;
                    break;
                    
                default:
                    data += character;
            }
                
                break;
                
            case kStateCommentLine:
                switch (character)
            {
                case '\n':
                case '\r':
                    state = kStateNormal;
            }
                break;
                
            case kStateCommentBlock:
                switch (character)
            {
                case '*':
                    if (input[counter+1] == '/')
                    {
                        counter++;
                        state = kStateNormal;
                    }
            }
                break;
        }
        
        if (!skipCounterIncrease)
            counter++;
    }
    
    return tokens;
}

SchemaAttribute* ParseAttribute(std::vector<Token>& tokens, size_t& counter)
{
    if (tokens[counter].type != Token::kTokenLeftSquare)
        return 0;
    
    counter++;
    
    Token nameToken = tokens[counter++];
    
    if (nameToken.type != Token::kTokenVariable)
        return 0; // TODO: Error (Missing name)
    
    if (tokens[counter].type == Token::kTokenRightSquare)
    {
        counter++;
        return new SchemaAttribute(nameToken.data);
    }
    
    if (tokens[counter++].type != Token::kTokenLeftBracket)
        return 0; // TODO: Error (Missing bracket)
    
    SchemaAttribute* attribute = new SchemaAttribute(nameToken.data);
    
    while (true)
    {
        Token paramToken = tokens[counter++];
        
        if (paramToken.type != Token::kTokenVariable)
            return attribute; // TODO: Error (Missing param name)
        
        if (tokens[counter++].type != Token::kTokenColon)
            return attribute; // TODO: Error (Missing colon)
        
        Token valueToken = tokens[counter++];
        
        if (valueToken.type != Token::kTokenString)
            return attribute; // TODO: Error (missing value)
        
        attribute->AddParam(paramToken.data, valueToken.data);
        
        if (tokens[counter].type == Token::kTokenRightBracket)
        {
            counter++;
            break;
        }
        
        if (tokens[counter++].type != Token::kTokenComma)
            return attribute;
    }
    
    if (tokens[counter++].type != Token::kTokenRightSquare)
        return attribute; // TODO: Error (missing right square)
    
    return attribute;
}

SchemaProperty* ParseProperty(SchemaItem* parent, std::vector<Token>& tokens, size_t& counter)
{
    std::vector<SchemaAttribute*> attributes;
    
    while (SchemaAttribute* attribute = ParseAttribute(tokens, counter))
        attributes.push_back(attribute);
    
    Token typeToken = tokens[counter];
    
    if (typeToken.type != Token::kTokenVariable)
        return 0; // Expected type
    
    counter++;
    
    bool isArray = false;
    SchemaProperty::SchemaPropertyType propertyType = SchemaProperty::kSchemaPropertyAtom;
    
    if (tokens[counter].type != Token::kTokenVariable)
    {
        if (tokens[counter].type == Token::kTokenStar)
        {
            propertyType = SchemaProperty::kSchemaPropertyPointer;
            counter++;
        }
        
        if (tokens[counter].type == Token::kTokenAmpersand)
        {
            propertyType = SchemaProperty::kSchemaPropertyReference;
            counter++;
        }
        
        if (tokens[counter].type == Token::kTokenLeftSquare && tokens[counter+1].type == Token::kTokenRightSquare)
        {
            isArray = true;
            counter += 2;
        }
    }
    
    Token nameToken = tokens[counter];
    
    if (nameToken.type != Token::kTokenVariable)
        return 0; // Expected name
    
    counter++;
    
    if (propertyType == SchemaProperty::kSchemaPropertyAtom && !(typeToken.data == "bool" || typeToken.data == "float" || typeToken.data == "int" || typeToken.data == "string"))
    {
        propertyType = SchemaProperty::kSchemaPropertyStruct;
    }
    
    if (tokens[counter].type != Token::kTokenSemicolon)
        return 0; // Error (missing semicolon
    
    counter++;
    
    SchemaProperty* schemaProperty = 0;
    
    switch (propertyType)
    {
        case SchemaProperty::kSchemaPropertyAtom:
            schemaProperty = new SchemaPropertyAtom(parent, typeToken.data, nameToken.data);
            break;
            
        case SchemaProperty::kSchemaPropertyStruct:
            schemaProperty = new SchemaPropertyStruct(parent, typeToken.data, nameToken.data);
            break;
            
        case SchemaProperty::kSchemaPropertyPointer:
            schemaProperty = new SchemaPropertyPointer(parent, typeToken.data, nameToken.data);
            break;
            
        case SchemaProperty::kSchemaPropertyReference:
            schemaProperty = new SchemaPropertyReference(parent, typeToken.data, nameToken.data);
            break;
        
        case SchemaProperty::kSchemaPropertyArray: // handled later
            break;
    }
    
    if (isArray)
    {
        schemaProperty = new SchemaPropertyArray(parent, typeToken.data, nameToken.data, schemaProperty);
    }
    
    schemaProperty->SetAttributes(attributes);
    
    return schemaProperty;
}

bool ParseStructContents(SchemaStruct* schemaStruct, Schema* schema, std::vector<Token>& tokens, size_t& counter)
{
    if (tokens[counter].type == Token::kTokenColon)
    {
        counter++;
        
        Token baseToken = tokens[counter++];
        if (baseToken.type != Token::kTokenVariable)
            return false; // TODO: Error missing variable
        
        schemaStruct->SetBaseType(baseToken.data);
    }
    
    if (tokens[counter++].type != Token::kTokenLeftBrace)
        return false; // TODO: Error (Missing left brace)
    
    while (counter < tokens.size())
    {            
        if (tokens[counter].type == Token::kTokenRightBrace)
        {
            counter++;
            break;
        }
        
        SchemaProperty* property = ParseProperty(schemaStruct, tokens, counter);
        
        if (!property)
            break;
        
        schemaStruct->AddProperty(property);
    }
    
    return true;
}

SchemaStruct* ParseStruct(Schema* schema, std::vector<Token>& tokens, size_t& counter)
{
    Token structToken = tokens[counter];
    
    if (structToken.type != Token::kTokenStruct)
        return 0;
    
    counter++;
    
    Token nameToken = tokens[counter++];
    
    if (nameToken.type != Token::kTokenVariable)
        return 0; // TODO: Error (Missing name)
    
    SchemaStruct* schemaStruct = new SchemaStruct(schema, nameToken.data);
    
    if (ParseStructContents(schemaStruct, schema, tokens, counter))
        return schemaStruct;
    
    delete schemaStruct;
    
    return 0;
}

SchemaRecord* ParseRecord(Schema* schema, std::vector<Token>& tokens, size_t& counter)
{
    Token entityToken = tokens[counter];
    
    if (entityToken.type != Token::kTokenRecord)
        return 0;
    
    counter++;
    
    Token nameToken = tokens[counter++];
    
    if (nameToken.type != Token::kTokenVariable)
        return 0; // TODO: Error (Missing name)
    
    SchemaRecord* record = new SchemaRecord(schema, nameToken.data);
    
    if (ParseStructContents(record, schema, tokens, counter))
        return record;
    
    delete record;
    
    return 0;
}

SchemaEntity* ParseEntity(Schema* schema, std::vector<Token>& tokens, size_t& counter)
{
    Token entityToken = tokens[counter];
    
    if (entityToken.type != Token::kTokenEntity)
        return 0;
    
    counter++;
    
    Token nameToken = tokens[counter++];
    
    if (nameToken.type != Token::kTokenVariable)
        return 0; // TODO: Error (Missing name)
    
    SchemaEntity* entity = new SchemaEntity(schema, nameToken.data);
    
    if (ParseStructContents(entity, schema, tokens, counter))
        return entity;
    
    delete entity;
    
    return 0;
}

bool Parse(Schema* schema, std::vector<Token>& tokens)
{
    return true;
}
    
bool SchemaParser::Parse(Schema* schema, const std::string& input)
{
    std::vector<Token> tokens = Tokenise(input);
    
    size_t counter = 0;
    
    Token token;
    
    while (counter < tokens.size())
    {
        std::vector<SchemaAttribute*> attributes;
        
        while (SchemaAttribute* attribute = ParseAttribute(tokens, counter))
            attributes.push_back(attribute);
        
        if (SchemaStruct* schemaStruct = ParseStruct(schema, tokens, counter))
        {
            schemaStruct->SetAttributes(attributes);
            schema->AddStruct(schemaStruct);
            continue;
        }
        
        if (SchemaRecord* schemaRecord = ParseRecord(schema, tokens, counter))
        {
            schemaRecord->SetAttributes(attributes);
            schema->AddRecord(schemaRecord);
            continue;
        }
        
        if (SchemaEntity* schemaEntity = ParseEntity(schema, tokens, counter))
        {
            schemaEntity->SetAttributes(attributes);
            schema->AddEntity(schemaEntity);
            continue;
        }
        
        // TODO: Error (Unknown item)
        break;
    }

    return true;
}
