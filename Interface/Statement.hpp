//  Created by rick gessner on 3/20/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  interface for all Statements, has some basic methods: run, parse......


#ifndef Statement_hpp
#define Statement_hpp

#include "../Other/keywords.hpp"
#include <iostream>
#include "../Schema.hpp"



//什么是statement
namespace ECE141 {


  //根据 tokenizer 来创建  statement
  class Tokenizer;
  
  class Statement {
  public:
    Statement(Keywords aStatementType=Keywords::unknown_kw);
    Statement(const Statement &aCopy);
    
    virtual                 ~Statement();
    

    //getter
    Keywords                getType() const {return stmtType;}


    virtual   StatusResult  run(std::ostream &aStream) const;
    virtual   StatusResult  parse(Tokenizer &aTokenizer);
    virtual   std::string   getName() const{ return "name";}



      //唯一成员变量  Keywords
  protected:
    Keywords   stmtType;    
  };
  
}

#endif /* Statement_hpp */
