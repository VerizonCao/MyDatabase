//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.


#include "Attribute.hpp"

namespace ECE141 {

    Attribute::Attribute(DataType aType):type(aType){
        length = 0;
        nullable = false;
        primary = false;
        key = false;
        auto_increment = false;
        Adefault = "";
    }
    Attribute::Attribute(std::string aName, DataType aType):name(aName),type(aType){
        length = 0;
        nullable = false;
        primary = false;
        key = false;
        auto_increment = false;
        Adefault = "";
    }
    Attribute::Attribute(const Attribute &aCopy):name(aCopy.name),type(aCopy.type){
        length = aCopy.length;
        auto_increment = aCopy.auto_increment;
        primary = aCopy.primary;
        key = aCopy.key;
        nullable = aCopy.nullable;
        Adefault = aCopy.Adefault;
    }
    Attribute::~Attribute(){}

    Attribute&    Attribute::setName(std::string &aName){
        name = aName;
        return *this;
    }
    Attribute&    Attribute::setType(DataType aType){
        type = aType;
        return *this;
    }



    int Attribute::getLength(){
        return length;
    }
    bool Attribute::getAutoIncrement(){
        return auto_increment;
    }
    bool Attribute::getPrimary(){
        return primary;
    }
    bool Attribute::getKey(){
        return key;
    }
    bool Attribute::getNullable(){
        return nullable;
    }
    DataType Attribute::getType(){
        return type;
    }
    std::string Attribute::getDefault(){
        return Adefault;
    }



    //setter
    Attribute&    Attribute::setLength(int aLength){
        length = aLength;
        return *this;
    }
    Attribute&    Attribute::setAutoIncrement(bool type){
        auto_increment = type;
        return *this;
    }
    Attribute&    Attribute::setPrimary(bool type){
        primary = type;
        return *this;
    }
    Attribute&    Attribute::setKey(bool type){
        key = type;
        return *this;
    }
    Attribute&    Attribute::setNullable(bool type){
        nullable = type;
        return *this;
    }
    Attribute&    Attribute::setdefault(std::string aType){
        Adefault = aType;
        return *this;
    }



}
