//  Created by rick gessner on 3/17/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  Some reminders:
//  main function of the whole project: click run......
//  The user could type commands into Terminal or use script to insert commands
//  Before click run, set an environment variable, 'DB_PATH' : the place you want to save your db.file
//  The code might have some bugs and forget to solve some corner case, just do it yourself.
//  Delete the makeFile and CMakeList and replace them with yours.

#include <iostream>
#include <sstream>
#include <fstream>

#include "AppProcessor.hpp"
#include "Tokenizer.hpp"
#include "Other/Errors.hpp"
#include "DBManager.h"
#include "SQLProcessor.hpp"



static std::map<int, std::string> theErrorMessages = {
  {ECE141::illegalIdentifier, "Illegal identifier"},
  {ECE141::unknownIdentifier, "Unknown identifier"},
  {ECE141::databaseExists, "Database exists"},
  {ECE141::tableExists, "Table Exists"},
  {ECE141::syntaxError, "Syntax Error"},
  {ECE141::unknownCommand, "Unknown command"},
  {ECE141::unknownDatabase,"Unknown database"},
  {ECE141::unknownTable,   "Unknown table"},
  {ECE141::unknownError,   "Unknown error"}
};

void showError(ECE141::StatusResult &aResult) {
  std::string theMessage="Unknown Error";
  if(theErrorMessages.count(aResult.code)) {
    theMessage=theErrorMessages[aResult.code];
  }
  std::cout << "Error (" << aResult.code << ") " << theMessage << "\n";
}

//build a tokenizer, tokenize input, ask processors to handle...
ECE141::StatusResult handleInput(std::istream &aStream, ECE141::CommandProcessor &aProcessor) {
  ECE141::Tokenizer theTokenizer(aStream);
  
  //tokenize the input from aStream...
  ECE141::StatusResult theResult=theTokenizer.tokenize();
  while(theResult && theTokenizer.more()) {
    if(";"==theTokenizer.current().data) {
      theTokenizer.next();  //skip the ";"...
    }
    //find the right one
    else theResult=aProcessor.processInput(theTokenizer);
  }
  return theResult;
}


//----------------------------------------------

int main(int argc, const char * argv[]) {


  ECE141::AppCmdProcessor   theProcessor;  //add your db processor here too!
  ECE141::StatusResult      theResult{};
  ECE141::DBManager         theManager;    //this is my dbProcessor
  ECE141::SQLProcessor      theSQL;

  theProcessor.next = &theSQL;
  theSQL.next = &theManager;

  if(argc>1) {
      std::ifstream theStream(argv[1]);
      return handleInput(theStream, theProcessor);
  }
  else {
    std::string theUserInput;
    bool running=true;
    do {
      std::cout << "\n> ";
      if(std::getline(std::cin, theUserInput)) {
        if(theUserInput.length()) {
          std::stringstream theStream(theUserInput);
          theResult=handleInput(theStream, theProcessor);
          if(!theResult) showError(theResult);
        }
        if(ECE141::userTerminated==theResult.code)
          running=false;
      }
    }
    while (running);
  }

  return 0;
}

