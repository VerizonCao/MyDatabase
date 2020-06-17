//
// Created by Cao Zongheng on 5/24/20.
//
// used for validate name and type of a table

#ifndef ECE141_SP20_ASSIGNMENT7_ZONGHENG_CAO_VALIDATOR_H
#define ECE141_SP20_ASSIGNMENT7_ZONGHENG_CAO_VALIDATOR_H

#include "Other/Value.hpp"
#include "Schema.hpp"
#include "Row.hpp"



namespace ECE141{

    using SchemaMap = std::map<std::string, Schema*>;


    class Validator {

    public:

        Validator(SchemaMap& s){
            schemas = s;
        }
        ~Validator(){}
        bool              checkTableViewName(const string& name, const vector<string>& list, const string& joinTableName);
        bool              checkWhereAndOrder(const string& name, const string& name1, const string& joinTableName);
        bool              checkAttributeName(const vector<string>& properties, string theName);
        bool              checkAttributeType(const vector<Row*>& aRowList, string theName);
        Validator&        setSchemas(SchemaMap& s);
        SchemaMap&        getSchemas();


    protected:

        SchemaMap       schemas;
        map<int,DataType> intToDataType = {
                make_pair(0,DataType::int_type),
                make_pair(1,DataType::float_type),
                make_pair(2,DataType::bool_type),
                make_pair(3,DataType::varchar_type)
        };
    };





}


#endif //ECE141_SP20_ASSIGNMENT7_ZONGHENG_CAO_VALIDATOR_H
