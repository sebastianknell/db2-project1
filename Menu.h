//
// Created by Sebastian Knell on 1/06/21.
//

#ifndef DB2_PROJECT1_MENU_H
#define DB2_PROJECT1_MENU_H

#include <iostream>
#include <vector>
#include <optional>
#include "SequentialFile.h"

enum class_type {
    sequential,
    isam
};

class Menu {
    class_type workingClass;
    SequentialFile sequentialFile;
    void select_working_file();
    void select_option();
    void search();
    void range_search();
    void insert();
    void remove();
public:
    void execute();
};


#endif //DB2_PROJECT1_MENU_H