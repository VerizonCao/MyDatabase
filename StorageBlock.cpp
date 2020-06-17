//  Created by rick gessner on 4/11/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.

#include "StorageBlock.hpp"

namespace ECE141 {

  StorageBlock::StorageBlock(BlockType aType) : header(aType) {
    memset(data,0,sizeof(data));
  }

  StorageBlock::StorageBlock(const StorageBlock &aCopy) : header(aCopy.header) {
    memcpy(reinterpret_cast<void*>(data), &aCopy.data, kPayloadSize);
  }

  StorageBlock& StorageBlock::operator=(const StorageBlock &aCopy) {
    header=aCopy.header;
    memcpy(reinterpret_cast<void*>(data), &aCopy.data, kPayloadSize);
    return *this;
  }
  
  StorageBlock& StorageBlock::store(std::ostream &aStream) {
    return *this;
  }


}
