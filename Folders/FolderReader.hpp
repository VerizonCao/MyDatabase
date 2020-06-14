//
//  FolderReader.hpp
//  Database5
//
//  Created by rick gessner on 4/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef FolderReader_h
#define FolderReader_h

#include <string>
#include <filesystem>

using namespace std::filesystem;

namespace ECE141 {





  //interface
  class FolderListener {
  public:
    virtual bool operator()(const std::string &aName)=0;
  };

  
  class FolderReader {
  public:

                  FolderReader(const char *aPath){
                      std::string temp(aPath);
                      path = temp;
                  }
    virtual       ~FolderReader() {}
    
    virtual bool  exists(const std::string &aPath) {
                    //STUDENT: add logic to see if FOLDER at given path exists.
                    std::string temp = path;
                    std::filesystem::path str(temp + "/" + aPath + ".db");  //change string to path
                    directory_entry entry(str);   //entry
                    return std::filesystem::exists(str);
                  }

    virtual void  each(FolderListener &aListener, const std::string &anExtension) const {
                    std::filesystem::path thePath(path);
                    for(auto & theItem : std::filesystem::directory_iterator(path)){
                        if(!theItem.is_directory()){   //如果是文件本身
                            std::filesystem::path temp(theItem.path());
                            std::string theExt(temp.extension().string());
                            //如果没有后缀或者 后缀匹配 那么输出
                            if(0 == anExtension.size() || 0 == anExtension.compare(theExt)){
                                if(!aListener(temp.stem().string())) break;
                            }
                        }
                    }
                  };
    virtual bool  drop(const std::string aName) const {
                    std::string temp = path;
                    std::string filePath = temp + "/" + aName + ".db";
                    if(remove(filePath)){
                        return true;
                    }
                    return false;
                  };

    std::string path;
  };
  
}

#endif /* FolderReader_h */
