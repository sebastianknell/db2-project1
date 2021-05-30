//
// Created by Sebastian Knell on 25/05/21.
//

#include "Record.h"

#include <utility>

void Record::pack(Buffer &buffer) {
    buffer.clear();
//    string next_str = to_string(next);
//    data.push_back(next_str);
    for (const auto &field : data) {
        buffer.pack(field.data());
    }
}

void Record::unpack(Buffer &buffer) {
    char* temp = new char[buffer.get_max_bytes()];
    if (data.empty()) {
        while (buffer.unpack(temp)) {
            string field_str(temp);
            data.push_back(field_str);
        }
    } else {
        for (auto &field : data) {
            if (buffer.unpack(temp)) {
                string field_str(temp);
                field = field_str;
            }
            else throw runtime_error("Couldn't unpack buffer");
        }
    }
//    next = stol(data.back());
    delete[] temp;
}

void Record::print() {
    for (auto &field : data) cout << field << " ";
    cout << endl;
}

void Record::load_data(vector<string> new_data) {
    data = std::move(new_data);
}
