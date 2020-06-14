//
// Created by Cao Zongheng on 4/14/20.
//
// Database Statements: DescribeStatement, separate from DBManager
// Show all the blocks in this database file


#ifndef ECE141_SP20_ASSIGNMENT2_ZONGHENG_CAO_DBSTATEMENT_H
#define ECE141_SP20_ASSIGNMENT2_ZONGHENG_CAO_DBSTATEMENT_H

#include "Statement.hpp"

namespace ECE141{


    class DBStatement : public Statement{

    public:
        DBStatement(std::string aname, Keywords aStatementType = Keywords::unknown_kw){
            stmtType = aStatementType;
            name = aname;
        }
        DBStatement(const DBStatement &aCopy){
            name = aCopy.name;
            stmtType = aCopy.stmtType;
        }

        ~DBStatement(){};

        //getter
        virtual   std::string   getName() const{ return name;}

    protected:
        std::string name;
    };

    class DescribeStatement : public Statement{

    public:

        DescribeStatement(std::string aname, Keywords aStatementType = Keywords::unknown_kw){
            stmtType = aStatementType;
            name = aname;
        }


        virtual   std::string   getName() const{ return name;}
        virtual   StatusResult  run(std::ostream &aStream) const;



    protected:
        string name;
    };
}




#endif //ECE141_SP20_ASSIGNMENT2_ZONGHENG_CAO_DBSTATEMENT_H
