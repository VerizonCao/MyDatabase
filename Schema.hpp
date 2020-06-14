//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after skeleton

#ifndef Schema_hpp
#define Schema_hpp

#include <stdio.h>
#include <vector>
#include "Attribute.hpp"
#include "Other/Errors.hpp"
#include <optional>
#include <variant>

//#include "Row.hpp"

namespace ECE141 {
  
  struct Block;
  struct Expression;
  class  Database;
  class  Tokenizer;



  using AttributeList = std::vector<Attribute>;
  using AttributeOpt = std::optional<Attribute>;


  //STUDENT: If you're using the Storable interface, add that to Schema class?

  class Schema  {
  public:
                          Schema(const std::string aName);
                          Schema(const Schema &aCopy);
                          Schema(){};
    
                          ~Schema();
        
    const std::string&    getName() const {return name;}
    std::string&          getNamer1() {return name;}
    const AttributeList&  getAttributes() const {return attributes;}
    uint32_t              getBlockNum() const {return blockNum;}
    bool                  isChanged() {return changed;}
    int                   getId() const {return id;}


    Schema& setName(std::string aName);
    Schema& setAttributes(AttributeList& x);
    Schema& setBlockNum(uint32_t number);
    Schema& setIsChanged(bool achanged);
    Schema& setId(int num);


    Schema&               addAttribute(const Attribute &anAttribute);
    const AttributeOpt&      getAttribute(const std::string &aName) const;
      
        //STUDENT: These methods will be implemented in the next assignment...

    std::string           getPrimaryKeyName() const;
    uint32_t              getNextAutoIncrementValue();
    
    //check has a primary key
    string          getPrimaryKey();
    bool            hasAttributes(string name);


  protected:
    
    AttributeList   attributes;
    std::string     name;
    uint32_t        blockNum;  //storage location.
    bool            changed;
    int             id;

  };
  
}
#endif /* Schema_hpp */
