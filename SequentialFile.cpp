//
// Created by Sebastian Knell on 22/05/21.
//

#include "SequentialFile.h"

static long get_file_size(fstream &stream) {
    auto pos = stream.tellg();
    stream.seekg(0, ios::end);
    auto size = stream.tellg();
    stream.seekg(pos);
    return size;
}

static long find(int key, fstream &stream) {
    long record_size = sizeof(FixedRecord);
    long l = 0;
    long r = get_file_size(stream) / record_size;
    FixedRecord temp;
    while (r >= l) {
        long m = (l + r) / 2;
        stream.seekg(m * record_size);
        readRecord(temp, stream);
        if (temp.get_key() < key) l = m+1;
        else if (temp.get_key() > key) r = m-1;
        else {
            return m * record_size;
        }
    }
    return -1;
}

void print_record(FixedRecord &record) {
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
    FixedRecord temp;
    fstream out_file(data_file, ios::out | ios::binary);
    if (!out_file) return;
    rapidcsv::Document document(from_filename);
    auto len = document.GetRowCount();
    long offset = 0;
    for (int i = 0; i < len; i++) {
        vector<string> row = document.GetRow<string>(i);
        // Save row to record and write to file
        temp.load_data(row);
        writeRecord(temp, out_file, offset);
    }
    out_file.close();
}

void SequentialFile::print_all() {
    fstream in_file(data_file, ios::in | ios::binary);
    if (!in_file) return;
    bool success;
    FixedRecord temp;
    do {
        success = readRecord(temp, in_file);
        print_record(temp);
        cout << in_file.tellg() << " " << temp.next << endl;
    } while (success);
    in_file.close();
}

vector<FixedRecord> SequentialFile::search(int key) {
    vector<FixedRecord> records;
    fstream in_file(data_file, ios::in | ios::binary);
    FixedRecord temp;
    long pos = find(key, in_file);
    in_file.seekg(pos);
    readRecord(temp, in_file);
    while (temp.get_key() == key) {
        records.push_back(temp);
        readRecord(temp, in_file);
    }
    in_file.close();
    return records;
}

vector<FixedRecord> SequentialFile::range_search(int begin_key, int end_key) {
    return vector<FixedRecord>();
}

void SequentialFile::insert(FixedRecord record) {
    ofstream out_file(aux_file, ios::app | ios::binary);
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
    fstream data(data_file, ios::binary);
    fstream aux(aux_file, ios::binary);
    FixedRecord temp1;
    vector<FixedRecord> records;
    do {
        readRecord(temp1, data);
        records.push_back(temp1);
        if (temp1.f_type == file_type::aux) {
            // Read all aux records in chain
            aux.seekg(temp1.next);
            FixedRecord temp2;
            do {
                readRecord(temp2, aux);
                records.push_back(temp2);
                aux.seekg(temp2.next);
            } while (temp2.f_type == file_type::aux);
        }
    } while (data);

    long offset = 0;
    for (auto record : records) {
        record.f_type = file_type::data;
        writeRecord(record, data, offset);
    }
}

bool readRecord(FixedRecord &record, fstream &stream) {
    stream.read((char*)&record, sizeof(record));
    return stream.good();
}

bool writeRecord(FixedRecord &record, fstream &stream, long &offset) {
    offset += sizeof(record);
    record.next = offset;
    stream.write((char*)&record, sizeof(record));
    return stream.good();
}
