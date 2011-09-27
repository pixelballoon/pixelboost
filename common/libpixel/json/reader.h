/**********************************************

License: BSD
Project Webpage: http://cajun-jsonapi.sourceforge.net/
Author: Terry Caton

***********************************************/

#pragma once

#include "elements.h"
#include <string>
#include <vector>

namespace json
{

class Reader
{
public:
   // this structure will be reported in one of the exceptions defined below
   struct Location
   {
      Location();

      unsigned int m_nLine;       // document line, zero-indexed
      unsigned int m_nLineOffset; // character offset from beginning of line, zero indexed
      unsigned int m_nDocOffset;  // character offset from entire document, zero indexed
   };

   // thrown during the first phase of reading. generally catches low-level problems such
   //  as errant characters or corrupt/incomplete documents
   class ScanException : public Exception
   {
   public:
      ScanException(const std::string& sMessage, const Reader::Location& locError) :
         Exception(sMessage),
         m_locError(locError) {}

      Reader::Location m_locError;
   };

   // thrown during the second phase of reading. generally catches higher-level problems such
   //  as missing commas or brackets
   class ParseException : public Exception
   {
   public:
      ParseException(const std::string& sMessage, const Reader::Location& locTokenBegin, const Reader::Location& locTokenEnd) :
         Exception(sMessage),
         m_locTokenBegin(locTokenBegin),
         m_locTokenEnd(locTokenEnd) {}

      Reader::Location m_locTokenBegin;
      Reader::Location m_locTokenEnd;
   };


   // if you know what the document looks like, call one of these...
   static void Read(Object& object, const std::string& str);
   static void Read(Array& array, const std::string& str);
   static void Read(String& string, const std::string& str);
   static void Read(Number& number, const std::string& str);
   static void Read(Boolean& boolean, const std::string& str);
   static void Read(Null& null, const std::string& str);

   // ...otherwise, if you don't know, call this & visit it
   static void Read(UnknownElement& elementRoot, const std::string& str);

private:
   struct Token
   {
      enum Type
      {
         TOKEN_OBJECT_BEGIN,  //    {
         TOKEN_OBJECT_END,    //    }
         TOKEN_ARRAY_BEGIN,   //    [
         TOKEN_ARRAY_END,     //    ]
         TOKEN_NEXT_ELEMENT,  //    ,
         TOKEN_MEMBER_ASSIGN, //    :
         TOKEN_STRING,        //    "xxx"
         TOKEN_NUMBER,        //    [+/-]000.000[e[+/-]000]
         TOKEN_BOOLEAN,       //    true -or- false
         TOKEN_NULL           //    null
      };

      Type nType;
      std::string sValue;

      // for malformed file debugging
      Reader::Location locBegin;
      Reader::Location locEnd;
   };

   class InputStream;
   class TokenStream;
   typedef std::vector<Token> Tokens;

   template <typename ElementTypeT>   
   static void Read_i(ElementTypeT& element, const std::string& str);

   // scanning istream into token sequence
   void Scan(Tokens& tokens, InputStream& inputStream);

   void EatWhiteSpace(InputStream& inputStream);
   void MatchString(std::string& sValue, InputStream& inputStream);
   void MatchNumber(std::string& sNumber, InputStream& inputStream);
   void MatchExpectedString(const std::string& sExpected, InputStream& inputStream);

   // parsing token sequence into element structure
   void Parse(UnknownElement& element, TokenStream& tokenStream);
   void Parse(Object& object, TokenStream& tokenStream);
   void Parse(Array& array, TokenStream& tokenStream);
   void Parse(String& string, TokenStream& tokenStream);
   void Parse(Number& number, TokenStream& tokenStream);
   void Parse(Boolean& boolean, TokenStream& tokenStream);
   void Parse(Null& null, TokenStream& tokenStream);

   const std::string& MatchExpectedToken(Token::Type nExpected, TokenStream& tokenStream);
};


} // End namespace


#include "reader.inl"