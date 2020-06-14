//  Created by rick gessner on 4/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  used for 3 commands: help, version and quit

#ifndef AppProcessor_hpp
#define AppProcessor_hpp

#include <stdio.h>
#include "CommandProcessor.hpp"

namespace ECE141 {


  class AppCmdProcessor : public CommandProcessor {
  public:
    
    AppCmdProcessor(CommandProcessor *aNext=nullptr);
    virtual ~AppCmdProcessor(){};

    virtual Statement*    getStatement(Tokenizer &aTokenizer);
    virtual StatusResult  interpret(const Statement &aStatement);
    
  };

}

#endif /* AppProcessor_hpp */
