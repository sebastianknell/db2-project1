//
// Created by Sebastian Knell on 24/05/21.
//

#include "../utils/Buffer.h"

class Person {
public:
    char last_name[11];
    char first_name[11];
    char address[16];

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
        buffer.clear();
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

int main() {
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
    p2.unpack(buffer_in);
    p2.print();
    test_in.close();

    return 0;
}