//
// Created by Sebastian Knell on 25/05/21.
//

#ifndef DB2_PROJECT1_TESTER_H
#define DB2_PROJECT1_TESTER_H

#include "../utils/Buffer.h"
#include "../Record.h"

class Person {
public:
    char last_name[11];
    char first_name[11];
    char address[16];

    Person() {
        last_name[0] = 0;
        first_name[0] = 0;
        address[0] = 0;
    }

    bool pack(Buffer &buffer) {
        bool result;
        buffer.clear();
        result = buffer.pack(last_name);
        result = result && buffer.pack(first_name);
        result = result && buffer.pack(address);
        return result;
    }

    bool unpack(Buffer &buffer) {
        bool result;
        result = buffer.unpack(last_name);
        result = result && buffer.unpack(first_name);
        result = result && buffer.unpack(address);
        return result;
    }

    void print() {
        cout << "Last Name: " << last_name << endl;
        cout << "First Name: " << first_name << endl;
        cout << "Address: " << address << endl;
    }
};

class Tester {
public:
    static bool buffer_test();
    static bool record_test();
};


#endif //DB2_PROJECT1_TESTER_H
