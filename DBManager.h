//
// Created by Cao Zongheng on 4/13/20.
//
// used for database command, like: use db, create db, delete db, describe db.....

#ifndef ECE141_SP20_ASSIGNMENT2_ZONGHENG_CAO_DBMANAGER_H
#define ECE141_SP20_ASSIGNMENT2_ZONGHENG_CAO_DBMANAGER_H


#include <iostream>
#include "AppProcessor.hpp"
#include "Tokenizer.hpp"
#include <memory>
#include <vector>
#include "CommandProcessor.hpp"
#include "Storage.hpp"
#include "DBStatement.h"
#include "Database.hpp"

namespace ECE141{

    class DBManager : public CommandProcessor {

    public:

        DBManager(CommandProcessor *aNext=nullptr);

        ~DBManager(){
            if(activeDB != nullptr){
                delete activeDB;
                activeDB = nullptr;
            }
        };


        StatusResult createDatabases(std::string name);
        StatusResult dropDatabases(std::string name);
        StatusResult useDatabases(std::string name);
        StatusResult showDatabases();


        StatusResult doCreateStatement(const Statement& theStatement);
        StatusResult doDropStatement(const Statement& theStatement);
        StatusResult doUseStatement(const Statement& theStatement);
        StatusResult doShowStatement(const Statement& theStatement);


        //virtual
        virtual Statement*    getStatement(Tokenizer &aTokenizer);
        virtual StatusResult  interpret(const Statement &aStatement);
        virtual Database*     getActiveDatabase(){return activeDB;}

        void releaseDB();


        Database* activeDB;

    };

}





#endif //ECE141_SP20_ASSIGNMENT2_ZONGHENG_CAO_DBMANAGER_H
