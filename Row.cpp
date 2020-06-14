//
//  Row.cpp
//  Assignment4
//
//  Created by rick gessner on 4/19/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Row.hpp"


namespace ECE141 {
  
  //STUDENT: You need to fully implement these methods...

  Row::Row(){}

  Row::Row(const Row &aRow) {
      pair = aRow.pair;
      pair2 = map(aRow.pair2);
      blockId = aRow.blockId;
      headerId = aRow.headerId;
  }

  Row& Row::operator=(const Row &aRow) {
      pair = aRow.pair;
      pair2 = map(aRow.pair2);
      blockId = aRow.blockId;
      headerId = aRow.headerId;
      return *this;
  }

  bool Row::operator==(Row &aCopy) const {
      return false;
  }


  Row::~Row() {}





}
