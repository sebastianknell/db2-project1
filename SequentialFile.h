//
// Created by Sebastian Knell on 22/05/21.
//

#ifndef DB2_PROJECT1_SEQUENTIALFILE_H
#define DB2_PROJECT1_SEQUENTIALFILE_H

#include <iostream>
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
    void load_data(string filename);
    vector<Record> search(int key);
    vector<Record> range_search(int begin_key, int end_key);
    void insert(Record r);
    void remove(int key);
};


#endif //DB2_PROJECT1_SEQUENTIALFILE_H
