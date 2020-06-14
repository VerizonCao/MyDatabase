//
// Created by Cao Zongheng on 4/14/20.
//

#include "DBStatement.h"
#include "Folders/FolderReader.hpp"
#include "Storage.hpp"
#include "Database.hpp"


namespace ECE141{
    //we have nothing right now, all in hpp file

    StatusResult  DescribeStatement::run(std::ostream &aStream) const{

        FolderReader theReader(StorageInfo::getDefaultStoragePath());
        if(!theReader.exists(name)) return StatusResult(unknownDatabase);

        Database db(name,OpenExistingStorage());
        int size = db.getId();
        int count = 1;

        //output
        std::cout<<"+-----------+--------------+-----------------------------+\n";
        std::cout <<"| Block#    | Type         | Other                       |\n";
        std::cout<<"+-----------+--------------+-----------------------------+\n";
        std::cout<<"| 0         | meta         |                             |\n";

        for(int i = 1; i<= size; i++){
            StorageBlock aBlock(BlockType::unknown_block);
            db.getStorage().readBlock(aBlock,i);
            if(aBlock.header.type == 'E'){
                string name = "";
                for(auto it : db.getToc().list){  //find the block in the list
                    if(it.second == i){
                        name = it.first;
                        break;
                    }
                }
                std::cout<<"| "<< std::setw(10)<< std::setiosflags(std::ios::left)<<i
                         <<"| schema       | "
                         << std::setw(28)<< std::setiosflags(std::ios::left)<<name<<"|\n";
            }
            else if(aBlock.header.type == 'D'){
                stringstream ss;
                string name = "";  string id = "";
                ss << aBlock.header.id;  ss >> id;
                name += id + ",  ";
                char table[1012];  memcpy(&table,aBlock.data, sizeof(aBlock.data));
                string table1 = table;
                int index = table1.find(";");
                name +=  table1.substr(0,index);
                std::cout<<"| "<< std::setw(10)<< std::setiosflags(std::ios::left)<<i
                         <<"| data         | "
                         << std::setw(28)<< std::setiosflags(std::ios::left)<<name<<"|\n";
            }
            else if(aBlock.header.type == 'I'){
                //index block
            }
            else{
                count--;
            }
            count++;
        }
        std::cout<<"+-----------+--------------+-----------------------------+\n";
        std::cout<<count<<" rows in set\n";
        return StatusResult();
    }

}
