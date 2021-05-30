//
// Created by Sebastian Knell on 25/05/21.
//

#ifndef DB2_PROJECT1_RECORD_H
#define DB2_PROJECT1_RECORD_H

#include <iostream>
#include <vector>
#include "utils/Buffer.h"

class Record {
public:
    long next;
    vector<string> data;
    void pack(Buffer &buffer);
    void unpack(Buffer &buffer);
    void print();
    void load_data(vector<string>);
    int get_key() {return stoi(data[0]);}
};


#endif //DB2_PROJECT1_RECORD_H
