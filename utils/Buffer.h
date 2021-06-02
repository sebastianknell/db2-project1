//
// Created by Sebastian Knell on 23/05/21.
//

#ifndef DB2_PROJECT1_BUFFER_H
#define DB2_PROJECT1_BUFFER_H

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class Buffer {
    char delimeter;
    char* buffer;
    int buffer_size;
    int max_bytes;
    int next_byte;
public:
    Buffer(char delim = '|', int max_b = 1000);
    ~Buffer() {delete[] buffer;}
    void clear();
    bool read(ifstream &);
    bool write(ofstream &) const;
    bool pack(const char*, int size = -1);
    bool unpack(char*);
    void print_buffer();
    int get_buffer_size() const {return buffer_size;}
    unsigned long get_buffer_length() const {return strlen(buffer);}
    int get_max_bytes() const {return max_bytes;}
};


#endif //DB2_PROJECT1_BUFFER_H
