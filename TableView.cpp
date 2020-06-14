//
// Created by Cao Zongheng on 5/10/20.
//
//
// This class is used for show a table by rowCollections
//
#include "TableView.h"
#include <sstream>


namespace ECE141{

    void TableView::ShowRowList(RowColloection& aRowCollections, int num, double time, bool join) const{
        if(aRowCollections.list.size() == 0){
            cout<<"0 rows in set \n";
            return;
        }
        string frame = getFrame(aRowCollections.list[0]);
        //output
        cout<<frame<<endl;
        getHeader(aRowCollections.list[0]);
        cout<<frame<<endl;
        //content
        if(num != -1){
            for(int i = 0; i<num; i++){
                getLine(aRowCollections.list[i], join);
            }
        }
        else{
            for(Row i : aRowCollections.list){
                getLine(i, join);
            }
        }

        cout<<frame<<endl;
        cout<< (num == -1 ? aRowCollections.list.size() : num)  <<" rows in set" << "(" << time << " ms.)\n";
    }

    void TableView::getHeader(Row& sample) const{
        string append = "|   ";
        if(isStar){
            for(auto it: sample.getPair()){
                cout<<"| "<<it.first<<"            ";
            }
        }
        else{
            for(auto it : showName){
                cout<<"| "<<it<<"            ";
            }
        }
        cout<<"|\n";
    }

    void TableView::getLine(Row& aRow, bool join) const{
        string append = "   ";
        int size = append.size()+10;
        if(isStar){
            for(auto it : aRow.getPair()){
                int tempLength = size + it.first.size()-1;
                cout<<"| " << setw(tempLength)<< std::setiosflags(std::ios::left)<<getValueToString(it.second,join);
            }
        }
        else{
            for(auto it : showName){
                int tempLength = size + it.size()-1;
                cout<<"| " << setw(tempLength)<< std::setiosflags(std::ios::left)<<getValueToString(aRow.pair2[it],join);
            }
        }
        cout<<"|\n";
    }


    string TableView::getFrame(Row& sample) const{
        string append = "|   ";    // space
        int size = append.size()+10;
        int length = 0;
        if(isStar){
            for(auto it: sample.getPair()){
                string name = it.first;
                length += name.size() + size;
            }
        }
        else{
            for(auto it : showName){
                length += it.size() + size;
            }
        }
        string output(length-1,'-');
        output = "+" + output + "+";
        return output;
    }

    string TableView::getValueToString(ValueType& data, bool join) const{
        int index = data.index();
        string res;
        stringstream ss;
        int num = 0;
        float num1 = 0.0;
        bool num2 = false;

        switch(index){
            case 0:
                num = get<uint32_t>(data);
                if(join && num == 0){
                    res = "NULL";
                }
                else{
                    ss << num;  ss >> res;
                }
                return res;
            case 1:
                num1 = get<float>(data);
                if(join && num1 == 0.0){
                    res = "NULL";
                }
                else{
                    ss << num1;  ss >> res;
                }
                return res;
            case 2:
                num2 = get<bool>(data);
                if(join && num2 == false){
                    res = "NULL";
                }
                else{
                    res = num2 == false ? "false" : "true";
                }
                return res;
            case 3:
                if(join && get<string>(data) == ""){
                    return "NULL";
                }
                return get<string>(data);
            default:
                return res;
        }
    }



    StatusResult TableView::addShowName(vector<string>& aShowName){
        showName = aShowName;
        return StatusResult();
    }

    vector<string> TableView::getShowName() const{
        return showName;
    }

    TableView& TableView::setIsStar(bool star){
        isStar = star;
        return * this;
    }




}
