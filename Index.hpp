//  Created by rick gessner on 5/17/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  used for in memory space of index, record the table and attribute name
//  and the valueMap: value, blockNum

#ifndef Index_h
#define Index_h

#include "Storage.hpp"
#include "Other/keywords.hpp"
#include "Other/Value.hpp"

using namespace std;
namespace ECE141 {
  
  using IntOpt = std::optional<uint32_t>;
  
  struct LessKey {
    bool operator()(const ValueType& anLHS, const ValueType& aRHS) const {
      return anLHS < aRHS;
    }
  };

  class Index{
  public:

    using ValueMap = std::map<ValueType, uint32_t, LessKey>;

    Index(){}

    Index(const Index& aCopy);

    Index& operator=(const Index& aCopy);

    Index(const std::string &aField, string tableName, DataType aType)
      : field(aField), type(aType), tableName(tableName), blockNum(0) {
        type=aType;
        changed=false;
      }
    
    virtual ~Index() {}
    
    ValueMap&           getList() {return list;}
    void                setChanged(bool aValue=true) {changed=aValue;}
    bool                isChanged() {return changed;}
    const std::string&  getFieldName() const {return field;}
    uint32_t            getBlockNum() const {return blockNum;}
    
      //manage keys/values in the index...
    Index& addKeyValue(const ValueType &aKey, uint32_t aValue);
    Index& removeKeyValue(const ValueType &aKey);    
    bool contains(const ValueType &aKey);    
    DataType& getValue();
    string   getTableName(){return tableName;}



  protected:
    
    std::string   field; // 名字 比如 name age
    DataType      type;    //是什么数据类型的
    string        tableName;     //table name
    bool          changed;   //是否被修改 如果被修改了 需要写入
    uint32_t      blockNum;  //storage block# of index...    //这个index在第几个block num
    ValueMap      list;   //tree map来存储value 和 对应的 index

  };

}
#endif /* Index_h */
