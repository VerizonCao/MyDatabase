//
//  StorageBlock.hpp
//  Assignment3
//
//  Created by rick gessner on 4/11/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef StorageBlock_hpp
#define StorageBlock_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <variant>
#include <iostream>
#include <string.h>

namespace ECE141 {

  //a "storage" file is comprised of fixed-sized blocks (defined below)

  const size_t kPayloadSize = 1012; //area reserved in storage-block for user data...


  enum class BlockType {
    meta_block='T',
    data_block='D',
    entity_block='E',
    free_block='F',
    index_block='I',
    unknown_block='V',
  };

  using NamedBlockNums = std::map<std::string, uint32_t>;

  struct BlockHeader {
   
    BlockHeader(BlockType aType=BlockType::data_block)
      : type(static_cast<char>(aType)), id(0) {}
   
    BlockHeader(const BlockHeader &aCopy) {
      *this=aCopy;
    }
   
    BlockHeader& operator=(const BlockHeader &aCopy) {
      type=aCopy.type;
      id=aCopy.id;
      return *this;
    }
    // type 和 id
    char      type;     //char version of block type {[D]ata, [F]ree... }
    uint32_t  id;       //use this anyway you like
  };
 
  struct StorageBlock {
        
    StorageBlock(BlockType aType=BlockType::data_block);
    
    StorageBlock(const StorageBlock &aCopy);
    StorageBlock& operator=(const StorageBlock &aCopy);

    StorageBlock& store(std::ostream &aStream);

    //we use attributes[0] as table name...        header 和 数据
    BlockHeader   header;
    char          data[kPayloadSize];
  };
 
}

#endif /* StorageBlock_hpp */
