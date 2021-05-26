//
// Created by Sebastian Knell on 22/05/21.
//

#ifndef DB2_PROJECT1_SEQUENTIALFILE_H
#define DB2_PROJECT1_SEQUENTIALFILE_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include "utils/rapidcsv.h"
#include "utils/Buffer.h"
#include "Record.h"

using namespace std;

class SequentialFile {
    string data_file;
    string aux_file = "aux.dat";
public:
    explicit SequentialFile(string data_file = "seq-file-data.dat"): data_file(std::move(data_file)) {};
    void load_data(string);
    void print_all();
    vector<Record> search(int);
    vector<Record> range_search(int, int);
    void insert(Record);
    void remove(int);
};


#endif //DB2_PROJECT1_SEQUENTIALFILE_H
