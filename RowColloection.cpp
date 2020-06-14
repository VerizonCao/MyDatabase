//
// Created by Cao Zongheng on 5/10/20.
//
// this class is used for store row collections

#include "RowColloection.h"

namespace ECE141{


    RowColloection::RowColloection(const RowColloection& aCopy){
        list = aCopy.list;
    }

    RowColloection& RowColloection::operator=(const RowColloection& aRow){
        list = aRow.list;
        return *this;
    }

    void RowColloection::addRow(Row& aRow){
        list.push_back(aRow);
    }
}
