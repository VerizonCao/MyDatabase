//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  used for each attribute in a table, the name type and isKey.....
//  for example:   name:  type: string, length: 25, primary: false....

#ifndef Attribute_hpp
#define Attribute_hpp

#include <stdio.h>
#include <string>
#include <variant>
#include "Other/Value.hpp"

namespace ECE141 {




  class Attribute {
  protected:
    std::string   name;
    DataType      type;


    int length;
    bool auto_increment;
    bool primary;
    bool key;
    bool nullable;
    std::string Adefault;

  public:
          
    Attribute(DataType aType=DataType::no_type);
    Attribute(std::string aName, DataType aType = DataType::no_type/*maybe more?*/);
    Attribute(const Attribute &aCopy);
    ~Attribute();
    
    Attribute&    setName(std::string &aName);
    Attribute&    setType(DataType aType);

    
    const std::string&  getName() const {return name;}
    DataType            getType() const {return type;}


    //getter
    int getLength();
    bool getAutoIncrement();
    bool getPrimary();
    bool getKey();
    bool getNullable();
    DataType getType();
    std::string getDefault();

    //setter
    Attribute&    setLength(int aLength);
    Attribute&    setAutoIncrement(bool type);
    Attribute&    setPrimary(bool type);
    Attribute&    setKey(bool tyoe);
    Attribute&    setNullable(bool type);
    Attribute&    setdefault(std::string aType);

  };
  

}


#endif /* Attribute_hpp */
