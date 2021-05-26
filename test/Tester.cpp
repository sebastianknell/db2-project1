//
// Created by Sebastian Knell on 25/05/21.
//

#include "Tester.h"

bool Tester::buffer_test() {
    Person p1;
    strcpy(p1.last_name, "Knell");
    strcpy(p1.first_name, "Sebastian");
    strcpy(p1.address, "Avenida 1234");

    Buffer buffer;
    bool success;
    ofstream test_out("buffer_test.dat", ios::out | ios::binary);
    if (!test_out.good()) return false;
    p1.pack(buffer);
    success = buffer.write(test_out);
    if (!success) return false;
    strcpy(p1.first_name, "Daniel");
    p1.pack(buffer);
    success = buffer.write(test_out);
    if (!success) return false;
    test_out.close();

    Person p2;
    ifstream test_in("buffer_test.dat", ios::in | ios::binary);
    if (!test_in) return false;
    Buffer buffer_in;
    success = buffer_in.read(test_in);
    if (!success) return false;
    p2.unpack(buffer_in);
    p2.print();
    success = buffer_in.read(test_in);
    if (!success) return false;
    p2.unpack(buffer_in);
    p2.print();
    test_in.close();
    return success;
}

bool Tester::record_test() {
    Record r1;
    r1.data.emplace_back("Knell");
    r1.data.emplace_back("Sebastian");
    Buffer buffer;
    bool success;
    ofstream test_out("record_test.dat", ios::out | ios::binary);
    if (!test_out.good()) return false;
    r1.pack(buffer);
    success = buffer.write(test_out);
    if (!success) return false;
    r1.data[1] = "Daniel";
    r1.pack(buffer);
    success = buffer.write(test_out);
    if (!success) return false;
    test_out.close();

    Record r2;
    ifstream test_in("a.dat", ios::in | ios::binary);
    if (!test_in) return false;
    Buffer buffer_in;
    success = buffer_in.read(test_in);
    if (!success) return false;
    r2.unpack(buffer_in);
    r2.print();
    success = buffer_in.read(test_in);
    if (!success) return false;
    r2.unpack(buffer_in);
    r2.print();
    test_in.close();
    return success;
}