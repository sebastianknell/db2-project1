//
// Created by Sebastian Knell on 22/05/21.
//

#include "SequentialFile.h"

void SequentialFile::load_data(const string& from_filename) {
    Record temp;
    ofstream out_file(data_file, ios::out | ios::binary);
    if (!out_file) return;
    rapidcsv::Document document(from_filename);
    auto len = document.GetRowCount();
    unsigned long offset = 0;
    for (int i = 0; i < len; i++) {
        vector<string> row = document.GetRow<string>(i);
        // Save row to record and write to file
        temp.load_data(row);
        writeRecord(temp, out_file, offset);
    }
    out_file.close();
}

void SequentialFile::print_all() {
    Record temp;
    ifstream in_file(data_file, ios::in | ios::binary);
    if (!in_file) return;
    while (readRecord(temp, in_file)) {
        temp.print();
        cout << in_file.tellg() << " " << temp.next << endl;
    }
    in_file.close();
}

vector<Record> SequentialFile::search(int key) {
    vector<Record> records;
    Record temp;
    ifstream in_file(data_file, ios::in | ios::binary);
    while (readRecord(temp, in_file)) {
        if (temp.get_key() == key) records.push_back(temp);
        if (temp.get_key() > key) break;
    }
    in_file.close();
    return records;
}

vector<Record> SequentialFile::range_search(int begin_key, int end_key) {
    return vector<Record>();
}

void SequentialFile::insert(Record record) {
    ofstream out_file(aux_file, ios::out | ios::binary);
    if (!out_file) return;
    // Search position
    // Insert
    if (out_file.tellp() >= max_aux_size) {
        merge_data();
    }
}

void SequentialFile::remove(int key) {

}

// Merge data file and aux file
void SequentialFile::merge_data() {
    
}

bool SequentialFile::readRecord(Record &record, ifstream &stream) {
    Buffer buffer;
    bool success = buffer.read(stream);
    if (!success) return false;
    record.unpack(buffer);
    stream.read((char*)&record.f_type, sizeof(record.f_type));
    stream.read((char*)&record.next, sizeof(record.next));
    return stream.good();
}

bool SequentialFile::writeRecord(Record &record, ofstream &stream, unsigned long &offset) {
    Buffer buffer;
    record.pack(buffer);
    bool success = buffer.write(stream);
    if (!success) return false;
    offset += buffer.get_buffer_size() + sizeof(int)*2 + sizeof(unsigned long); // TODO refactor
    stream.write((char*)&record.f_type, sizeof(record.f_type));
    stream.write((char*)&offset, sizeof(offset));
    return stream.good();
}
