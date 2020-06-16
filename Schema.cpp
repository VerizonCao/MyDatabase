//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.


#include "Schema.hpp"
#include <string>


namespace ECE141 {


    Schema::Schema(const std::string aName): attributes(){
        name = aName;
        changed = false;
        blockNum = 0;
    }
    Schema::Schema(const Schema &aCopy){
        name = aCopy.name;
        changed = false;
        blockNum = 0;
        attributes = aCopy.attributes;
    }

    Schema::~Schema(){

    }

    Schema&               Schema::addAttribute(const Attribute &anAttribute){
        if(std::nullopt == getAttribute(anAttribute.getName())){
            attributes.push_back(anAttribute);
        }
        return *this;
    }

    const AttributeOpt&      Schema::getAttribute(const std::string &aName) const {
        for(auto& i : attributes){
            if(i.getName() == aName) return i;
        }
        return std::nullopt;
    }

    string  Schema::getPrimaryKey(){
        for(auto it : attributes){
            if(it.getPrimary() == true){
                return it.getName();
            }
        }
        return "";   //means no primary key
    }

    bool   Schema::hasAttributes(string name){
        for(Attribute i : attributes){
            if(i.getName() == name){
                return true;
            }
        }
        return false;
    }




    //setter
    Schema& Schema::setName(std::string aName){
        name = aName;
        return *this;
    }
    Schema& Schema::setAttributes(AttributeList& x){
        attributes = x;
        return *this;
    }
    Schema& Schema::setBlockNum(uint32_t number){
        blockNum = number;
        return *this;
    }
    Schema& Schema::setIsChanged(bool achanged){
        changed = achanged;
        return *this;
    }
    Schema& Schema::setId(int num){
        id = num;
        return *this;
    }




}
