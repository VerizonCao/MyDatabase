//
// Created by Cao Zongheng on 4/24/20.
//

#include "SQLStatement.h"
#include <string>
#include "Attribute.hpp"
#include <sstream>


//this class is used for table service, like create a table
//delete a table, insert some data and update the data as well
//as select operation


namespace ECE141{


    StatusResult  CreateTableStatement::run(std::ostream &aStream) const{
        if(database == nullptr) return StatusResult(Errors::noDatabaseSpecified);
        string theName = schema->getName();
        if(database->hasTableName(theName)) return StatusResult(Errors::tableExists);
        database->getMap()[theName] = schema;

        // ***********  check if the table has a primary key
        string temp = schema->getPrimaryKey();
        if(temp != ""){
            //need create a index in the Toc by the primary key, and create an empty index
            Index* tempIndex = new Index(temp,theName,DataType::int_type);
            //add into memory
            database->manager.addIndexIntoMap(*tempIndex,theName,temp);
            int blockNum = database->getId() + 1;
            database->manager.addIndexIntoToc(theName,temp, blockNum);
            //add into db.file
            database->saveIndex(theName,temp);
            database->saveIndexInfo();
        }
        // **************

        //store the table and meta info into hardware
        database->getToc().list[theName] = database->getId() + 1;
        if(database->saveSchema(*schema) && database->saveMetaInfo()){
            std::cout << "Create Table "<< name << "\n";
            database->setId(database->getId() + 1);   //if success, let the id + 1
            database->saveMetaInfo();  //save into database
            return StatusResult();
        }
        else return StatusResult(Errors::writeError);
    }

    //drop table need to drop the table block and data block
    //and index block and update index info(decrease the index of that table in index table)
    StatusResult  DropTableStatement::run(std::ostream &aStream) const{
        if(database == nullptr) return StatusResult(Errors::noDatabaseSpecified);
        if(!database->hasTableName(name)) return StatusResult(Errors::unknownTable);

        //look if it has a primary key
        string indexName = database->getMap()[name]->getPrimaryKey();
        if(indexName == ""){  //right now, if no primary key, we assume that no other index
            //iterate block and delete the related block
            int id = database->getId();
            for(int i = 1; i<=id; i++){
                StorageBlock aBlock(BlockType::unknown_block);
                database->getStorage().readBlock(aBlock,i);
                if(aBlock.header.type == 'D'){
                    char table[data_size];  memcpy(&table,aBlock.data, sizeof(aBlock.data));
                    string table1 = table;
                    int index = table1.find(";");
                    string theName = table1.substr(0,index);
                    if(name == theName){
                        database->getStorage().writeBlank(i);
                    }
                }
            }
        }
        else{
            //load and get the primary index
            database->loadIndex(name,indexName);
            Index temp = database->manager.getIndex(name,indexName);
            //delete the data by index
            for(auto it : temp.getList()){
                int blockNum = it.second;
                database->getStorage().writeBlank(blockNum);  //write blank
            }
        }

        //delete all index block
        //delete the index of a table (meta index)
        database->manager.deleteAllIndexOfTable(name, database->getStorage());
        database->manager.getMap().erase(name);
        database->manager.getToc().erase(name);
        database->saveIndexInfo();

        //delete in toc and map, also write a blank into disk
        int index = database->getToc().list[name];
        database->getToc().list.erase(name);
        database->getMap().erase(name);
        // save meta and write blank
        if(database->getStorage().writeBlank(index) && database->saveMetaInfo()){
            std::cout << "Drop Table "<< name << "\n";
            return StatusResult();
        }
        else return StatusResult(Errors::unknownError);
    }

    StatusResult ShowTableStatement::run(std::ostream &aStream) const{
        if(database == nullptr) return StatusResult(Errors::noDatabaseSpecified);
        return database->showTables();
    }

    StatusResult DescribeTableStatement::run(std::ostream &aStream) const {
        if(database == nullptr) return StatusResult(Errors::noDatabaseSpecified);
        return database->describeTable(name);
    }

