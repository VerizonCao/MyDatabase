//  Created by rick gessner on 3/19/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.


#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <iostream>
#include <vector>
#include "Other/keywords.hpp"

namespace ECE141 {


  //定义符号
  const char left_paren = '(';
  const char right_paren = ')';
  const char left_bracket = '[';
  const char right_bracket = ']';
  const char left_brace = '{';
  const char right_brace = '}';
  const char colon = ':';
  const char comma = ',';
  const char dot = '.';
  const char semicolon = ';';
  const char apostrophe = '\'';
  const char quote = '"';


  //The method name, return type, and arguments essentially define the "type"
  using parseCallback = bool(char aChar);



  enum class TokenType {
    function, identifier, keyword, number, operators, punctuation, string, unknown
  };
  
  //-----------------


  struct Token {

    // = operator
    Token& operator=(const Token &aCopy) {
      type=aCopy.type;
      keyword=aCopy.keyword;
      data=aCopy.data;
      return *this;
    }

    bool operator == (Token &aToken){
        if(type == aToken.type && keyword == aToken.keyword
           && data == aToken.data)
            return true;
         else return false;
    }

    bool operator != (Token &aToken){
        return !( *this == aToken);
    }
    
    TokenType   type;
    Keywords    keyword;
    Operators   op;
    std::string data;
  };
 
  //-----------------
  
  class Tokenizer {
  public:
    Tokenizer(std::istream &anInputStream);

    
    StatusResult  tokenize();
    Token&        tokenAt(size_t anOffset);

    Token&        current();
    bool          more() {return index<size();}
    bool          more1() { return  index + 1 < size();}
    bool          next(int anOffset=1);
    Token&        peek(int anOffset=1);
    void          restart() {index=0;}
    size_t        size() {return tokens.size();}
    size_t        remaining() {return index<size() ? size()-index :0;}

                          //these might consume a token...
    bool          skipIf(Keywords aKeyword);
    bool          skipIf(Operators anOperator);
    bool          skipIf(TokenType aTokenType);
    bool          skipIf(char aChar);

    void          dump(); //utility

    bool operator==(Tokenizer &aTokenizer) ;


  protected:
    
    std::string   readWhile(parseCallback aCallback);
    std::string   readUntil(char aTerminal, bool addTerminal=false);
    std::string   readUntil(parseCallback aCallback, bool addTerminal=false);


    std::vector<Token>    tokens;
    std::istream          &input;
    size_t                index;
  };
  
}

#endif /* Tokenizer_hpp */

