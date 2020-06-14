//  Created by rick gessner on 3/30/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.

#include <iostream>
#include "AppProcessor.hpp"
#include "Tokenizer.hpp"
#include <memory>
#include <vector>
#include <algorithm>

namespace ECE141 {


  //.....................................

  AppCmdProcessor::AppCmdProcessor(CommandProcessor *aNext) : CommandProcessor(aNext) {
  }


  //1 help
  StatusResult doHelpStatement(const Statement& theStatement){
      std::cout << "help -- the available list of commands shown below:\n";
      std::cout << "-- help - shows this list of commands\n";
      std::cout << "-- version -- shows the current version of this application\n";
      std::cout << "-- create database <name> -- creates a new database\n";
      std::cout << "-- drop database <name> -- drops the given database\n";
      std::cout << "-- use database <name>  -- uses the given database\n";
      std::cout << "-- describe database <name>  -- describes the given database\n";
      std::cout << "-- show databases   -- shows the list of databases available\n";
      return StatusResult();
  }

  //2 version
  StatusResult doVersionStatement(const Statement& theStatement){
      std::cout << "version ECE141b-10";
      return StatusResult();
  }

  StatusResult doQuitStatement(const Statement& theStatement){
      return StatusResult(Errors::userTerminated);
  }


  // USE: -----------------------------------------------------
  StatusResult AppCmdProcessor::interpret(const Statement &aStatement) {
    switch(aStatement.getType()){   //type which is a Keywords
        case Keywords::help_kw:   return doHelpStatement(aStatement);  //if debugging clog
        case Keywords::version_kw:  return doVersionStatement(aStatement);
        case Keywords::quit_kw:  return doQuitStatement(aStatement);
        default: break;
    }
    return StatusResult();
  }
  
  // USE: factory to create statement based on given tokens...
  Statement* AppCmdProcessor::getStatement(Tokenizer &aTokenizer) {
    std::vector<ECE141::Keywords> theTerms{
            {Keywords::help_kw, Keywords::version_kw, Keywords::quit_kw}
    };
    Token & theToken = aTokenizer.current();
    auto it = std::find(theTerms.begin(), theTerms.end(), theToken.keyword);
    if(it != theTerms.end()){
        aTokenizer.next();
        return new Statement(theToken.keyword);   //make a statement and its type
    }
    return nullptr;
  }
  
}
