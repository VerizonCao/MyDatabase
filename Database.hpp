//  Created by rick gessner on 3/30/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  The in memory storage space for database, has a Storage class to store data into disk



#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include "Storage.hpp"
#include "Schema.hpp"
#include <sstream>
#include "Row.hpp"
#include "RowColloection.h"
#include "IndexManager.h"



using namespace std;

namespace ECE141 {
  using SchemaMap = std::map<std::string, Schema*>;

  class SchemaTOC{
  public:
      SchemaTOC(){};
      map<string, uint32_t> list;
  };
  
  class Database{
  public:
    
    Database(const std::string aPath, CreateNewStorage);
    Database(const std::string aPath, OpenExistingStorage);
    ~Database();

    //load meta and schemas
    StatusResult      loadMetaInfo();
    StatusResult      loadSchemas();

    //save the metaInfo, the block info into db
    StatusResult      saveMetaInfo();
    //save the schema into db
    StatusResult      saveSchema(Schema& aSchema);

    //does the database has this table?
    bool              hasTableName(string aName);

    //show all the table name and id
    StatusResult      showTables();

    //describe a table
    StatusResult      describeTable(string aName);

    int               getTableId(const string aName);

    //save data in next block in default, or in blockNum
    StatusResult      saveData(string theName, int id, Row& aRow, int blockNum = -1);

    //set table id, which is the num of data rows in this table
    Database&         setTableId(int num, string aName);

    //change String to DataType
    ValueType&        changeStringToDataType(string x, DataType aType);

    //index methods
    StatusResult      saveIndexInfo();
    StatusResult      saveIndex(string tableName, string indexName);
    StatusResult      saveAllIndex();


    StatusResult      loadIndexTable();   //load all the exist index in meta2
    StatusResult      loadIndex(string tableName, string indexName);   //load the specific index for a table if exists


    //alter methods
    StatusResult      addNULLToRow(Row& aRow, const Attribute& attribute);




    //getter and setter
    Storage&          getStorage() {return storage;}
    std::string&      getName() {return name;}
    SchemaTOC&        getToc(){return toc;}
    SchemaMap&        getMap(){return schemas;}
    int               getId(){return id;}
    Database&         setId(int num){id = num; return *this;}

    IndexManager      manager;



  protected:
    std::string     name;
    Storage         storage;
    SchemaTOC       toc;
    SchemaMap       schemas;
    int             id;



    //used for describe database, change the DataType to string
    std::map<DataType, std::string>    MapToType = { make_pair(DataType::int_type,"integer"),
                                                     make_pair(DataType::float_type,"float"),
                                                     make_pair(DataType::datetime_type,"date"),
                                                     make_pair(DataType ::varchar_type,"varchar"),
                                                     make_pair(DataType::bool_type,"boolean"),

    };

  };
  
}

#endif /* Database_hpp */
