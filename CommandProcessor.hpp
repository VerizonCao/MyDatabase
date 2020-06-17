//  Created by rick gessner on 3/17/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  the interface for 3 processor, has basic functions: getStatement, interpret and getDatabase


#ifndef CommandProcessor_hpp
#define CommandProcessor_hpp

#include <stdio.h>
#include <string>

#include "Interface/Statement.hpp"

namespace ECE141 {
  
  class Statement;
  class Tokenizer;
  class Database;


  
  //------ serves as base class for our 3 command-processors --------------

  class CommandProcessor { //processor interface
  public:
                          CommandProcessor(CommandProcessor *aNext=nullptr);
                          ~CommandProcessor();
    
    virtual StatusResult  processInput(Tokenizer &aTokenizer);


    virtual Statement*    getStatement(Tokenizer &aTokenizer)=0;
    virtual StatusResult  interpret(const Statement &aStatement)=0;
    virtual Database*     getActiveDatabase();

    CommandProcessor *next;
  };
   
}

#endif /* CommandProcessor_hpp */