    //we need to add into the map
    StatusResult InsertTableStatement::run(std::ostream &aStream) const{
        if(database == nullptr) return StatusResult(Errors::noDatabaseSpecified);
        if(!database->hasTableName(name)) return StatusResult(Errors::unknownTable);
        //initialize a validator
        Validator s(database->getMap());

        if(!s.checkAttributeName(properties,name)) return StatusResult(Errors::syntaxError);   //check name
        if(!s.checkAttributeType(rowList,name)) return StatusResult(Errors::syntaxError);   //check type

        //maybe we need first load the index into memory?
        IndexManager& tempManager = database->manager;
        for(auto it : tempManager.getToc()){
            if(!tempManager.isIndexLoaded(name, it.first)){
                database->loadIndex(name, it.first);
            }
        }

        for(Row* i : rowList){
            int id = database->getTableId(name);
            database->saveData(name,id,*i,-1);   //save it into next block  include id + 1
            int blockNum = database->getId();  //store the blockNum quickly

            //index stuff
            map<string, Index> temp = database->manager.getMap()[name];
            for(auto& it : temp){  //for all its indexes
                string indexName = it.first;
                ValueType* tempType = new ValueType();
                *tempType = i->pair2[indexName];
                tempManager.getMap()[name][indexName].addKeyValue(*tempType, blockNum);   //add a ValueType and a blockNum
            }
        }

        //save all the index
        database->saveAllIndex();

        //save meta and table into db file
        database->saveSchema(*database->getMap()[name]);
        database->saveMetaInfo();   //only because it increase.
        return StatusResult();
    }


    StatusResult  DeleteTableStatement::run(std::ostream &aStream) const{
        if(database == nullptr) return StatusResult(Errors::noDatabaseSpecified);
        if(!database->hasTableName(name)) return StatusResult(Errors::unknownTable);

        Timer aTimer;
        aTimer.start();
        //delete all the data of the table and the index for the table, but not the index info
        string indexName = database->getMap()[name]->getPrimaryKey();

        if(indexName == "") {   //if no primary data
            int id = database->getId();  //how many block we have
            for(int i = 1; i<=id; i++){
                StorageBlock aBlock(BlockType::unknown_block);
                database->getStorage().readBlock(aBlock,i);
                if(aBlock.header.type == 'D'){
                    char table[data_size];  memcpy(&table,aBlock.data, sizeof(aBlock.data));
                    string table1 = table;
                    int index = table1.find(";");
                    string theName = table1.substr(0,index);   //table's name
                    if(name == theName){
                        database->getStorage().writeBlank(i);
                        database->setTableId(-1,name);   //this is necessary, at the end, it becomes 1 or 0?
                    }
                }

            }
        }
        else{
            if(!database->manager.isIndexLoaded(name,indexName)) database->loadIndex(name,indexName);
            Index temp = database->manager.getIndex(name,indexName);  //get the primary index
            for(auto it : temp.getList()){  //delete the data by index
                int blockNum = it.second;
                database->getStorage().writeBlank(blockNum);  //write blank
                database->setTableId(-1,name);   //this is necessary, at the end, it becomes 1 or 0?
            }
        }

        //delete the index itself in db.file, but not in index meta info
        database->manager.deleteAllIndexOfTable(name, database->getStorage());
        database->saveMetaInfo();

        aTimer.stop();
        double time = aTimer.elapsed();
        cout <<"1 rows affected (" << time << " ms.)\n";  //now we don't realize delete where

        return StatusResult();
    }

    StatusResult  ShowIndexStatement::run(std::ostream &aStream) const{
        if(database == nullptr) return StatusResult(Errors::noDatabaseSpecified);
        else return database->manager.showIndexes();
    }


    Expression*  getMatchExpression(IndexManager& indexManager, string name, const Filters& filters){
        for(auto it : filters.getExpression()){
            //check if this index exist
            string indexName = it->lhs.name;
            if(indexManager.isIndexExisted(name,indexName)){
                return it;  //find one, use it
            }
        }
        return nullptr;
    }

    StatusResult addARow(StorageBlock& aBlock, RowColloection* aRowCollections, Database* database, string name, int blockNum){
        if(aBlock.header.type == 'D'){  //check if it is data block
            char data[data_size];
            memcpy(data,aBlock.data,data_size);
            string data1 = data;
            int nameIndex = data1.find(";");
            AttributeList list = database->getMap()[name]->getAttributes();  //get list
            string theName = data1.substr(0,nameIndex);
            int index = 0;
            if(theName == name){
                Row* aRow = new Row();
                aRow->blockId = blockNum;
                aRow->headerId = aBlock.header.id;
                int start = nameIndex + 1;
                for(int i = nameIndex+1; i<data1.length(); i++){
                    if(data1[i] == ','){
                        string temp = data1.substr(start,i-start);
                        int dataIndex = temp.find(":");
                        string tempData = temp.substr(dataIndex+1,temp.size() - dataIndex - 1);
                        DataType tempType = list[index].getType();
                        ValueType type = database->changeStringToDataType(tempData,tempType);
                        KeyValuePair tempPair = make_pair(list[index].getName(),type);
                        aRow->getPair().push_back(tempPair);
                        //used for order
                        aRow->pair2[list[index].getName()] = type;
                        start = i + 1;
                        index++;
                    }
                }
                aRowCollections->addRow(*aRow);
            }
        }
        return StatusResult();
    }


