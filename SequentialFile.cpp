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
    long r = (get_file_size(stream) / record_size) - 1;
    long pos;
    FixedRecord temp;
    while (r > l) {
        long m = (l + r) / 2;
        pos = m * record_size;
        stream.seekg(m * record_size);
        readRecord(temp, stream);
        if (temp.get_key() < key) l = m+1;
        else if (temp.get_key() > key) r = m-1;
        else {
            return pos;
        }
    }
    // TODO check if key is in aux file
    return pos;
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
    cout << record.next;
    cout << endl;
}

void SequentialFile::load_data(const string& from_filename) {
    FixedRecord temp;
    fstream data(data_file, ios::out | ios::binary);
    if (!data) return;
    rapidcsv::Document document(from_filename);
    auto len = document.GetRowCount();
    long offset = 0;
    for (int i = 0; i < len; i++) {
        vector<string> row = document.GetRow<string>(i);
        // Save row to record and write to file
        temp.load_data(row);
        offset += sizeof(temp);
        temp.next = i == len-1 ? -1 : offset;
        writeRecord(temp, data);
    }
    data.close();
}

void SequentialFile::print_all() {
    fstream data(data_file, ios::in | ios::binary);
    if (!data) return;
    FixedRecord temp;
    while (readRecord(temp, data)) print_record(temp);
    data.close();
}

optional<FixedRecord> SequentialFile::search(int key) {
    fstream data(data_file, ios::in | ios::binary);
    fstream aux(data_file, ios::in | ios::binary);
    FixedRecord temp;
    long pos = find(key, data);
    data.seekg(pos);
    readRecord(temp, data);
    // Record may be in aux file
    if (temp.get_key() == key) {
        data.close();
        aux.close();
        return temp;
    }
    else if (temp.next_file_type == file_type::aux) {
        while (temp.get_key() < key && temp.next_file_type == file_type::aux) {
             aux.seekg(temp.next);
             readRecord(temp, aux);
        }
        if (temp.get_key() == key) {
            data.close();
            aux.close();
            return temp;
        }
    }
    data.close();
    aux.close();
    return nullopt;
}

vector<FixedRecord> SequentialFile::range_search(int begin_key, int end_key) {
    return vector<FixedRecord>();
}

void SequentialFile::insert(FixedRecord record) {
    fstream data(data_file, ios::binary);
    fstream aux(aux_file, ios::app | ios::binary);
    if (!data || !aux) return;
    // Search position
    auto pos = find(record.get_key(), data);
    data.seekg(pos);
    FixedRecord temp;
    readRecord(temp, data);
    // Return if key is already in file
    if (temp.get_key() == record.get_key()) return;

    // Insert
    temp.next_file_type = file_type::aux;
    temp.next = aux.tellg();
    data.seekg(pos);
    writeRecord(temp, data);

    record.next_file_type = file_type::aux;
    record.next = data.tellg();
    writeRecord(record, aux);

    if (aux.tellp() >= max_aux_size) {
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
        if (temp1.next_file_type == file_type::aux) {
            // Read all aux records in chain
            aux.seekg(temp1.next);
            FixedRecord temp2;
            do {
                readRecord(temp2, aux);
                records.push_back(temp2);
                aux.seekg(temp2.next);
            } while (temp2.next_file_type == file_type::aux);
        }
    } while (data);

    long offset = 0;
    for (auto record : records) {
        record.next_file_type = file_type::data;
        offset += sizeof(record);
        record.next = offset;
        writeRecord(record, data);
    }
}

bool readRecord(FixedRecord &record, fstream &stream) {
    stream.read((char*)&record, sizeof(record));
    return stream.good();
}

bool writeRecord(FixedRecord &record, fstream &stream) {
    stream.write((char*)&record, sizeof(record));
    return stream.good();
}
