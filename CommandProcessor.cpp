//  Created by rick gessner on 3/30/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.


#include <iostream>
#include "CommandProcessor.hpp"
#include "Interface/Statement.hpp"
#include "Tokenizer.hpp"
#include <memory>

namespace ECE141 {
  
  CommandProcessor::CommandProcessor(CommandProcessor *aNext) : next(aNext) {
  }

  CommandProcessor::~CommandProcessor() {}
  
  // USE: retrieve active db (if available) to subystem...
  Database* CommandProcessor::getActiveDatabase() {
    return next ? next->getActiveDatabase() : nullptr;
  }

  // USE: try to make/run statement from tokens; return error if you can't...
  //      This version will work fine for you in most cases...
  StatusResult CommandProcessor::processInput(Tokenizer &aTokenizer) {
    //get a statement, if not, find next. else use interpret
    std::unique_ptr<Statement> theStatement(getStatement(aTokenizer));
    if(theStatement) {
      return interpret(*theStatement);
    }
    else if(next) {
      return next->processInput(aTokenizer);
    }
    return StatusResult{ECE141::unknownCommand};
  }

}