    RowColloection& getRowColloections(Database* database, string name, const Filters& filters){
        //case 1: check if the expressions match the index, include id
        //case 2: if not 1, then 2, we use primary key to avoid iterate
        RowColloection* aRowCollections = new RowColloection();

        Expression* matchEx = getMatchExpression(database->manager, name, filters);

        //case 1
        if(matchEx != nullptr){
            string matchIndex = matchEx->lhs.name;
            if(!database->manager.isIndexLoaded(name,matchIndex)) database->loadIndex(name,matchIndex);
            Index tempIndex = database->manager.getIndex(name, matchIndex);
            // do something, like  = , < , >
            for(auto it : tempIndex.getList()){
                //check if each index satisfy the requirements
                ValueType temp = it.first;
                if((*matchEx)(temp)){
                    //add this row
                    StorageBlock aBlock(BlockType::unknown_block);
                    database->getStorage().readBlock(aBlock, it.second);
                    addARow(aBlock, aRowCollections, database, name, it.second);   //use the function above
                }
            }
            return *aRowCollections;
        }
        //case 2
        else if(database->getMap()[name]->getPrimaryKey() != ""){
            string key = database->getMap()[name]->getPrimaryKey();
            if(!database->manager.isIndexLoaded(name,key)) database->loadIndex(name,key);
            Index tempIndex = database->manager.getIndex(name, key);
            for(auto it : tempIndex.getList()){
                StorageBlock aBlock(BlockType::unknown_block);
                database->getStorage().readBlock(aBlock, it.second);
                addARow(aBlock, aRowCollections, database, name, it.second);   //use the function above
            }

        }
        else{  //no primary index
            int id = database->getId();
            for(int i = 2; i<=id; i++){   //界限
                StorageBlock aBlock(BlockType::data_block);
                database->getStorage().readBlock(aBlock,i);
                if(aBlock.header.type == 'D'){
                    char data[data_size];
                    memcpy(data,aBlock.data,data_size);
                    string data1 = data;
                    int nameIndex = data1.find(";");
                    AttributeList list = database->getMap()[name]->getAttributes();  //get list
                    string theName = data1.substr(0,nameIndex);
                    if(theName == name){
                        addARow(aBlock, aRowCollections, database, name, i);
                    }
                }
            }
        }
        return *aRowCollections;
    }

    StatusResult  SelectStatement::run(std::ostream &aStream) const{
        //check table name and database
        if(database == nullptr) return StatusResult(Errors::noDatabaseSpecified);
        if(!database->hasTableName(name)) return StatusResult(Errors::unknownTable);
        //initialize a validator
        Validator s(database->getMap());

        //add the join table name also in the fucntion. if not, add ""
        string joiunTableName = joins.size() > 0 ? joins[0].table : "";
        if(!s.checkTableViewName(name,tableView.getShowName(),joiunTableName)) return StatusResult(Errors::unknownAttribute);
        if(filters.order) if(!s.checkWhereAndOrder(name,filters.orderName,joiunTableName)) return StatusResult(Errors::unknownAttribute);
        for(auto it : filters.getExpression()){
            if(!s.checkWhereAndOrder(name,it->lhs.name,joiunTableName)) return StatusResult(Errors::unknownAttribute);
        }

        Timer aTimer;
        aTimer.start();
        //add all the rows into row collection
        RowColloection aRowCollections =  getRowColloections(database, name, filters);

        //if have join, we add the attribute of the join
        if(joins.size() > 0){
            if(!checkJoinName()) return StatusResult(Errors::syntaxError);
            RowColloection aRowCollections1 =  getRowColloections(database, joiunTableName, filters);
            addJoinRowsToRows(aRowCollections, aRowCollections1);
        }

        aTimer.stop();
        double time = aTimer.elapsed();

        //filter
        if(filters.getCount() > 0){
            for(auto it = aRowCollections.list.begin(); it != aRowCollections.list.end(); ){
                if(!filters.match(*it)){
                    aRowCollections.list.erase(it);
                }
                else it++;
            }
        }
        //if we need order
        if(filters.order){
            filters.sortCollections(&aRowCollections);
        }
        //if we need limit and show
        bool join = joins.size() > 0;
        tableView.ShowRowList(aRowCollections,filters.limit ? filters.limitNumber : -1, time, join);
        return StatusResult();
    }

