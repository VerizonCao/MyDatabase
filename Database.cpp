//
//  Created by rick gessner on 4/12/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.

#include <sstream>
#include "Database.hpp"
#include "Interface/View.hpp"
#include "Storage.hpp"
#include "Other/Value.hpp"



namespace ECE141 {
    
  Database::Database(const std::string aName, CreateNewStorage)
    : name(aName), storage(aName, CreateNewStorage{}) {

      StorageBlock ablock(BlockType::meta_block);
      int size = 0;
      memcpy(ablock.data,(char*)&size,4);
      ablock.header.id = 1;   //file and index meta
      storage.writeBlock(ablock, 0);

  }
  
  Database::Database(const std::string aName, OpenExistingStorage)
    : name(aName), storage(aName, OpenExistingStorage{}) {
      loadMetaInfo();
      loadSchemas();
      loadIndexTable();   //load index table into index manager
  }
  
  Database::~Database() {

  }

  bool Database::hasTableName(string aName){
      for(auto it : toc.list){
          if(it.first == aName) return true;
      }
      return false;
  }

  StatusResult Database::loadMetaInfo(){
      StorageBlock metaBlock(BlockType::meta_block);
      storage.readBlock(metaBlock,0);  //get thee meta block
      //load id
      setId(metaBlock.header.id);
      //if no tables, quit
      int size = 0;  memcpy((char*)&size, metaBlock.data, sizeof(int));
      if(size == 0) return StatusResult();

      char current[1008]; memset(current,0,sizeof(current));
      memcpy(current,metaBlock.data+4, sizeof(metaBlock.data)-4);
      string getData = current;

      //size
      int start = 0;  int index;
      for(int i = 0; i<getData.size(); i++){
          if(getData[i] == ';') break;
          //************
          if(getData[i] == ','){
              stringstream ss;
              std::string temp = getData.substr(start,i-start);
              int colon = temp.find(":");
              string theName = temp.substr(0,colon);
              ss << temp.substr(colon + 1 , i-1);   ss >> index;
              //write into toc
              toc.list[theName] = index;
              start = i + 1;

          }
      }
      return StatusResult();
  }

  StatusResult Database::loadSchemas(){
      //load each table
      for(auto it : toc.list){
          StorageBlock tableBlock(BlockType::entity_block);
          storage.readBlock(tableBlock,it.second);
          Schema* tempSchema = new Schema(it.first);
          //**************************************  load all the info
          //id
          tempSchema->setId(tableBlock.header.id);
          //attributes size
          int size = 0;
          memcpy((char*)&size,tableBlock.data,4);

          for(int i = 0; i<size; i++){
              char temp[40];    memset(temp,0,sizeof(temp));
              int index = 112+40*i;
              memcpy(temp,tableBlock.data+index,40);
              //name
              char tempname[20];  memcpy(tempname,temp+1,20);  string name(tempname);  Attribute thisAttribute(name);
              //type
              DataType type;  memcpy((char*)&type,temp+21,1);  thisAttribute.setType(type);
              //null
              bool nullValue;   memcpy((char*)&nullValue,temp+22,1); thisAttribute.setNullable(nullValue);
              //key
              bool key;   memcpy((char*)&key,temp+23,1);  thisAttribute.setKey(key);
              //default
              char Default1[10];  memset(Default1,0, sizeof(Default1));   memcpy(Default1,temp+24,10);
              string Default(Default1); if(Default == "") Default = "NULL";
              thisAttribute.setdefault(Default);
              //extra
              std::string extraShow;
              bool auto_in;   memcpy((char*)&auto_in,temp+34,1); thisAttribute.setAutoIncrement(auto_in);
              bool primary;   memcpy((char*)&primary,temp+35,1); thisAttribute.setPrimary(primary);

              tempSchema->addAttribute(thisAttribute);
          }

          schemas[it.first] = tempSchema;
      }
      return StatusResult();
  }

  StatusResult Database::saveMetaInfo(){
      int size = toc.list.size();

      StorageBlock metaBlock(BlockType::meta_block);
      //set id
      metaBlock.header.id = getId();
      strcpy(metaBlock.data,(char*)&size);

      string tables;
      for(auto it : toc.list){
          stringstream ss;
          string place;  ss << it.second;  ss >> place;
          tables += it.first + ":" + place + ",";
      }
      tables += ";";
      memcpy(metaBlock.data + 4,tables.c_str(), tables.size());
      if(storage.writeBlock(metaBlock,0)){
          return StatusResult();
      }
      else return StatusResult(Errors::unknownError);
  }

