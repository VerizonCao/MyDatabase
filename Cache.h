//
// Created by Cao Zongheng on 6/25/20.
//

#ifndef MYDATABASE_CACHE_H
#define MYDATABASE_CACHE_H

#include "Tokenizer.hpp"
#include "RowColloection.h"
#include <queue>
#include "StorageBlock.hpp"

namespace ECE141{

    using namespace std;

    class Cache {  //interface for QueryCache and BlockCache

    };


    class QueryCache : public Cache{

    public:

        QueryCache(){
            size = 5;
        }




    protected:
        //map of query and it's result (only support select right now)
        //list of Tokenizer
        vector<Tokenizer> tokenList;
        //list of RowCollections
        vector<RowColloection> rowList;

        int size;

    };


    class BlockCache : public Cache{

    public:
        BlockCache(){
            size = 100;
        }

        StatusResult AddaBlock(int num, StorageBlock& storageBlock);
        StorageBlock*  getABlock(int num);

    protected:

        //map of block and its blockNum
        vector<StorageBlock> blockList;
        //list of RowCollections
        vector<int> numList;

        int size;

    };

}



#endif //MYDATABASE_CACHE_H
