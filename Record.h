//
// Created by Sebastian Knell on 25/05/21.
//

#ifndef DB2_PROJECT1_RECORD_H
#define DB2_PROJECT1_RECORD_H

#include <iostream>
#include <vector>
#include "utils/Buffer.h"

enum file_type {
    data,
    aux
};

class Record {
    /*char id[7];
    char gender[6];
    char age[3];
    char hypertension[1];
    char heart_disease[1];
    char ever_married[3];
    char self_employed[10];*/
public:
    long next;
    file_type f_type;
    Record(): next(0), f_type(file_type::data) {}
    vector<string> data;
    void pack(Buffer &buffer);
    void unpack(Buffer &buffer);
    void print();
    void load_data(vector<string>);
    int get_key() {return stoi(data[0]);}
};


#endif //DB2_PROJECT1_RECORD_H
