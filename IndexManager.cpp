//
// Created by Cao Zongheng on 5/24/20.
//

#include "IndexManager.h"


using namespace std;

namespace ECE141{

    //constructor
    IndexManager::IndexManager(){

    }

    IndexManager::~IndexManager(){

    }

    //save and load
    StorageBlock* IndexManager::saveInfo(){
        StorageBlock* aBlock = new StorageBlock(BlockType::meta_block);
        int size = getIndexNum();
        aBlock->header.id = size;
        stringstream ss;
        //save the meta info
        string append = "";
        for(auto it : indexToc){
            string tableName = it.first;
            for(auto i : it.second){
                string indexName = i.first;
                int blockNum = i.second;
                string num;  ss << blockNum;  ss >> num; ss.clear();
                append += tableName + ":";
                append += indexName + ",";
                append += num + ";";
            }
        }
        memcpy(aBlock->data, append.c_str(), append.size());
        return aBlock;
    }

    StorageBlock* IndexManager::saveIndex(string tableName, string indexName){
        //save one index of certain table and index name
        if(!isIndexExisted(tableName, indexName)) return nullptr;
        StorageBlock* aBlock = new StorageBlock(BlockType::index_block);
        Index tempIndex = indexMap[tableName][indexName];
        //type
        memcpy(aBlock->data, (char*)&tempIndex.getValue(), 1);
        //append
        stringstream ss;
        string append = "";
        append += tableName + "," + indexName + ";";
        for(auto it : tempIndex.getList()){
            ValueType tempValue = it.first;
            int blockNum = it.second;
            string num; ss << blockNum;  ss >> num;  ss.clear();
            string res = changeValueTypeToString(tempValue);
            append += res + ":" + num + ",";
        }
        append += ";";
        memcpy(aBlock->data + 4, append.c_str(), append.size());
        return aBlock;
    }


    //use the same logic to load the info
    StatusResult IndexManager::loadInfo(StorageBlock& aBlock){
        char current[1012]; memset(current,0,sizeof(current));
        memcpy(current,aBlock.data, sizeof(aBlock.data));
        string getData = current;
        int start = 0;
        for(int i = 0; i<getData.length(); i++){
            if(getData[i] == ';'){
                string temp = getData.substr(start, i - start + 1);
                loadARowintoToc(temp);
                start = i +1;
            }
        }
        //till now, we load all the info into memory: indexToc
        return StatusResult();
    }

    StatusResult IndexManager::loadIndex(StorageBlock& aBlock){
        //load a index block into memory: indexMap
        char current[1008]; memset(current,0,sizeof(current));
        memcpy(current,aBlock.data + 4, sizeof(aBlock.data) - 4);  //data
        string getData = current;
        string tableName;  string indexName;
        DataType tempType;  memcpy(&tempType,aBlock.data, 1);   //type

        int start = 0;
        for(int i = 0; i<getData.length(); i++){
            //load table and indexName;
            if(getData[i] == ';' && start == 0){
                string* res = getTableNameAndIndexName(getData.substr(start, i - start));
                tableName = res[0];  indexName = res[1];
                start = i + 1;
            }
            //load each place
            else if(getData[i] == ',' && start != 0){
                string temp = getData.substr(start, i - start);
                loadValueAndBlockNum(temp,tableName,indexName, tempType);
                start = i + 1;
            }
        }
        return StatusResult();
    }

    StatusResult  IndexManager::showIndexes(){
        std::cout<<"+-----------------+-----------------+\n";
        std::cout<<"| table           | field           |\n";
        std::cout<<"+-----------------+-----------------+\n";
        int count = 0;
        for(auto it : indexToc){
            for(auto i : it.second){
                cout<<"| "<< setw(16)<< std::setiosflags(std::ios::left) << it.first
                << "| " << setw(16) << std::setiosflags(std::ios::left) << i.first << "|\n";
                std::cout<<"+-----------------+-----------------+\n";
                count++;
            }
        }
        cout<< count << " rows in set \n";
        return StatusResult();
    }

