//
// Created by Cao Zongheng on 5/10/20.
//
// This class is used for show a table by rowCollections
//

/**    Main function
*       used to show a table by given attributed and limit number
*      +--------------------+--------------+
*      | id  | first_name   | last_name    |
*      +-----+--------------+--------------+
*      | 1   | chandhini    | grandhi      |
*      | 3   | rick         | gessner      |
*      | 2   | savya        |              |
*      +-----+--------------+--------------+
*      3 rows in set
*/


#ifndef ECE141_SP20_ASSIGNMENT6_ZONGHENG_CAO_TABLEVIEW_H
#define ECE141_SP20_ASSIGNMENT6_ZONGHENG_CAO_TABLEVIEW_H

#include <string>
#include <vector>
#include "Other/Errors.hpp"
#include "RowColloection.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace ECE141{

    class TableView {

    public:
        TableView(){
            isStar = false;
        }
        ~TableView(){}

        //Show a Row
        void ShowRowList(RowColloection& aRowCollections, int num = -1, double time = 0.0, bool join = false) const;
        //show a header
        void  getHeader(Row& sample) const;
        void getLine(Row& aRow, bool join = false) const;
        //show a "--------"
        string getFrame(Row& sample) const;
        string getValueToString(ValueType& data, bool join = false) const;

        //getter and setter
        StatusResult addShowName(vector<string>& aShowName);
        vector<string> getShowName() const;
        bool getIsStar(){return isStar;}
        TableView& setIsStar(bool star);

    protected:
        bool isStar;   //isStar means we choose all the attributes in this table
        vector<string> showName;
    };

}



#endif //ECE141_SP20_ASSIGNMENT6_ZONGHENG_CAO_TABLEVIEW_H
