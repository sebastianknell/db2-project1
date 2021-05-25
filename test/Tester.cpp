//
// Created by Sebastian Knell on 25/05/21.
//

#include "Tester.h"

int Tester::buffer_test() {
    Person p1;
    strcpy(p1.last_name, "Knell");
    strcpy(p1.first_name, "Sebastian");
    strcpy(p1.address, "Avenida 1234");

    Buffer buffer;
    ofstream test_out("deltext.dat", ios::out | ios::binary);
    p1.pack(buffer);
    buffer.write(test_out);
    strcpy(p1.first_name, "Daniel");
    p1.pack(buffer);
    buffer.write(test_out);

    Person p2;
    ifstream test_in("deltext.dat", ios::in | ios::binary);
    Buffer buffer_in;
    buffer_in.read(test_in);
    p1.unpack(buffer_in);
    p1.print();
    buffer_in.read(test_in);
    p1.unpack(buffer_in);
    p1.print();
    test_in.close();
    return 0;
}
