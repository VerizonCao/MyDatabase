cmake_minimum_required(VERSION 3.15)
project(MyDatabase)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(
        MyDatabase
        AppProcessor.cpp
        Attribute.cpp
        CommandProcessor.cpp
        Database.cpp
        DBStatement.cpp
        DBManager.cpp
        main.cpp
        Schema.cpp
        SQLProcessor.cpp
        Interface/Statement.cpp
        Storage.cpp
        StorageBlock.cpp
        Tokenizer.cpp
        SQLStatement.cpp

        Row.cpp
        Filters.cpp
        Validator.cpp


        RowColloection.cpp RowColloection.h TableView.cpp TableView.h Index.cpp IndexManager.cpp IndexManager.h Cache.cpp Cache.h)