  StatusResult Database::saveSchema(Schema& aSchema){
      string theName = aSchema.getName();
      int blockNumber = toc.list[theName];
      StorageBlock aBlock(BlockType::entity_block);
      aBlock.header.id = schemas[theName]->getId(); //set it to be id

      char tableinfo[112];   memset(tableinfo,0,sizeof(tableinfo));
      char attributesinfo[900];  memset(attributesinfo,0,sizeof(attributesinfo));

      //each attribute   unit char
      //name 20  type 1  null 1  key 1  default 10  extra 5  { }    total 40
      int size = aSchema.getAttributes().size();    memcpy(tableinfo,(char*)&size,4);
      std::string attributes;   //得到每个attributes的名字

      int index = 0;
      for(Attribute temp : aSchema.getAttributes()){
          attributes += (temp.getName() + ",");

          char attribute[40];  memset(attribute,0,sizeof(attribute));

          attribute[0] = '{';
          //name
          memcpy(attribute+1,temp.getName().c_str(), sizeof(aSchema.getName()));

          //type
          char x1 = (char)temp.getType();
          memcpy(attribute+21,&x1, sizeof(x1));

          //null
          bool x2 = temp.getNullable();
          memcpy(attribute+22,(char*)&x2, sizeof(x2));

          //key
          bool x3 = temp.getKey();
          memcpy(attribute+23,(char*)&x3, sizeof(x3));

          //default
          std::string x4 = temp.getDefault();
          memcpy(attribute+24,x4.c_str(), sizeof(x4));

          //extra
          // auto_increment and primary key
          bool x5 = temp.getAutoIncrement();
          memcpy(attribute+34,(char*)&x5, sizeof(x5));

          bool x6  = temp.getPrimary();
          memcpy(attribute+35,(char*)&x6, sizeof(x6));

          attribute[39] = '}';
          memcpy((attributesinfo + index*40),&attribute, sizeof(attribute));
          index++;
      }

      attributes = attributes.substr(0,attributes.size()-1) + ";";  //delete last, and add ;
      memcpy(tableinfo+4,attributes.c_str(), sizeof(attributes));

      memcpy(aBlock.data,tableinfo, sizeof(tableinfo));
      memcpy(aBlock.data + 112,attributesinfo, sizeof(attributesinfo));

      if(storage.writeBlock(aBlock,blockNumber)){
          return StatusResult();
      }
      else return StatusResult(Errors::unknownError);

  }


  StatusResult Database::showTables(){
      uint32_t cnt = 0;
      cout << "+----------------------+\n";
      cout << "| Tables_in_mydb       |\n";
      cout << "+----------------------+\n";
      cout.setf(ios::left);

      for (auto it : schemas) {
          cout << "|";
          cout.width(22);
          cout <<it.first << "|\n";
          cnt++;
      }
      cout << "+----------------------+\n";
      cout << cnt << " rows in set\n";
      return StatusResult();
  }


  StatusResult Database::describeTable(string aName){
      for(auto it : schemas){
          if(it.first == aName){
              std::cout<<"+-----------+--------------+------+-----+---------+-----------------------------+\n";
              std::cout<<"| Field     | Type         | Null | Key | Default | Extra                       |\n";
              std::cout<<"+-----------+--------------+------+-----+---------+-----------------------------+\n";
              //show
              Schema* temp = schemas[it.first];
              //each attributes
              for(Attribute i : temp->getAttributes()){
                  string nullShow = i.getNullable() == true ? "NO" : "YES";
                  string key = i.getKey() == true ? "YES " : "";
                  string extraShow; extraShow += i.getAutoIncrement() == true ? "auto_increment " : "";
                  extraShow += i.getPrimary() == true ? "primary key " : "";

                  cout<<"| " << std::setw(10)<< std::setiosflags(std::ios::left)<< i.getName()
                      <<"| " << std::setw(13)<< std::setiosflags(std::ios::left)<< MapToType[i.getType()]
                      <<"| "<< std::setw(5) << std::setiosflags(std::ios::left)<< nullShow
                      <<"| " << std::setw(4)<< std::setiosflags(std::ios::left) << key
                      <<"| "<<std::setw(8)<< std::setiosflags(std::ios::left)<< i.getDefault()
                      <<"| " <<std::setw(27)<< std::setiosflags(std::ios::left) << extraShow  << " |" <<std::endl;
              }
              std::cout<<"+-----------+--------------+------+-----+---------+-----------------------------+\n";
              std::cout<< temp->getAttributes().size() <<" rows in set\n";
              return StatusResult();
          }

      }
      return StatusResult(Errors::unknownTable);
  }


  int Database::getTableId(const string aName){  //the id will increment by one automatically
      int id = schemas[aName]->getId();
      schemas[aName]->setId(id + 1);   //add one
      return id;  //still return the previous value
  }

  Database& Database::setTableId(int num, string aName){
      if(num == -1){
          int tableId = schemas[aName]->getId();
          schemas[aName]->setId(tableId -1);
      }
      else{
          schemas[aName]->setId(num);
      }
      return *this;
  }


