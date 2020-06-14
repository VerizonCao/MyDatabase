//
// Created by Cao Zongheng on 5/24/20.
//
//
// This class is used for manage index in a database

#ifndef ECE141_SP20_ASSIGNMENT8_ZONGHENG_CAO_INDEXMANAGER_H
#define ECE141_SP20_ASSIGNMENT8_ZONGHENG_CAO_INDEXMANAGER_H

#include "Index.hpp"
#include <string>
#include <sstream>


namespace ECE141{

    using IndexMap = map<string, map<string, Index>>;


    class IndexManager {


    public:

        IndexManager();

        ~IndexManager();

        StorageBlock* saveInfo();   //change the indexTable into a block and save the indexTable into db
        StorageBlock* saveIndex(string tableName, string indexName);   //change the indexMap into a block and save the indexMap into db

        StatusResult  loadInfo(StorageBlock& aBlock);   //load the indexTable from a block and save the indexTable into memory
        StatusResult  loadIndex(StorageBlock& aBlock);   //load the indexMap from a block and save the indexMap into memory


        StatusResult  addIndexIntoMap(Index& aIndex, string tableName, string indexName);  //add index into memory: map
        StatusResult  addIndexIntoToc(string tableName, string indexName, int blockNum);  // add index into memory: toc

        bool          isIndexExisted(string tableName, string indexName);   //look the toc to look if exist
        bool          isIndexLoaded(string tableName, string indexName);    //look the map ro see if loaded

        int           getIndexNum();   //total indexes in this database
        StatusResult  deleteAllIndexOfTable(string tableName, Storage& store);   //delete the index block in db
        int           getIndexBlockNum(string tableName, string indexName);

        StatusResult  showIndexes();


        //small helper function
        StatusResult  loadARowintoToc(string append);
        string        changeValueTypeToString(ValueType& temp);
        string*       getTableNameAndIndexName(string temp);
        StatusResult  loadValueAndBlockNum(string temp, string tableName, string indexName, DataType aType);

        //getter
        IndexMap&     getMap(){return indexMap;}
        map<string, map<string, int>>&  getToc(){return indexToc;}
        Index&        getIndex(string tableName, string indexName);    //get a index from map

    protected:

        //only need a map, because index itself contains the tableName, the place and the name
        IndexMap        indexMap;
        map<string, map<string, int>>      indexToc;    //load the meta info of the index of a database

    };
}



#endif //ECE141_SP20_ASSIGNMENT8_ZONGHENG_CAO_INDEXMANAGER_H