    StatusResult SelectStatement::addJoinRowsToRows(RowColloection& aRowCollections, RowColloection& aRowCollections1) const{

        Schema* schemaLeft = database->getMap()[name];
        Schema* schemaRight = database->getMap()[joins[0].table];

        bool isPrimaryLeft = schemaLeft->getPrimaryKey() == joins[0].getFieldName()[0];
        bool isPrimaryRight = schemaRight->getPrimaryKey() == joins[0].getFieldName()[1];

        if(joins[0].joinType == ECE141::Keywords::left_kw){
            int size = aRowCollections.list.size();  int cnt = 0;
            for(auto i  = aRowCollections.list.begin(); i != aRowCollections.list.end() && cnt < size;){
                bool match = false;    Row tempRow = *i;
                aRowCollections.list.erase(i);
                for(Row& j : aRowCollections1.list){
                    if(matchJoinCondition(tempRow,j)){
                        addTwoRows(tempRow,j,aRowCollections);
                        match = true;
                        if(isPrimaryLeft && isPrimaryRight) break;
                    }
                }
                if(!match){
                    addEmptyAttributes(*i, schemaRight->getAttributes(), schemaRight->getPrimaryKey(),aRowCollections);
                }
                cnt++;
            }
        }
        else if(joins[0].joinType == ECE141::Keywords::right_kw){
            int size = aRowCollections.list.size();
            for(auto i : aRowCollections1.list){
                bool match = false;  int cnt = 0;
                for(auto j  = aRowCollections.list.begin(); j != aRowCollections.list.end() && cnt < size; j++, cnt++){
                    if(matchJoinCondition(i,*j)){
                        addTwoRows(i,*j,aRowCollections);
                        match = true;
                        if(isPrimaryRight && isPrimaryLeft) break;
                    }
                }
                if(!match){
                    addEmptyAttributes(i, schemaLeft->getAttributes(), schemaLeft->getPrimaryKey(),aRowCollections);
                }
            }
            //delete all origin rows in RowCollection
            int cnt = 0;
            for(auto i  = aRowCollections.list.begin(); i != aRowCollections.list.end() && cnt < size; cnt++){
                aRowCollections.list.erase(i);
            }

        }
        else{  //normal join
            for(auto i  = aRowCollections.list.begin(); i != aRowCollections.list.end();){
                bool match = false;
                for(Row& j : aRowCollections1.list){
                    if(matchJoinCondition(*i,j)){
                        addTwoRows(*i,j,aRowCollections);
                        match = true;
                        break;
                    }
                }
                if(!match) aRowCollections.list.erase(i);
                else i++;
            }
        }

        return StatusResult();
    }


    StatusResult  UpdateStatement::run(std::ostream &aStream) const{
        //check table name and database
        if(database == nullptr) return StatusResult(Errors::noDatabaseSpecified);
        if(!database->hasTableName(name)) return StatusResult(Errors::unknownTable);

        //initialize a validator
        Validator s(database->getMap());

        Timer aTimer;
        aTimer.start();
        RowColloection aRowCollections =  getRowColloections(database, name, filters);

        //check set name   and   where name
        for(auto it : filters.getExpression())              if(!s.checkWhereAndOrder(name,it->lhs.name, "")) return StatusResult(Errors::unknownAttribute);
        for(auto it : filters.updateValues)              if(!s.checkWhereAndOrder(name,it.first, "")) return StatusResult(Errors::unknownAttribute);

        if(filters.updateValues.size() != 0){   //need update
            for(auto it = aRowCollections.list.begin(); it != aRowCollections.list.end(); ){
                if(!filters.match1(*it)){
                    //if not, delete it from rowCollections
                    aRowCollections.list.erase(it);
                }
                else it++;
            }
        }
        //need store
        if(aRowCollections.list.size() != 0){
            for(Row i : aRowCollections.list){
                database->saveData(name,i.headerId,i,i.blockId);   //store it into the previous index
            }
        }
        aTimer.stop();
        double time = aTimer.elapsed();
        cout<< aRowCollections.list.size() <<" rows affected (" << time << " ms.)\n";
        return StatusResult();
    }


