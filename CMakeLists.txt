cmake_minimum_required(VERSION 3.16.5)
project(db2_project1)

set(CMAKE_CXX_STANDARD 17)

add_executable(db2_project1 main.cpp SequentialFile.cpp SequentialFile.h utils/Buffer.cpp utils/Buffer.h test/Tester.cpp test/Tester.h Record.cpp Record.h ISAM.cpp ISAM.h Menu.h Menu.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/stroke-data.csv
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)