//
// Created by Cao Zongheng on 6/25/20.
//

#include "Cache.h"


namespace ECE141{




    StatusResult BlockCache::AddaBlock(int num, StorageBlock& storageBlock){

        //first check if beyond boundary
        if(blockList.size() >= size){
            int decreaseNum = blockList.size() - size;
            for(int i = 0; i<decreaseNum; i++){
                blockList.erase(blockList.begin());
                numList.erase(numList.begin());
            }
        }

        //directly add to the last
        blockList.push_back(storageBlock);
        numList.push_back(num);

        return StatusResult();
    }


    StorageBlock*  BlockCache::getABlock(int num){
        //get one and delete it, and add it to the last

        bool  find = false;

        int index = 0;
        for(auto i = numList.begin(); i != numList.end(); i++){
            if(*i == num){
                numList.erase(i);
                find = true;
                break;
            }
            index++;
        }

        if(find){  //find it
            StorageBlock res(BlockType::unknown_block);
            for(auto i = blockList.begin(); i != blockList.end(); i++){
                if(index-- == 0){
                    res = (*i);   //....
                    blockList.erase(i);
                    break;
                }
            }
            AddaBlock(num, res);
            return &res;
        }
        //not found
        else   return nullptr;

    }


}