    StatusResult  AlterStatement::run(std::ostream &aStream) const{
        //change the toc
        if(database == nullptr) return StatusResult(Errors::noDatabaseSpecified);
        if(!database->hasTableName(name)) return StatusResult(Errors::unknownTable);

        Schema* s = database->getMap()[name];
        s->addAttribute(attribute);
        database->saveSchema(*s);

        //load all the data and change it
        RowColloection aRowCollections =  getRowColloections(database, name, filters);
        for(Row i : aRowCollections.list){
            //添加这个KeyValuePair， string是null
            database->addNULLToRow(i,attribute);
            //save
            database->saveData(name,i.headerId,i,i.blockId);
        }
        return StatusResult();
    }


    //parse//////////////


    StatusResult  CreateTableStatement::parse(Tokenizer &aTokenizer){
        aTokenizer.next();  //jump the punc (

        schema = new Schema(name);
        schema->setId(1);
        bool length = false;
        bool IsDefault = false;
        //seperate diff attributes
        Attribute* pointer = nullptr;
        while(aTokenizer.current().data != ")"){
            while(aTokenizer.current().data != ","){
                if(aTokenizer.current().type == TokenType::identifier && pointer == nullptr){
                    Attribute* temp = new Attribute(aTokenizer.current().data);
                    pointer = temp;
                }
                else if(aTokenizer.current().keyword == Keywords::integer_kw || aTokenizer.current().data == "TIMESTAMP" ||
                        aTokenizer.current().keyword == Keywords::float_kw || aTokenizer.current().keyword == Keywords::varchar_kw ||
                        aTokenizer.current().keyword == Keywords::boolean_kw){
                    switch(aTokenizer.current().keyword){
                        case Keywords::integer_kw:
                            pointer->setType(DataType::int_type);  break;
                        case Keywords::float_kw:
                            pointer->setType(DataType::float_type);   break;
                        case Keywords::varchar_kw:
                            pointer->setType(DataType::varchar_type); break;
                        case Keywords::boolean_kw:
                            pointer->setType(DataType::bool_type); break;
                        default:
                            pointer->setType(DataType::datetime_type); break;
                    }
                }
                    //set auto_increment
                else if(aTokenizer.current().keyword == Keywords::auto_increment_kw) pointer->setAutoIncrement(true);
                    //set key
                else if(aTokenizer.current().keyword == Keywords::key_kw) pointer->setKey(true);
                    //set primary
                else if(aTokenizer.current().keyword == Keywords::primary_kw) pointer->setPrimary(true);
                    // var char length
                else if(aTokenizer.current().type == TokenType::number){
                    if(length){
                        std::stringstream ss;
                        ss << aTokenizer.current().data;
                        int x;  ss >> x;
                        pointer->setLength(x);
                        length = false;
                    }
                    else if(IsDefault){
                        pointer->setdefault(std::string(aTokenizer.current().data));
                        IsDefault = false;
                    }
                }
                    //
                else if(aTokenizer.current().keyword == Keywords::not_kw)  pointer->setNullable(true);

                    //(100) and 0.0
                else if(aTokenizer.current().data == "("){
                    length = true;
                }
                else if(aTokenizer.current().data == "DEFAULT"){
                    IsDefault = true;
                }
                else if(aTokenizer.current().data == "FALSE" || aTokenizer.current().data == "TRUE"){
                    if(IsDefault){
                        pointer->setdefault(std::string(aTokenizer.current().data));
                        IsDefault = false;
                    }
                }
                //go to next token
                if(!aTokenizer.next()){
                    schema->addAttribute(*pointer);   // add in the schema
                    pointer = nullptr;
                    return StatusResult();
                }
            }
            schema->addAttribute(*pointer);   // add in the schema
            pointer = nullptr;
            if(!aTokenizer.next()) return StatusResult(); //jump the  ,
        }
        return StatusResult();
    }

