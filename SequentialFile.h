//
// Created by Sebastian Knell on 22/05/21.
//

#ifndef DB2_PROJECT1_SEQUENTIALFILE_H
#define DB2_PROJECT1_SEQUENTIALFILE_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <optional>
#include <cstring>
#include "utils/rapidcsv.h"

using namespace std;

enum file_type {
    data,
    aux,
    none
};

struct location_type {
    long position{};
    file_type fileType{};
};

struct FixedRecord {
    char id[7]{}; // unique
    char gender[7]{};
    char age[4]{};
    char hypertension[2]{};
    char heart_disease[2]{};
    char ever_married[4]{};
    char work_type[14]{};
    char residence_type[6]{};
    char avg_glucose[7]{};
    char bmi[7]{};
    char smoking_status[16]{};
    char stroke[2]{};
    file_type next_file_type = file_type::data;
    long next{};

    void load_data(vector<string> data) {
        assert(data.size() == 12);
        strcpy(id, data.at(0).c_str());
        strcpy(gender, data.at(1).c_str());
        strcpy(age, data.at(2).c_str());
        strcpy(hypertension, data.at(3).c_str());
        strcpy(heart_disease, data.at(4).c_str());
        strcpy(ever_married, data.at(5).c_str());
        strcpy(work_type, data.at(6).c_str());
        strcpy(residence_type, data.at(7).c_str());
        strcpy(avg_glucose, data.at(8).c_str());
        strcpy(bmi, data.at(9).c_str());
        strcpy(smoking_status, data.at(10).c_str());
        strcpy(stroke, data.at(11).c_str());
    }

    int get_key() const {return stoi(id);}
};

void print_record(FixedRecord &);
static bool readRecord(FixedRecord &, fstream&);
static bool writeRecord(FixedRecord &, fstream&);
extern int countReadSeq;
extern int countWriteSeq;

class SequentialFile {
    string data_file;
    string aux_file = "aux.dat";
    int max_aux_size;
public:
    explicit SequentialFile(string data_file = "seq-file-data.dat", int max_aux_size = 10):
        data_file(std::move(data_file)), max_aux_size(max_aux_size) {};
    void load_data(const string&);
    void print_all();
    optional<FixedRecord> search(int);
    vector<FixedRecord> range_search(int, int);
    bool insert(FixedRecord);
    bool remove(int);
    void merge_data();
};


#endif //DB2_PROJECT1_SEQUENTIALFILE_H
