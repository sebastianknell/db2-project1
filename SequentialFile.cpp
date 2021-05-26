//
// Created by Sebastian Knell on 22/05/21.
//

#include "SequentialFile.h"

void SequentialFile::load_data(string from_filename) {
    Record temp;
    Buffer buffer;
    ofstream out_file(data_file, ios::out | ios::binary);
    if (!out_file) return;
    rapidcsv::Document document(from_filename);
    auto len = document.GetRowCount();
    for (int i = 0; i < len; i++) {
        vector<string> row = document.GetRow<string>(i);
        // Save row to record and write to file
        temp.load_data(row);
        temp.pack(buffer);
        buffer.write(out_file);
    }
    out_file.close();
}

void SequentialFile::print_all() {
    Record temp;
    Buffer buffer;
    ifstream in_file(data_file, ios::in | ios::binary);
    if (!in_file) return;
    while (buffer.read(in_file)) {
        temp.unpack(buffer);
        temp.print();
    }
    in_file.close();
}

vector<Record> SequentialFile::search(int key) {
    return vector<Record>();
}

vector<Record> SequentialFile::range_search(int begin_key, int end_key) {
    return vector<Record>();
}

void SequentialFile::insert(Record r) {

}

void SequentialFile::remove(int key) {

}