    StatusResult InsertTableStatement::parse(Tokenizer &aTokenizer){
        //create the rowList by the token, it miss a syntax error, return error
        //insert into users (id, name) values (9, 'foo'), (12, 'bar')
        bool value = false;    stringstream ss;
        while(aTokenizer.current().data != ";" && aTokenizer.more()){
            if(!value){
                if(aTokenizer.current().keyword == Keywords::values_kw){
                    value = true;
                    aTokenizer.next(); continue;
                }
                else if(aTokenizer.current().type == TokenType::identifier){  // properties
                    properties.push_back(aTokenizer.current().data);
                }
            }
            else{  //now go into record data
                Row* temp = new Row(); int index = 0;
                while(aTokenizer.current().data != ")"){   //separate by )
                    if(aTokenizer.current().type == TokenType::number){    //it maybe id
                        ss << aTokenizer.current().data;   unsigned int num = 0;  ss >> num;
                        ss.clear();
                        ValueType* x = new ValueType(); *x = num;
                        string tempName = properties[index];
                        KeyValuePair* y = new KeyValuePair(make_pair(properties[index++],*x));
                        temp->getPair().push_back(*y);
                        temp->pair2[tempName] = *x;
                    }
                    else if(aTokenizer.current().type == TokenType::identifier){
                        //bool and string
                        if(aTokenizer.current().data == "TRUE" || aTokenizer.current().data == "FALSE"){
                            bool tempBool = aTokenizer.current().data == "TRUE" ? true : false;
                            ValueType* x = new ValueType();  *x = tempBool;
                            string tempName = properties[index];
                            KeyValuePair* y = new KeyValuePair(make_pair(properties[index++],*x));
                            temp->getPair().push_back(*y);
                            temp->pair2[tempName] = *x;
                        }
                        else{
                            ValueType* x = new ValueType();  *x = aTokenizer.current().data;
                            string tempName = properties[index];
                            KeyValuePair* y = new KeyValuePair(make_pair(properties[index++],*x));
                            temp->getPair().push_back(*y);
                            temp->pair2[tempName] = *x;
                        }
                    }
                    if(!aTokenizer.next()) return StatusResult(Errors::syntaxError);
                }
                rowList.push_back(temp);
            }
            aTokenizer.next();
        }
        return StatusResult();
    }

    StatusResult SelectStatement::parse(Tokenizer &aTokenizer){
        //here, the aToken is at the first element
        vector<string> showName;
        bool from = false;
        while(aTokenizer.current().data != ";" && aTokenizer.more()){
            if(!from){
                if(aTokenizer.current().data == "*"){
                    tableView.setIsStar(true);
                }
                else if(aTokenizer.current().keyword == Keywords::from_kw){
                    from = true;
                    aTokenizer.next();
                    if(aTokenizer.current().type != TokenType::identifier) return StatusResult(Errors::syntaxError);
                    else   name = aTokenizer.current().data; //table name
                    //add showname
                    tableView.addShowName(showName);

                    //now let's check if we face a join
                    if(!aTokenizer.more1() || aTokenizer.current().data == ";"){
                        aTokenizer.next();
                        return StatusResult();
                    }
                    parseJoin(aTokenizer);
                }
                //show name
                else if(aTokenizer.current().type == TokenType::identifier){
                    showName.push_back(aTokenizer.current().data);
                }
            }
            else{
                if(aTokenizer.current().keyword == Keywords::where_kw){
                    aTokenizer.next();
                    while(aTokenizer.more() && aTokenizer.current().keyword != Keywords::order_kw && aTokenizer.current().data != "LIMIT"){
                        if(aTokenizer.current().type == TokenType::identifier){
                            Operand* operand1 = new Operand(); operand1->name = aTokenizer.current().data;
                            aTokenizer.next();
                            Operators operators = operatorChange[aTokenizer.current().data];
                            aTokenizer.next();
                            Operand* operand2 = new Operand(); operand2->name = aTokenizer.current().data;
                            Expression* expression = new Expression(*operand1,operators,*operand2);
                            filters.add(expression);
                        }
                        aTokenizer.next();

                    }
                }
                if(aTokenizer.current().keyword == Keywords::order_kw){
                    filters.order = true;
                    while(aTokenizer.more() && aTokenizer.current().data != "LIMIT"){
                        //除了没了 或者 limit 否则 继续
                        if(aTokenizer.current().type == TokenType::identifier){  // 按什么排顺序
                            filters.orderName = aTokenizer.current().data;
                            filters.sorter.field = filters.orderName;  //终于定义好了
                        }
                        aTokenizer.next();
                    }
                    
                }
                if(aTokenizer.current().data == "LIMIT"){
                    filters.limit = true;
                    std::stringstream ss;
                    aTokenizer.next();
                    ss << aTokenizer.current().data;
                    ss >> filters.limitNumber;
                }
            }
            aTokenizer.next();
        }
        return StatusResult();
    }


