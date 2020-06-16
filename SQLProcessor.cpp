//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.


#include "SQLProcessor.hpp"
#include <string>
#include <algorithm>

namespace ECE141 {


    SQLProcessor::SQLProcessor(CommandProcessor *aNext){
        next = aNext;
        activeDB = nullptr;
    }

    Statement*    SQLProcessor::getStatement(Tokenizer &aTokenizer){

        std::vector<ECE141::Keywords> theTerms{
                {Keywords::create_kw, Keywords::drop_kw,Keywords::show_kw, Keywords::describe_kw, Keywords::delete_kw }
        };

        std::string currentName = "";
        Token & theToken = aTokenizer.current();

        if(aTokenizer.peek(1).keyword == Keywords::database_kw || aTokenizer.peek(1).keyword == Keywords::databases_kw){
            return nullptr;
        }

        activeDB = getActiveDatabase();

        //select
        if(theToken.keyword == Keywords::select_kw){
            SelectStatement* temp = new SelectStatement(activeDB,theToken.keyword);
            temp->parse(aTokenizer);
            return temp;
        }
        //update
        else if(theToken.keyword == Keywords::update_kw){
           UpdateStatement* temp = new UpdateStatement(activeDB,theToken.keyword);
           temp->parse(aTokenizer);
           return temp;
        }
        else if(theToken.keyword == Keywords::show_kw){
            if(aTokenizer.peek(1).data == "indexes"){
                ShowIndexStatement* temp = new ShowIndexStatement(activeDB,theToken.keyword);
                temp->parse(aTokenizer);
                return temp;
            }
        }
        else if(theToken.keyword == Keywords::alter_kw){
            AlterStatement* temp = new AlterStatement(activeDB,theToken.keyword);
            temp->parse(aTokenizer);
            return temp;
        }

        while(aTokenizer.current().data != ";"){

            if(aTokenizer.current().keyword == Keywords::unknown_kw && aTokenizer.current().type == TokenType::identifier){
                currentName += aTokenizer.current().data;
            }
            //if it is create or insert
            else if(aTokenizer.current().data == "("){
                if(theToken.keyword == Keywords::create_kw){
                    CreateTableStatement* temp = new CreateTableStatement(currentName,activeDB,theToken.keyword);
                    temp->parse(aTokenizer);
                    aTokenizer.next();  //jump the punctuation ')'
                    return temp;
                }
                else if(theToken.keyword == Keywords::insert_kw){
                    InsertTableStatement* temp = new InsertTableStatement(currentName,activeDB,theToken.keyword);
                    temp->parse(aTokenizer);
                    return temp;
                }
            }
            if(!aTokenizer.next()){
                break;
            }
        }
        auto it = std::find(theTerms.begin(), theTerms.end(), theToken.keyword);
        if(it != theTerms.end()){
            switch(theToken.keyword){
                case Keywords::drop_kw:      return new DropTableStatement(currentName,activeDB,theToken.keyword);
                case Keywords::show_kw:      return new ShowTableStatement(currentName,activeDB,theToken.keyword);
                case Keywords::describe_kw:  return new DescribeTableStatement(currentName,activeDB,theToken.keyword);
                case Keywords::delete_kw:    return new DeleteTableStatement(currentName,activeDB,theToken.keyword);
                default: break;
            }
        }
        return nullptr;
    }

    StatusResult  SQLProcessor::interpret(const Statement &aStatement){
        return aStatement.run(cout);
    }

}
