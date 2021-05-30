//
// Created by Sebastian Knell on 22/05/21.
//

#include "SequentialFile.h"

static void print_record(FixedRecord &record) {
    cout << record.id << " ";
    cout << record.gender << " ";
    cout << record.age << " ";
    cout << record.hypertension << " ";
    cout << record.heart_disease << " ";
    cout << record.ever_married << " ";
    cout << record.work_type << " ";
    cout << record.residence_type << " ";
    cout << record.avg_glucose << " ";
    cout << record.bmi << " ";
    cout << record.smoking_status << " ";
    cout << record.stroke << " ";
    cout << endl;
}

void SequentialFile::load_data(const string& from_filename) {
//    FixedRecord temp;
    ofstream out_file(data_file, ios::out | ios::binary);
    if (!out_file) return;
    rapidcsv::Document document(from_filename);
    auto len = document.GetRowCount();
    long offset = 0;
    for (int i = 0; i < len; i++) {
        vector<string> row = document.GetRow<string>(i);
        // Save row to record and write to file
        FixedRecord temp;
        temp.load_data(row);
        writeRecord(temp, out_file, offset);
    }
    out_file.close();
}

void SequentialFile::print_all() {
    ifstream in_file(data_file, ios::in | ios::binary);
    if (!in_file) return;
    bool success;
    do {
        FixedRecord temp;
        success = readRecord(temp, in_file);
        print_record(temp);
        cout << in_file.tellg() << " " << temp.next << endl;
    } while (success);
    in_file.close();
}

vector<FixedRecord> SequentialFile::search(int key) {
    vector<FixedRecord> records;
    FixedRecord temp;
    ifstream in_file(data_file, ios::in | ios::binary);
    while (readRecord(temp, in_file)) {
        if (temp.get_key() == key) records.push_back(temp);
        if (temp.get_key() > key) break;
    }
    in_file.close();
    return records;
}

vector<FixedRecord> SequentialFile::range_search(int begin_key, int end_key) {
    return vector<FixedRecord>();
}

void SequentialFile::insert(FixedRecord record) {
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

bool SequentialFile::readRecord(FixedRecord &record, ifstream &stream) {
    /*Buffer buffer;
    bool success = buffer.read(stream);
    if (!success) return false;
    record.unpack(buffer);
    stream.read((char*)&record.f_type, sizeof(record.f_type));
    stream.read((char*)&record.next, sizeof(record.next));*/
    stream.read((char*)&record, sizeof(record));
    return stream.good();
}

bool SequentialFile::writeRecord(FixedRecord &record, ofstream &stream, long &offset) {
    /*Buffer buffer;
    record.pack(buffer);
    bool success = buffer.write(stream);
    if (!success) return false;
    offset += buffer.get_buffer_size() + sizeof(int)*2 + sizeof(unsigned long); // TODO refactor
    stream.write((char*)&record.f_type, sizeof(record.f_type));
    stream.write((char*)&offset, sizeof(offset));*/

    record.f_type = file_type::data;
    offset += sizeof(record);
    record.next = offset;
    stream.write((char*)&record, sizeof(record));
    return stream.good();
}
