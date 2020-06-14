//
//  SQLProcessor.hpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

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



/*  do these in next assignment
    StatusResult insert();
    StatusResult update();
    StatusResult delete();
*/
    
  protected:

      Database* activeDB;
    
    //do you need other data members?
//    std::vector<Schema> list;
  };

}
#endif /* SQLProcessor_hpp */
