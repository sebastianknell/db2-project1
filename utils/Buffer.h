//
// Created by Sebastian Knell on 23/05/21.
//

#ifndef DB2_PROJECT1_BUFFER_H
#define DB2_PROJECT1_BUFFER_H

#include <iostream>
#include <fstream>

using namespace std;

class Buffer {
    char delimeter;
    char* buffer;
    int buffer_size;
    int max_bytes;
    int next_byte;
public:
    Buffer(char delim = '|', int max_b = 1000);
    void clear();
    bool read(ifstream &);
    bool write(ofstream &) const;
    bool pack(const char*, int size = -1);
    bool unpack(char*);
    void print_buffer();
};


#endif //DB2_PROJECT1_BUFFER_H