    StatusResult UpdateStatement::parse(Tokenizer &aTokenizer){

        if(aTokenizer.current().keyword != Keywords::update_kw) return StatusResult(Errors::syntaxError);
        aTokenizer.next();
        name = aTokenizer.current().data;  //record table name
        aTokenizer.next();
        if(aTokenizer.current().keyword != Keywords::set_kw) return StatusResult(Errors::syntaxError);
        aTokenizer.next();
        while(aTokenizer.current().keyword != Keywords::where_kw){

            string name = aTokenizer.current().data;   //name
            if(!aTokenizer.next()) return StatusResult(Errors::syntaxError);  //next
            if(aTokenizer.current().data != "=") return StatusResult(Errors::syntaxError);  // =
            if(!aTokenizer.next()) return StatusResult(Errors::syntaxError);   //next
            filters.updateValues[name] = aTokenizer.current().data;
            if(!aTokenizer.next()) return StatusResult(Errors::syntaxError);  //next
        }
        aTokenizer.next();  //jump where
        while(aTokenizer.current().data != ";" && aTokenizer.more()){
            if(aTokenizer.current().type == TokenType::identifier){
                Operand* operand1 = new Operand(); operand1->name = aTokenizer.current().data;
                aTokenizer.next();
                Operators operators = operatorChange[aTokenizer.current().data];
                aTokenizer.next();
                Operand* operand2 = new Operand(); operand2->name = aTokenizer.current().data;
                Expression* expression = new Expression(*operand1,operators,*operand2);
                filters.add(expression);
            }
            else if(aTokenizer.current().keyword == Keywords::and_kw)  filters.And = true;
            else if(aTokenizer.current().keyword == Keywords::or_kw)  filters.Or = true;
            if(!aTokenizer.next()) return StatusResult(Errors::syntaxError);
        }
        return StatusResult();
    }

    StatusResult ShowIndexStatement::parse(Tokenizer &aTokenizer){
        if(aTokenizer.current().keyword != Keywords::show_kw) return StatusResult(Errors::syntaxError);
        while(aTokenizer.current().data != ";" && aTokenizer.more()){
            aTokenizer.next();
        }
        return StatusResult();
    }

    StatusResult AlterStatement::parse(Tokenizer &aTokenizer){
        if(aTokenizer.current().keyword != Keywords::alter_kw) return StatusResult(Errors::syntaxError);
        aTokenizer.next(2);
        name = aTokenizer.current().data;
        aTokenizer.next();
        //onlu for add right now
        if(aTokenizer.current().keyword != Keywords::add_kw) return StatusResult(Errors::syntaxError);
        aTokenizer.next();
        attribute.setName(aTokenizer.current().data);
        aTokenizer.next();
        //type
        switch(aTokenizer.current().keyword){
            case Keywords::integer_kw:
                attribute.setType(DataType::int_type);  break;
            case Keywords::float_kw:
                attribute.setType(DataType::float_type);   break;
            case Keywords::varchar_kw:
                attribute.setType(DataType::varchar_type); break;
            case Keywords::boolean_kw:
                attribute.setType(DataType::bool_type); break;
            default:
                attribute.setType(DataType::datetime_type); break;
        }
        aTokenizer.next();
        while(aTokenizer.current().data != ";" && aTokenizer.more()){
            //数字
            if(aTokenizer.current().type == TokenType::number){
                //length
                stringstream ss;
                int num = 0;
                ss << aTokenizer.current().data;  ss >> num;  ss.clear();
                attribute.setLength(num);
            }
            else if(aTokenizer.current().keyword == Keywords::auto_increment_kw){
                attribute.setAutoIncrement(true);
            }
            aTokenizer.next();
        }
        return StatusResult();
    }

    StatusResult SelectStatement::parseTableName(Tokenizer& aTokenizer, string& theTable){
        if(aTokenizer.current().type == TokenType::identifier){
            theTable = aTokenizer.current().data;
            aTokenizer.next();
        }
        return StatusResult();
    }

    StatusResult SelectStatement::parseTableField(Tokenizer& aTokenizer, TableField& theField){
        if(aTokenizer.current().type == TokenType::identifier){
            theField = aTokenizer.current().data;
            aTokenizer.next();
            if(aTokenizer.current().data == "."){
                theField += ".";
                aTokenizer.next();
            }
            else return StatusResult(Errors::syntaxError);
            if(aTokenizer.current().type == TokenType::identifier){
               theField += aTokenizer.current().data;
               aTokenizer.next();
            }
            else return StatusResult(Errors::syntaxError);
        }
        return StatusResult();
    }


