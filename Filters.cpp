//  Created by rick gessner on 5/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//
//  Modified by Zongheng Cao after skeleton
//



#include "Filters.hpp"
#include "Row.hpp"

using namespace std;
namespace ECE141 {


    Filters&      Filters::add(Expression *anExpression){
        expressions.push_back(anExpression);
        return *this;
    }

    //check if it has the field name
    Expression* Filters::hasThisFiled(string fieldName) const{
        for(Expression* it : expressions){
            if(it->lhs.name == fieldName) return it;
        }
        return nullptr;
    }


    bool          Filters::match(Row& aRow) const{
        if(Or){   //if satisfy, return true
            for(Expression* it : expressions){
                if((*it)(aRow))  return true;
            }
        }

        else {  //if not, return false
            for(Expression* it : expressions){
                if(!(*it)(aRow))  return false;
            }
        }
        return true;
    }

    bool          Filters::match1(Row& aRow) const{
        if(Or){
            for(Expression* it : expressions){
                if((*it)(aRow)){
                    for(auto it : updateValues){
                        string name = it.first;
                        int index = aRow.pair2[name].index();
                        ValueType temp = changeStringToValue(index,it.second);
                        aRow.pair2[name] = temp;
                        for(auto& i : aRow.getPair()){
                            if(i.first == name){
                                i.second = temp;
                                break;
                            }
                        }
                    }
                    return true;
                }
            }
            return false;
        }
        else {
            for(Expression* it : expressions){
                if(!(*it)(aRow))  return false;
            }
            for(auto it : updateValues){
                string name = it.first;
                int index = aRow.pair2[name].index();
                ValueType temp = changeStringToValue(index,it.second);
                aRow.pair2[name] = temp;

                for(KeyValuePair& i : aRow.getPair()){
                    if(i.first == name){
                        i.second = temp;
                        break;
                    }
                }
            }
            return true;
        }
    }

    ValueType&    Filters::changeStringToValue(int index, string data) const{
        ValueType* aType = new ValueType();
        uint32_t num = 0;
        float num1 = 0.0;
        stringstream ss;
        switch (index){
            case 0:
                ss << data;
                ss >> num;
                *aType = num;
                break;
            case 1:
                ss << data;
                ss >> num1;
                *aType = num1;
                break;
            case 2:
                *aType = data == "FALSE" ? false : true;
                break;
            case 3:
                *aType = data;
                break;
            case 4:
                break;
        }
        return *aType;
    }
    

    StatusResult Filters::sortCollections(RowColloection* aRowCollection) const{
        sort(aRowCollection->list.begin(), aRowCollection->list.end(),sorter);
        return StatusResult();
    }


    //used for expression from op to get a function

    bool GT(const ValueType& a, const ValueType& b) { return a > b; }
    bool GE(const ValueType& a, const ValueType& b) { return a >= b; }
    bool LT(const ValueType& a, const ValueType& b) { return a < b; }
    bool LE(const ValueType& a, const ValueType& b) { return a <= b; }
    bool EQ(const ValueType& a, const ValueType& b) { return a == b; }
    bool UE(const ValueType& a, const ValueType& b) { return a != b; }

    static map<Operators, OperationFunc>  mapOp = {
            {Operators::equal_op, EQ},
            {Operators::notequal_op, UE},
            {Operators::gt_op, GT},
            {Operators::gte_op, GT},
            {Operators::lt_op, LT},
            {Operators::lte_op, LT},
    };


    bool Expression::operator()(Row& aRow){  //check is this row satisfy the conditions
        ValueType aType = aRow.pair2[lhs.name];
        ValueType c = getByIndex(aType.index());
        return mapOp[op](aType,c);
    }

    bool Expression::operator()(ValueType& aType){
        ValueType c = getByIndex(aType.index());
        return mapOp[op](aType,c);
    }

    ValueType& Expression::getByIndex(int index){
        ValueType* c = new ValueType();
        uint32_t num = 0;
        float num2 = 0.0;
        bool num3 = false;
        stringstream ss;

        switch (index){
            case 0:
                ss << rhs.name;
                ss >> num;
                *c = num;
                break;
            case 1:
                ss << rhs.name;
                ss >> num2;
                *c = num2;
                break;
            case 2:
                num3 = rhs.name == "FALSE" ? false : true;
                *c = num3;
                break;
            case 3:
                *c = rhs.name;
                break;
        }
        return *c;
    }


    //index
    Index* Filters::matchIndex(Index& tempIndex, Expression* it) const{
        //create a new index by given index
        //judge type, > or <
        Index* res = new Index();
        if(it->op == Operators::gte_op || it->op == Operators::gt_op){
            auto greater = tempIndex.getList().upper_bound(it->rhs.value);
            for(auto index = greater; index != tempIndex.getList().end(); index++){
                res->getList()[(*index).first] = (*index).second;
            }
        }
        else if(it->op == Operators::lte_op || it->op == Operators::lt_op){
            auto fewer = tempIndex.getList().lower_bound(it->rhs.value);
            for(auto index = tempIndex.getList().begin(); index != fewer; index++){
                res->getList()[(*index).first] = (*index).second;
            }
        }
        return res;
    }


}