  StatusResult Database::saveData(string theName, int id, Row& aRow, int blockNum){
      AttributeList list = schemas[theName]->getAttributes();
      StorageBlock aBlock(BlockType::data_block);
      aBlock.header.id = id;  //set data id
      stringstream ss;
      string append = theName + ";";

      int index1 = 0;   int index2 = 0;
      while(index1 < list.size() && index2 < aRow.getPair().size()){
          string name1 = list[index1].getName();
          string name2 = aRow.getPair()[index2].first;
          if(name1 == name2){
              append += name1 + ":";
              ValueType temp =  aRow.getPair()[index2].second;
              string tempShow = ""; bool x;
              switch (temp.index()){
                  case 0: ss << get<u_int32_t>(temp); ss >> tempShow;  ss.clear(); break;
                  case 1: ss << get<float>(temp);   ss >> tempShow;  break;
                  case 2: x = get<bool>(temp); tempShow = x == true ? "TRUE" : "FALSE"; break;
                  case 3: tempShow = get<string>(temp); break;
              }
              append += tempShow + ",";
              index1++;  index2++;
          }
          else{  //default or increment
              if(list[index1].getAutoIncrement()){  //increment
                  string number = "";
                  ss << id;
                  ss >> number;
                  ss.clear();

                  //add into row   convinient for update index
                  ValueType* x = new ValueType(); *x = (uint32_t)id;
                  KeyValuePair* y = new KeyValuePair(make_pair(name1,*x));
                  aRow.getPair().push_back(*y);
                  aRow.pair2[name1] = *x;

                  //add into append
                  append += name1 + ":" + number;
              }
              else{   //default
                  append += name1 + ":" + list[index1].getDefault();
              }
              append += ",";
              index1++;
          }
      }
      while(index1 < list.size()){
          append += list[index1].getName() + ":";
          append += list[index1].getDefault() + ",";
          index1++;
      }
      append += ";";
      memcpy(aBlock.data, append.c_str(), append.size());
      //if we add it in next block
      if(blockNum == -1){
          storage.writeBlock(aBlock,getId()+1);  //write data
          setId(getId()+1);   //update meta id
      }
      else{
          storage.writeBlock(aBlock,blockNum);
      }
      return StatusResult();
  }


  ValueType& Database::changeStringToDataType(string x, DataType aType){
      ValueType* temp = new ValueType();
      stringstream ss;
      bool y = false;
      __uint32_t num = 0;
      switch (aType){
          case DataType::varchar_type:
              *temp = x;
              return *temp;
          case DataType::int_type:
              ss << x; ss >> num;
              *temp = num;
              return *temp;
          case DataType::bool_type:
              y = x == "false" ? false : true;
              *temp = x;
              return *temp;
          default:
              return *temp;
      }

  }

  //index
  StatusResult Database::loadIndexTable(){ //load all the exist index in meta2
      StorageBlock temp;
      storage.readBlock(temp,1);  //load
      manager.loadInfo(temp);  //load into memory
      return StatusResult();
  }

  StatusResult Database::loadIndex(string tableName, string indexName){ //load the specific index for a table if exists
      if(manager.isIndexExisted(tableName,indexName)){
          if(manager.isIndexLoaded(tableName,indexName)) return StatusResult();
          else{
              int blockNum = manager.getIndexBlockNum(tableName,indexName);
              StorageBlock temp;
              storage.readBlock(temp, blockNum);
              manager.loadIndex(temp);
              return StatusResult();
          }
      }
      else return StatusResult(Errors::unknownIndex);
  }

  StatusResult  Database::saveIndexInfo(){
      StorageBlock temp = *manager.saveInfo();
      getStorage().writeBlock(temp,1);
      return StatusResult();
  }

  StatusResult  Database::saveIndex(string tableName, string indexName){
      StorageBlock* aBlock = manager.saveIndex(tableName, indexName);
      if(aBlock == nullptr) return StatusResult(Errors::unknownIndex);
      int blockNum = manager.getToc()[tableName][indexName];   //get the place from the Toc
      storage.writeBlock(*aBlock, blockNum);
      setId(getId()+1);   //add a block into db.file leads to plus 1
      return StatusResult();
  }

  StatusResult  Database::saveAllIndex(){
      for(auto it : manager.getToc()){
          for(auto i : it.second){
              saveIndex(it.first, i.first);
          }
      }
      return StatusResult();
  }


  //alter
  StatusResult      Database::addNULLToRow(Row& aRow, const Attribute& attribute){
      uint32_t temp = 0;
      float temp1 = 0.0;
      string temp2 = "";
      ValueType* x = new ValueType();
      switch (attribute.getType()){
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
      KeyValuePair* pair = new KeyValuePair(make_pair(attribute.getName(),*x));
      aRow.getPair().push_back(*pair);
      aRow.pair2[attribute.getName()] = *x;
      return StatusResult();
  }
}