    StatusResult SelectStatement::parseJoin(Tokenizer &aTokenizer) {

        Keywords theKeyWord = aTokenizer.peek(1).keyword;
        StatusResult theResult{joinTypeExpected}; //add joinTypeExpected to your errors file if missing...

        Keywords theJoinType{Keywords::join_kw}; //could just be a plain join
        if(in_array<Keywords>(gJoinTypes, theKeyWord)) {
            theJoinType=theKeyWord;
            aTokenizer.next(2); //yank the 'join-type' token (e.g. left, right)
            if(aTokenizer.skipIf(Keywords::join_kw)) {
                std::string theTable;
                if((theResult=parseTableName(aTokenizer, theTable))) {
                    Join theJoin(theTable, theJoinType, std::string(""),std::string(""));
                    theResult.code=keywordExpected; //on...
                    if(aTokenizer.skipIf(Keywords::on_kw)) { //LHS field = RHS field
                        TableField LHS("");
                        if((theResult=parseTableField(aTokenizer, theJoin.lhs))) {
                            if(aTokenizer.current().data == "=") {
                                aTokenizer.next();
                                if((theResult=parseTableField(aTokenizer, theJoin.rhs))) {
                                    joins.push_back(theJoin);
                                }
                            }
                        }
                    }
                }
            }
        }
        return theResult;
    }


    StatusResult SelectStatement::addEmptyAttributes(Row& aRow1, const AttributeList & aList, string primaryName, RowColloection& aRowCollections) const{
        Row* tempRow = new Row();
        //add all in the Row1
        for(auto it : aRow1.getPair()){
            tempRow->getPair().push_back(it);
            tempRow->pair2[it.first] = it.second;
        }
        //add all empty in Row2
        for(Attribute i : aList){
            if(i.getName() == primaryName) continue;
            database->addNULLToRow(*tempRow, i);
        }
        aRowCollections.addRow(*tempRow);
        return StatusResult();
    }

    bool         SelectStatement::matchJoinCondition(Row& aRow1, Row& aRow2) const{
        string* fields = joins[0].getFieldName();
        string fieldLeft = fields[0];
        string fieldRight = fields[1];
        return aRow1.pair2[fieldLeft] == aRow2.pair2[fieldRight];
    }
    StatusResult SelectStatement::addTwoRows(Row& aRow1, Row& aRow2, RowColloection& aRowCollections) const{
        Row* tempRow = new Row();

        for(KeyValuePair i : aRow1.getPair()){
            tempRow->getPair().push_back(i);
            tempRow->pair2[i.first] = i.second;
        }
        string* fields = joins[0].getFieldName();
        string fieldRight = fields[1];

        for(KeyValuePair i : aRow2.getPair()){
            if(i.first != fieldRight){
                tempRow->getPair().push_back(i);
                tempRow->pair2[i.first] = i.second;
            }
        }
        aRowCollections.addRow(*tempRow);
        return StatusResult();
    }

    StatusResult SelectStatement::addRightRow(RowColloection& aRowCollections, Row& aRow2, const AttributeList & aList, string primaryName) const{
        Row* tempRow = new Row();
        for(Attribute i : aList){
            uint32_t temp = 0;
            float temp1 = 0.0;
            string temp2 = "";
            ValueType* x = new ValueType();
            switch (i.getType()){
                case DataType::int_type:
                    *x = temp;
                    break;
                case DataType::float_type:
                    *x = temp1;
                    break;
                case DataType::bool_type:
                    *x = false;
                    break;
                case DataType::varchar_type:
                    *x = temp2;
                    break;
                default:
                    break;
            }
            KeyValuePair* pair = new KeyValuePair(make_pair(i.getName(),*x));
            tempRow->getPair().push_back(*pair);
            tempRow->pair2[i.getName()] = *x;

        }

        for(auto it : aRow2.getPair()){
            string name = it.first;
            if(name == primaryName) continue;
            tempRow->getPair().push_back(it);
            tempRow->pair2[name] = it.second;
        }

        aRowCollections.addRow(*tempRow);
        return StatusResult();
    }

    bool   SelectStatement::checkJoinName() const{
        string* table = joins[0].getTableName();
        string tableLeft = table[0];
        string tableRight = table[1];

        string* fields = joins[0].getFieldName();
        string fieldLeft = fields[0];
        string fieldRight = fields[1];

        if(name != tableLeft) return false;
        if(joins[0].table != tableRight) return false;

        if(!database->getMap()[name]->hasAttributes(fieldLeft)) return false;
        if(!database->getMap()[joins[0].table]->hasAttributes(fieldRight)) return false;

        return true;
    }


}