    //in memory operation
    StatusResult IndexManager::addIndexIntoMap(Index& aIndex, string tableName, string indexName){
        indexMap[tableName][indexName] = aIndex;
        return StatusResult();
    }

    StatusResult   IndexManager::addIndexIntoToc(string tableName, string indexName, int blockNum){
        indexToc[tableName][indexName] = blockNum;
        return StatusResult();
    }

    bool         IndexManager::isIndexExisted(string tableName, string indexName){
        for(auto it : indexToc[tableName]){
            if(it.first == indexName) return true;
        }
        return false;
    }

    bool         IndexManager::isIndexLoaded(string tableName, string indexName){
        for(auto it : indexMap[tableName]){
            if(it.second.getTableName() == indexName) return true;
        }
        return false;
    }

    Index&       IndexManager::getIndex(string tableName, string indexName){
        return indexMap[tableName][indexName];
    }

    int          IndexManager::getIndexNum(){
        int res = 0;
        for(auto it : indexMap){
            res += it.second.size();
        }
        return res;
    }

    //delete in db.file
    StatusResult  IndexManager::deleteAllIndexOfTable(string tableName, Storage& store){
        for(auto it: indexToc[tableName]){
            uint32_t blockNum = it.second;
            store.writeBlank(blockNum);//write blank
        }
        return StatusResult();
    }

    int         IndexManager::getIndexBlockNum(string tableName, string indexName){
        int res = indexToc[tableName][indexName];
        return res;
    }



    //helper function
    string   IndexManager::changeValueTypeToString(ValueType& temp){
        string res;
        stringstream ss;  bool x = false;
        switch (temp.index()){
            case 0: ss << get<u_int32_t>(temp); ss >> res; break;
            case 1: ss << get<float>(temp);   ss >> res;  break;
            case 2: x = get<bool>(temp); res = x == true ? "TRUE" : "FALSE"; break;
            case 3: res = get<string>(temp); break;
        }
        ss.flush();
        return res;
    }

    //insert a row into memory:indexMap by a string
    StatusResult IndexManager::loadValueAndBlockNum(string temp, string tableName, string indexName, DataType aType){
        stringstream ss;
        for(int i = 0; i<temp.length(); i++){
            if(temp[i] == ':'){
                string Value = temp.substr(0,i);
                string blockNum = temp.substr(i+1);
                //get the ValueType
                ValueType tempType;
                switch(aType){
                    case DataType::bool_type:
                        tempType = Value == "FALSE" ? false : true;
                        break;
                    case DataType::float_type:
                        float num1;
                        ss << Value;  ss >> num1; tempType = num1; ss.clear();
                        break;
                    case DataType::int_type:
                        __uint32_t num2;
                        ss << Value;  ss >> num2; tempType = num2; ss.clear();
                        break;
                    case DataType::varchar_type:
                        tempType = Value;
                        break;
                    default: break;
                }
                //get the blockNum
                int num = 0;
                ss << blockNum;  ss >> num;  ss.clear();

                //insert into list
                indexMap[tableName][indexName].getList().insert(make_pair(tempType,num));
            }
        }
        return StatusResult();
    }

    //get the name and index by a string
    string*  IndexManager::getTableNameAndIndexName(string temp){
        string* res = new string[2];
        for(int i = 0; i<temp.length(); i++){
            if(temp[i] == ','){
                res[0] = temp.substr(0,i);
                res[1] = temp.substr(i+1);
            }
        }
        return res;
    }

    //load a row into memory: indexToc by a string
    StatusResult  IndexManager::loadARowintoToc(string append){
        int start = 0;
        string tableName; string indexName; string num;  int blockNum = 0;
        stringstream ss;

        for(int i = 0; i<append.length(); i++){
            if(append[i] == ':'){
                tableName = append.substr(start, i - start);
                start = i + 1;
            }
            else if(append[i] == ','){
                indexName = append.substr(start, i - start);
                start = i + 1;
            }
            else if(append[i] == ';'){
                num = append.substr(start, i - start);
                ss << num;  ss >> blockNum;
            }
        }
        indexToc[tableName][indexName] = blockNum;  //add this into map
        return StatusResult();
    }
}
