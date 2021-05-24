//
// Created by Sebastian Knell on 23/05/21.
//

#ifndef DB2_PROJECT1_CSV_READER_H
#define DB2_PROJECT1_CSV_READER_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "rapidcsv.h"

using namespace std;

class CSV_Reader {
    string filename;
public:
    explicit CSV_Reader(string filename = "data/stroke-data.csv"): filename(std::move(filename)) {};
    void load();
};


#endif //DB2_PROJECT1_CSV_READER_H
