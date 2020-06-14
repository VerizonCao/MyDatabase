//
//  Value.hpp
//  RGAssignment5
//
//  Created by rick gessner on 4/26/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Value_h
#define Value_h

#include <variant>
#include <optional>
#include <string>
#include <map>
#include <cstdint>
#include "Errors.hpp"


using namespace std;
namespace ECE141 {


  struct Storable {
     virtual StatusResult  encode(std::ostream &aWriter)=0;
     virtual StatusResult  decode(std::istream &aReader)=0;
  };

  enum class DataType {
     no_type='N', bool_type='B', datetime_type='D', float_type='F', int_type='I', varchar_type='V',
   };

  using ValueType = std::variant<uint32_t, float, bool, std::string>;

  using KeyValues = std::map<const std::string, ValueType>;  //很多名字和value的对应

  using KeyValuePair = std::pair<std::string, ValueType>;   //一对pair

  const int data_size = 1012;


}

#endif /* Value_h */
