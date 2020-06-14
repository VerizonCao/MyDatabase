//  Created by rick gessner on 5/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//
//  Modified by Zongheng Cao after the skeleton   All rights reserved.
//
//  this class is used for collect the conditions in where operation
//  and then filter the rowCollections and return the rows satisfy
//  the condition


#ifndef Filters_h
#define Filters_h

#include <stdio.h>
#include <vector>
#include <string>
#include "Other/Errors.hpp"
#include "Other/Value.hpp"
#include "Tokenizer.hpp"
#include "Row.hpp"
#include "RowColloection.h"
#include <algorithm>
#include <sstream>

namespace ECE141 {


  using OperationFunc = bool(*)(const ValueType & lhs, const ValueType & rhs);

  class Row;
  class Schema;

  struct Operand {
    Operand() {}
    Operand(std::string &aName, TokenType aType, ValueType &aValue, uint32_t anId=0)
      :  type(aType), name(aName), value(aValue),entityId(anId) {}
    
    TokenType   type; //so we know if it's a field, a const (number, string)...
    std::string name; //for named attr. in schema
    ValueType   value;
    uint32_t    entityId;
  };
  
  //---------------------------------------------------

  struct Expression {
    Operand     lhs;
    Operand     rhs;
    Operators   op;
    
    Expression(Operand &aLHSOperand, Operators anOp, Operand &aRHSOperand)
      : lhs(aLHSOperand),  rhs(aRHSOperand) ,op(anOp) {}

    ValueType& getByIndex(int index);

    bool operator()(KeyValues &aList);
    bool operator()(Row& aRow);
    bool operator()(ValueType& aType);
  };

  //---------------------------------------------------

  using Expressions = std::vector<Expression*>;

  //---------------------------------------------------
  struct RowSorter {
      RowSorter(const std::string &aField) : field(aField) { }
      RowSorter(){ }

      bool operator () (Row& aRow1,
                        Row& aRow2) {
          ValueType &theLHS=aRow1.pair2[field];
          ValueType &theRHS=aRow2.pair2[field];
          return theLHS < theRHS;
      }

      std::string field;
  };

  //---------------------------------------------------

  class Filters {

  public:
    
    Filters(){limit = false;  order = false; And = false;  Or = false;};
    Filters(const Filters &aFilters);
    ~Filters(){
        for(auto it : expressions){
            delete it;
        }
    };
    
    size_t        getCount() const {return expressions.size();}
    Filters&      add(Expression *anExpression);
    bool          match(Row& aRow) const;   //match used for select
    bool          match1(Row& aRow) const;  //match1 used for uodate
    ValueType&    changeStringToValue(int index, string data) const;

    const Expressions&  getExpression() const{return expressions;}

    StatusResult sortCollections(RowColloection* aRowCollection) const;



    friend class Tokenizer;

    bool limit;
    int limitNumber;
    bool order;
    string orderName;
    RowSorter    sorter;
    bool And;
    bool Or;

    std::map<string,string> updateValues;

  protected:
    Expressions  expressions;

  };


}

#endif /* Filters_h */

