//
//  Storage.hpp
//  Assignment2
//
//  Created by rick gessner on 4/5/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Storage_hpp
#define Storage_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <fstream>
#include <variant>
#include "Other/Errors.hpp"
#include "StorageBlock.hpp"
#include "Folders/FolderReader.hpp"
#include <vector>
#include "Other/Value.hpp"


namespace ECE141 {


  class StorageInfo {
  public:
    static const char* getDefaultStoragePath();
  };
  
  struct CreateNewStorage {};
  struct OpenExistingStorage {};



  // USE: Our storage manager class...
  class Storage {
  public:
        
    Storage(const std::string aName, CreateNewStorage);
    Storage(const std::string aName, OpenExistingStorage);
    ~Storage();        
    uint32_t        getTotalBlockCount();

        //high-level IO (you're not required to use this, but it may help)...    
    StatusResult    save(Storable &aStorable); //using a stream api
    StatusResult    load(Storable &aStorable); //using a stream api
    
        //low-level IO...
    StatusResult    readBlock(StorageBlock &aBlock, uint32_t aBlockNumber);
    StatusResult    writeBlock(StorageBlock &aBlock, uint32_t aBlockNumber);

    StatusResult    writeBlank(uint32_t aBlockNumber);
    StatusResult    findFreeBlockNum(){return StatusResult();}



  protected:

    bool            isReady() const;
    std::string     name;
    std::fstream    stream;    
  };

}

#endif /* Storage_hpp */
