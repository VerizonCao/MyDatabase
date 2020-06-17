//
// Created by Cao Zongheng on 5/24/20.
//

#include "Validator.h"

namespace ECE141{

    Validator& Validator::setSchemas(SchemaMap& s){
        schemas = s;
        return *this;
    }

    SchemaMap& Validator::getSchemas(){
        return schemas;
    }


    bool         Validator::checkAttributeName(const vector<string>& properties, string theName){
        AttributeList list = schemas[theName]->getAttributes();
        int index1 = 0; int index2 = 0;
        while(index1 < properties.size() && index2 < list.size()){
            string name1 = properties[index1];
            string name2 = list[index2].getName();
            if(name1 != name2){
                if(list[index2].getDefault() != "NULL" && list[index2].getAutoIncrement() != true) return false;
                else{
                    index2++;
                }
            }
            else{
                index1++;   index2++;
            }
        }
        if(index1 != properties.size()) return false;
        return true;
    }
    bool         Validator::checkAttributeType(const vector<Row*>& aRowList, string theName){
        Schema* schema = schemas[theName];
        AttributeList list = schema->getAttributes();
        for(Row* it : aRowList){
            int index1 = 0; int index2 = 0;
            while(index1 < it->getPair().size() && index2 < list.size()){
                DataType valueType1 = intToDataType[it->getPair()[index1].second.index()];
                DataType valueType2 = list[index2].getType();
                if(valueType1 != valueType2){
                    if(list[index2].getDefault() != "NULL" && list[index2].getAutoIncrement() != true) return false;
                    else index2++;
                }
                else{
                    index1++;   index2++;
                }
            }
            if(index1 != it->getPair().size()) return false;
        }
        return true;
    }

    bool         Validator::checkTableViewName(const string& name, const vector<string>& list, const string& joinTableName){
        Schema* schema = schemas[name];
        Schema* schema1 = schemas[joinTableName];
        AttributeList list1 = schema->getAttributes();

        for(auto it : list){
            bool find = false;
            for(auto it1 : list1){
                if(it == it1.getName()){
                    find = true;
                    break;
                }
            }
            if(!find && joinTableName != ""){
                for(auto it2 : schema1->getAttributes()){
                    if(it == it2.getName()){
                        find = true;
                        break;
                    }
                }
            }
            if(!find) return false;
        }
        return true;
    }
    bool         Validator::checkWhereAndOrder(const string& name, const string& name1, const string& joinTableName){

        Schema* schema = schemas[name];
        Schema* schema1 = schemas[name];
        AttributeList list1 = schema->getAttributes();
        AttributeList list2 = schema1->getAttributes();
        bool find = false;
        for(auto it : list1){
            if(name1 == it.getName()){
                find = true;
                break;
            }
        }
        if(!find && joinTableName != ""){
            for(auto it : list2){
                if(name1 == it.getName()){
                    find = true;
                    break;
                }
            }
        }
        return find;
    }



}
