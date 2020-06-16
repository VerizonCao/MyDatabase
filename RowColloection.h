//
// Created by Cao Zongheng on 5/10/20.
//
// this class is used for store row collections


#ifndef ECE141_SP20_ASSIGNMENT6_ZONGHENG_CAO_ROWCOLLOECTION_H
#define ECE141_SP20_ASSIGNMENT6_ZONGHENG_CAO_ROWCOLLOECTION_H


#include "Row.hpp"

using namespace std;

namespace ECE141 {

    class RowColloection {

    public:

        RowColloection(){}
        RowColloection(const RowColloection& aCopy);
        RowColloection& operator=(const RowColloection& aRow);

        ~RowColloection(){};

        void addRow(Row& aRow);
        
        vector<Row> list;

    };
}




#endif //ECE141_SP20_ASSIGNMENT6_ZONGHENG_CAO_ROWCOLLOECTION_H
