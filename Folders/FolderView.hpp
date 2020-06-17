//
// Created by Cao Zongheng on 4/13/20.
//

#ifndef ECE141_SP20_ASSIGNMENT2_ZONGHENG_CAO_FOLDERVIEW_HPP
#define ECE141_SP20_ASSIGNMENT2_ZONGHENG_CAO_FOLDERVIEW_HPP

#include "FolderReader.hpp"
#include "../Interface/View.hpp"


namespace ECE141{



    class FolderView : public View, public FolderListener{

    public:

        FolderView(FolderReader &theReader, const char *anExtension = ".db") :
        reader(theReader),extension(anExtension),stream(nullptr)
        {}

        bool operator()(const std::string &aString ){
            (*stream) << aString << "\n";
            return true;
        }

        ~FolderView(){

        }

        virtual bool    show(std::ostream &aStream){
            stream = &aStream;
            aStream << "Showing databases: \n";
            reader.each(*this,".db");
            return true;
        }

        const FolderReader &reader;
        const char *extension;
        std::ostream    *stream;
    };







}







#endif //ECE141_SP20_ASSIGNMENT2_ZONGHENG_CAO_FOLDERVIEW_HPP





