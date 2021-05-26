//
// Created by Sebastian Knell on 25/05/21.
//

#include "Record.h"

void Record::pack(Buffer &buffer) {
    buffer.clear();
    for (const auto &field : data) {
        buffer.pack(field.data());
    }
}

void Record::unpack(Buffer &buffer) {
    char* temp = new char[1000];
    if (data.empty()) {
        while (buffer.unpack(temp)) {
            string field_str(temp);
            data.push_back(field_str);
        }
    } else {
        for (auto &field : data) {
            if (buffer.unpack(temp)) {
                string field_str(temp);
                data.push_back(field_str);
            }
            // throw error
        }
    }
    delete[] temp;
}

void Record::print() {
    for (auto &field : data) cout << field << " ";
}
