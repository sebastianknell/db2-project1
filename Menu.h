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
    static class_type workingClass;
    static SequentialFile sequentialFile;
    static void select_working_file();
    static void select_option();
    static void search();
    static void range_search();
    static void insert();
    static void remove();
public:
    static void execute();
};


#endif //DB2_PROJECT1_MENU_H
