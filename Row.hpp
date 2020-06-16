//  Created by rick gessner on 4/19/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  A data line in database has many key-value-pair

#ifndef Row_hpp
#define Row_hpp

#include <stdio.h>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include "Other/Value.hpp"
#include "Attribute.hpp"

using namespace std;

namespace ECE141 {


  class Row{
  public:

    Row();
    Row(const Row &aRow);
    Row(const std::unique_ptr<Row>& aRow){
        pair = aRow->pair;
        pair2 = map(aRow->pair2);
        blockId = aRow->blockId;
        headerId = aRow->headerId;
    }
    ~Row();
    Row& operator=(const Row &aRow);
    bool operator==(Row &aCopy) const;



    vector<KeyValuePair>& getPair(){return pair;}
    const vector<KeyValuePair>& getPair() const {return pair;}
    KeyValues pair2;

    int blockId;
    int headerId;



  protected:
    vector<KeyValuePair> pair;
  };

}

#endif /* Row_hpp */
