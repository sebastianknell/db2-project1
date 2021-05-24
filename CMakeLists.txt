cmake_minimum_required(VERSION 3.19)
project(db2_project1)

set(CMAKE_CXX_STANDARD 17)

add_executable(db2_project1 main.cpp SequentialFile.cpp SequentialFile.h utils/CSV_Reader.cpp utils/CSV_Reader.h utils/Buffer.cpp utils/Buffer.h)