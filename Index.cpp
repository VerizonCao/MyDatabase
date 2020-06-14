//
// Created by Cao Zongheng on 5/24/20.
//


#include "Index.hpp"

using namespace std;
namespace ECE141{

    Index::Index(const Index& aCopy){
        field = aCopy.field;
        type = aCopy.type;
        tableName = aCopy.tableName;
        changed = aCopy.changed;
        blockNum = aCopy.blockNum;
        list = aCopy.list;
    }

    Index& Index::operator=(const Index& aCopy){
        field = aCopy.field;
        type = aCopy.type;
        tableName = aCopy.tableName;
        changed = aCopy.changed;
        blockNum = aCopy.blockNum;
        list = aCopy.list;
        return *this;
    }

    Index& Index::addKeyValue(const ValueType &aKey, uint32_t aValue){
        list.insert(make_pair(aKey,aValue));
        return *this;
    }
    Index& Index::removeKeyValue(const ValueType &aKey){
        for(auto it = list.begin(); it != list.end(); it++){
            if(it->first == aKey){
                list.erase(it);
                break;
            }
        }
        return *this;
    }
    bool Index::contains(const ValueType &aKey){
        for(auto it = list.begin(); it != list.end(); it++){
            if(it->first == aKey){
                return true;
            }
        }
        return false;
    }
    DataType& Index::getValue(){
        return type;
    }






}

