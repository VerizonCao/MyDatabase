//
//  Storage.cpp
//  Assignment2
//
//  Created by rick gessner on 4/5/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Storage.hpp"
#include <fstream>
#include <exception>
#include <stdlib.h>

namespace ECE141 {

  // USE: Our main class for managing storage...
  const char* StorageInfo::getDefaultStoragePath() {
      const char* thePath = std::getenv("DB_PATH");
      return thePath;
  }

  //----------------------------------------------------------

  //path to the folder where you want to store your DB's...
  std::string getDatabasePath(const std::string &aDBName) {
    std::string thePath;
    //build a full path (in default storage location) to a given db file..
    std::string temp = StorageInfo::getDefaultStoragePath();
    thePath += ( temp + "/"  + aDBName + ".db");
    return thePath;
  }

  // USE: ctor ---------------------------------------
  Storage::Storage(const std::string aName, CreateNewStorage) : name(aName),stream() {
    std::string thePath = getDatabasePath(name);
    try {
        stream.open(thePath,std::ios::out | std::ios::binary);
        stream.close();
        stream.open(thePath,std::ios::in | std::ios::out | std::ios::binary);

    }catch (std::exception e){
        std::cout << e.what() <<std::endl;
    }
  }

  // USE: ctor ---------------------------------------
  Storage::Storage(const std::string aName, OpenExistingStorage) : name(aName),stream() {
    std::string thePath = getDatabasePath(aName);
    stream.open(thePath,std::ios::in | std::ios::out | std::ios::binary);   // because it not, then error
    if(!stream.is_open()){
        std::cout << "can not find or open  this file\n";
    }
  }

  // USE: dtor ---------------------------------------
  Storage::~Storage() {
    stream.close();
  }

  // USE: validate we're open and ready ---------------------------------------
  bool Storage::isReady() const {
    return stream.is_open();
  }

  // USE: count blocks in file ---------------------------------------
  uint32_t Storage::getTotalBlockCount() {
    //how can we compute the total number of blocks in storage?
    return 0;

  }

    // USE: for use with "storable API" [NOT REQUIRED -- but very useful]

  StatusResult Storage::save(Storable &aStorable) {
    //High-level IO: save a storable object (like a table row)...
     return StatusResult{noError};
  }

// USE: for use with "storable API" [NOT REQUIRED -- but very useful]

  StatusResult Storage::load(Storable &aStorable) {
    //high-level IO: load a storable object (like a table row)
    stream.seekg(1);
    return StatusResult{noError};
  }

  // USE: write data a given block (after seek)
  StatusResult Storage::writeBlock(StorageBlock &aBlock, uint32_t aBlockNumber) {
    //STUDENT: Implement this; this is your low-level block IO...

    char insert[1024];  memset(insert,0,sizeof(insert));

    memcpy(insert,(char*)&aBlockNumber,4);
    memcpy(insert+4,(char*)&aBlock.header.type, sizeof(aBlock.header.type));
    memcpy(insert+8,(char*)&aBlock.header.id,4);
    memcpy(insert+12,aBlock.data, data_size);

    int start = aBlockNumber*1024;
    stream.seekg(start, std::ios::beg);

    if(stream.write(insert, sizeof(insert))){
        stream.flush();
        return StatusResult{};
    }
    else{
        return StatusResult(writeError);
    }
  }

  // USE: read data from a given block (after seek)
  StatusResult Storage::readBlock(StorageBlock &aBlock, uint32_t aBlockNumber) {
    //STUDENT: Implement this; this is your low-level block IO...
    //直接找到并且读出来，因为没有返回值



    int start = aBlockNumber*1024;

    stream.seekg(start + 4, std::ios::beg);

    char type;
    stream.read(&type, sizeof(char));
    aBlock.header.type = type;

    stream.seekg(start + 8, std::ios::beg);
    int id;
    stream.read((char*)&id, sizeof(int));
    aBlock.header.id = id;

    stream.seekg(start + 12, std::ios::beg);
    stream.read(aBlock.data, data_size);



    return StatusResult{};
  }

  StatusResult    Storage::writeBlank(uint32_t aBlockNumber){


      char insert[1024];   memset(insert,0,sizeof(insert));

      stream.seekg(aBlockNumber*1024,std::ios::beg);

      if(stream.write(insert, sizeof(insert))){
          stream.close();
          stream.open(getDatabasePath(name),std::ios::in | std::ios::out | std::ios::binary);
          return StatusResult{};
      }
      else{
          return StatusResult(writeError);
      }

  }




}


