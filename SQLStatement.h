//
// Created by Cao Zongheng on 4/24/20.
//
// this class is used for table service, like create a table
// delete a table, insert some data and update the data as well
// as select operation

#ifndef ECE141_SP20_ASSIGNMENT4_ZONGHENG_CAO_SQLSTATEMENT_H
#define ECE141_SP20_ASSIGNMENT4_ZONGHENG_CAO_SQLSTATEMENT_H

#include "Interface/Statement.hpp"
#include "Schema.hpp"
#include "Other/Errors.hpp"
#include "Tokenizer.hpp"
#include "Database.hpp"
#include "Row.hpp"
#include "RowColloection.h"
#include "Filters.hpp"
#include "TableView.h"
#include "Validator.h"
#include "Index.hpp"
#include "Other/Timer.hpp"
#include "Other/Helpers.hpp"

using namespace std;

namespace ECE141 {

    using TableField = string;

    //create a table
    class CreateTableStatement : public Statement {
    public:

        CreateTableStatement(std::string aName, Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw){
            name = aName;
            database = aDatabase;
            schema = nullptr;  //set at parser
            stmtType = aStatementType;
        }

        ~CreateTableStatement(){

        }

        virtual StatusResult parse(Tokenizer &aTokenizer);
        virtual   StatusResult  run(std::ostream &aStream) const;

    protected:
        std::string name;
        Schema* schema;
        Database* database;
    };


    //drop a table
    class DropTableStatement : public Statement{

    public:
        DropTableStatement(std::string aName, Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw){
            name = aName;
            database = aDatabase;
            schema = nullptr;  //set at parser
            stmtType = aStatementType;
        }

        ~DropTableStatement(){

        }
        virtual   StatusResult  run(std::ostream &aStream) const;

    protected:
        string name;
        Schema* schema;
        Database* database;
    };

    //show a table
    class ShowTableStatement : public Statement{

    public:
        ShowTableStatement(std::string aName, Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw){
            name = aName;
            database = aDatabase;
            schema = nullptr;  //set at parser
            stmtType = aStatementType;
        }

        ~ShowTableStatement(){

        }
        virtual   StatusResult  run(std::ostream &aStream) const;

    protected:
        string name;
        Schema* schema;
        Database* database;
    };

    //describe table
    class DescribeTableStatement : public Statement{

    public:
        DescribeTableStatement(std::string aName, Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw){
            name = aName;
            database = aDatabase;
            schema = nullptr;  //set at parser
            stmtType = aStatementType;
        }
        ~DescribeTableStatement(){

        }

        virtual   StatusResult  run(std::ostream &aStream) const;

    protected:
        string name;
        Schema* schema;
        Database* database;

    };

    //insert data into a table
    class InsertTableStatement : public Statement{

    public:
        InsertTableStatement(std::string aName, Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw){
            name = aName;
            database = aDatabase;
            schema = nullptr;  //set at parser
            stmtType = aStatementType;
        }
        ~InsertTableStatement(){

        }

        virtual StatusResult parse(Tokenizer &aTokenizer);
        virtual   StatusResult  run(std::ostream &aStream) const;

    protected:
        string name;
        Schema* schema;
        Database* database;
        vector<Row*> rowList;
        vector<string>  properties;
    };

    //delete a table
    class DeleteTableStatement : public Statement{
    public:
        DeleteTableStatement(std::string aName, Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw){
            name = aName;
            database = aDatabase;
            stmtType = aStatementType;
        }

        ~DeleteTableStatement(){}

        virtual   StatusResult  run(std::ostream &aStream) const;

    protected:
        string name;
        Database* database;
    };

    //select from
    class SelectStatement : public Statement{
    public:
        SelectStatement(Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw){
            database = aDatabase;
            stmtType = aStatementType;
        }

        ~SelectStatement(){}

        virtual   StatusResult  run(std::ostream &aStream) const;
        virtual StatusResult parse(Tokenizer &aTokenizer);

        //parse table join
        StatusResult parseJoin(Tokenizer &aTokenizer);
        StatusResult parseTableField(Tokenizer& aTokenizer, TableField& theField);
        StatusResult parseTableName(Tokenizer& aTokenizer, string& theTable);

        StatusResult addJoinRowsToRows(RowColloection& aRowCollections, RowColloection& aRowCollections1) const;
        bool         checkJoinName() const;

        StatusResult addEmptyAttributes(Row& aRow1, const AttributeList & aList, string primaryName, RowColloection& aRowCollections) const;
        bool         matchJoinCondition(Row& aRow1, Row& aRow2) const;
        StatusResult addTwoRows(Row& aRow1, Row& aRow2, RowColloection& aRowCollections) const;

        StatusResult addRightRow(RowColloection& aRowCollections, Row& aRow2, const AttributeList & aList, string primaryName) const;



        //inner class, join operation
        struct Join  {
            Join(const std::string &aTable, Keywords aType, const std::string &aLHS, const std::string &aRHS)
                    : joinType(aType),table(aTable) ,lhs(aLHS), rhs(aRHS) {}

            Keywords    joinType;
            std::string table;
            TableField  lhs;
            TableField  rhs;

            string* getTableName() const{
                string* res = new string[2];
                int index = lhs.find(".");
                res[0] = lhs.substr(0,index);
                int index1 = rhs.find(".");
                res[1] = rhs.substr(0,index1);
                return res;
            }

            string* getFieldName() const{
                string* res = new string[2];
                int index = lhs.find(".");
                res[0] = lhs.substr(index + 1);
                int index1 = rhs.find(".");
                res[1] = rhs.substr(index1 + 1);
                return res;
            }
        };



    protected:
        string name;
        Database* database;
        Filters filters;
        TableView tableView;
        std::map<string, Operators>    operatorChange = { make_pair("=",Operators::equal_op),
                                                         make_pair(">",Operators::gte_op),
                                                         make_pair("<", Operators::lte_op),
        };
        std::vector<Join> joins;

    };


    //update a row of data
    class UpdateStatement : public Statement{
    public:

        UpdateStatement(Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw){
            database = aDatabase;
            stmtType = aStatementType;
        }

        ~UpdateStatement(){}

        virtual StatusResult parse(Tokenizer &aTokenizer);
        virtual   StatusResult  run(std::ostream &aStream) const;

    protected:
        string name;
        Database* database;
        Filters filters;
        std::map<string, Operators>    operatorChange = { make_pair("=",Operators::equal_op),
                                                          make_pair(">",Operators::gte_op),
                                                          make_pair("<", Operators::lte_op),
        };

    };


    //show all indexes
    class ShowIndexStatement : public Statement{
    public:

        ShowIndexStatement(Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw){
            database = aDatabase;
            stmtType = aStatementType;
        }

        ~ShowIndexStatement(){

        }
        virtual StatusResult parse(Tokenizer &aTokenizer);
        virtual   StatusResult  run(std::ostream &aStream) const;

    protected:
        string name;
        Database* database;
    };


    //alter a table
    class AlterStatement : public Statement{
    public:
        AlterStatement(Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw){
            database = aDatabase;
            stmtType = aStatementType;
        }
        ~AlterStatement(){}

        virtual StatusResult parse(Tokenizer &aTokenizer);
        virtual   StatusResult  run(std::ostream &aStream) const;

    protected:
        string name;
        Database* database;
        Attribute attribute;
        Filters filters;
    };









}





#endif //ECE141_SP20_ASSIGNMENT4_ZONGHENG_CAO_SQLSTATEMENT_H
