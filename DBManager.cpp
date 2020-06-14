//
// Created by Cao Zongheng on 4/13/20.
//

#include "DBManager.h"
#include "Folders/FolderView.hpp"
#include "Folders/FolderReader.hpp"
#include <iostream>
#include <algorithm>
#include <string>






namespace ECE141{

    DBManager::DBManager(CommandProcessor *aNext){
        next = aNext;
        activeDB = nullptr;
    }

    Statement*    DBManager::getStatement(Tokenizer &aTokenizer){
        std::vector<ECE141::Keywords> theTerms{
                {Keywords::create_kw, Keywords::drop_kw, Keywords::use_kw, Keywords::show_kw, Keywords::describe_kw }
        };

        std::string currentName = "";
        Token & theToken = aTokenizer.current();

        if(!aTokenizer.more()) return nullptr;    //avoid bug likes databse
        if(aTokenizer.peek(1).keyword != Keywords::database_kw && aTokenizer.peek(1).keyword != Keywords::databases_kw){
            return nullptr;
        }

        while(aTokenizer.more()&&aTokenizer.current().data != ";"){
            if(aTokenizer.current().keyword == Keywords::unknown_kw){
                currentName += aTokenizer.current().data;
            }
            aTokenizer.next();
        }
        auto it = std::find(theTerms.begin(), theTerms.end(), theToken.keyword);
        if(it != theTerms.end()){
            if(theToken.keyword == Keywords::describe_kw) return new DescribeStatement(currentName,theToken.keyword);
            else return new DBStatement(currentName,theToken.keyword);   //make a statement and its type
        }
        return nullptr;
    }


    StatusResult  DBManager::interpret(const Statement &aStatement){
        switch(aStatement.getType()){
            case Keywords::create_kw:   return doCreateStatement(aStatement);
            case Keywords::drop_kw:  return doDropStatement(aStatement);
            case Keywords::use_kw:  return doUseStatement(aStatement);
            case Keywords::show_kw:  return doShowStatement(aStatement);
            case Keywords::describe_kw:  return aStatement.run(std::cout);
            default: break;
        }
        return StatusResult();
    }

    StatusResult DBManager::doCreateStatement(const Statement& theStatement){
        return createDatabases(theStatement.getName());
    }

    StatusResult DBManager::doDropStatement(const Statement& theStatement){
        return dropDatabases(theStatement.getName());
    }

    StatusResult DBManager::doUseStatement(const Statement& theStatement){
        return useDatabases(theStatement.getName());
    }

    StatusResult DBManager::doShowStatement(const Statement& theStatement){
        return showDatabases();
    }


    //revise add an index meta into the second block
    StatusResult DBManager::createDatabases(std::string name){
        FolderReader theReader(StorageInfo::getDefaultStoragePath());
        if(!theReader.exists(name)){
            try{
                Database database(name,CreateNewStorage{});
                database.saveIndexInfo(); //create and save the index meta
                std::cout << "Create Database "<< name << "\n";
                return StatusResult();
            }
            catch (...){

            }
        }
        return StatusResult(databaseExists);
    }

    StatusResult DBManager::dropDatabases(std::string name){
        if(activeDB != nullptr && name == activeDB->getName()){
            releaseDB();
            activeDB = nullptr;
        }
        FolderReader theReader(StorageInfo::getDefaultStoragePath());
        if(theReader.exists(name)){  //has this file
            if(theReader.drop(name)){
                std::cout << "Drop Database "<< name << "\n";
                return StatusResult();
            }
            return StatusResult(unknownError);
        }
        else{
            return StatusResult(unknownDatabase);
        }
    }

    StatusResult DBManager::useDatabases(std::string name){
        FolderReader theReader(StorageInfo::getDefaultStoragePath());
        if(activeDB!= nullptr && activeDB->getName() == name){   // it already use this
            return StatusResult();
        }
        //not in use
        if(theReader.exists(name)){
            if(activeDB != nullptr){
                releaseDB();
                activeDB = nullptr;
            }
            if((activeDB = new Database(name, OpenExistingStorage{}))){
                std::cout << "Use Database "<< name << "\n";
                return StatusResult();
            }
            else return StatusResult(unknownError);
        }
        else{
            return StatusResult(unknownDatabase);
        }
    }


    StatusResult DBManager::showDatabases(){
        FolderReader theReader(StorageInfo::getDefaultStoragePath());
        FolderView theView(theReader,".db");
        theView.show(std::cout);
        return StatusResult();
    }

    //release
    void DBManager::releaseDB(){
        if(activeDB != nullptr){
            delete activeDB;
            activeDB = nullptr;
        }
    }

}
