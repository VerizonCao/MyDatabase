//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  used for SQL commands, like:  show tables, select, create table, delete table
//  update data, alter table, describe table....

#ifndef SQLProcessor_hpp
#define SQLProcessor_hpp

#include <stdio.h>
#include <stdlib.h>
#include "CommandProcessor.hpp"
#include "Tokenizer.hpp"
#include "Schema.hpp"
#include "SQLStatement.h"
#include "Database.hpp"
#include "Storage.hpp"
#include "StorageBlock.hpp"
#include <sstream>


class Statement;
class Database; //define this later...

namespace ECE141 {

  class SQLProcessor : public CommandProcessor {
  public:
    
    SQLProcessor(CommandProcessor *aNext=nullptr);
    virtual ~SQLProcessor(){

    };
    
    virtual Statement*    getStatement(Tokenizer &aTokenizer);
    virtual StatusResult  interpret(const Statement &aStatement);


    
  protected:

      Database* activeDB;
  };

}
#endif /* SQLProcessor_hpp */
