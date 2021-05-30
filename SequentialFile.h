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
    int max_aux_size; // max size of aux.dat in bytes
    bool readRecord(Record &, ifstream&);
    bool writeRecord(Record &, ofstream&, unsigned long&);
public:
    explicit SequentialFile(string data_file = "seq-file-data.dat", int max_aux_size = 1000): data_file(std::move(data_file)), max_aux_size(
            max_aux_size) {};
    void load_data(const string&);
    void print_all();
    vector<Record> search(int);
    vector<Record> range_search(int, int);
    void insert(Record);
    void remove(int);
    void merge_data();
};


#endif //DB2_PROJECT1_SEQUENTIALFILE_H
