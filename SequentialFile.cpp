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

static location_type find(int key, fstream &data, fstream &aux) {
    long record_size = sizeof(FixedRecord);
    long l = 0;
    long r = (get_file_size(data) / record_size) - 1;
    long pos;
    FixedRecord temp;
    while (r > l) {
        long m = (l + r) / 2;
        pos = m * record_size;
        data.seekg(m * record_size);
        readRecord(temp, data);
        if (temp.get_key() < key) l = m+1;
        else if (temp.get_key() > key) r = m-1;
        else {
            return {pos, file_type::data};
        }
    }
    while (temp.get_key() < key) {
        pos += record_size;
        data.seekg(pos);
        readRecord(temp, data);
    }
    while (temp.get_key() > key) {
        pos -= record_size;
        data.seekg(pos);
        readRecord(temp, data);
    }
    if (temp.next_file_type == file_type::aux) {
        bool success = true;
        while (temp.get_key() < key && temp.next_file_type == file_type::aux && success) {
            pos = temp.next;
            aux.seekg(pos);
            success = readRecord(temp, aux);
        }
        return {pos, file_type::aux};
    }
    return {pos, file_type::data};
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
    fstream aux(aux_file, ios::in | ios::binary);
    FixedRecord temp;
    auto loc = find(key, data, aux);
    (loc.fileType == file_type::data ? data : aux).seekg(loc.position);
    readRecord(temp, loc.fileType == file_type::data ? data : aux);
    data.close();
    aux.close();
    if (temp.get_key() == key) return temp;
    return nullopt;
}

vector<FixedRecord> SequentialFile::range_search(int begin_key, int end_key) {
    fstream data(data_file, ios::in | ios::binary);
    fstream aux(data_file, ios::in | ios::binary);
    vector<FixedRecord> records;
    auto pos = find(begin_key, data, aux);
    FixedRecord temp;
    readRecord(temp, temp.next_file_type == file_type::data ? data : aux);
    
}

bool SequentialFile::insert(FixedRecord record) {
    fstream data(data_file, ios::out | ios::in | ios::binary);
    fstream aux(aux_file, ios::out | ios::in | ios::binary);
    if (!data || !aux) {
        return false;
    }
    // Search location
    auto loc = find(record.get_key(), data, aux);
    (loc.fileType == file_type::data ? data : aux).seekg(loc.position);
    FixedRecord temp;
    readRecord(temp, loc.fileType == file_type::data ? data : aux);
    // Return if key is already in file
    if (temp.get_key() == record.get_key()) return false;
    // Insert
    // Save temp pointers
    auto next_file_type = temp.next_file_type;
    auto next = temp.next;
    // Update temp pointers and write
    temp.next_file_type = file_type::aux;
    temp.next = aux.tellg();
    data.seekg(loc.position);
    writeRecord(temp, data); // TODO validate
    // Write new record
    record.next_file_type = next_file_type;
    record.next = next;
    writeRecord(record, aux); // TODO validate

    data.close();
    aux.close();

    if (aux.tellp() >= max_aux_size) {
        merge_data();
    }
    return true;
}

bool SequentialFile::remove(int key) {

    
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
    bool success = stream.good();
    return success;
}

bool writeRecord(FixedRecord &record, fstream &stream) {
    stream.write((char*)&record, sizeof(record));
    return stream.good();
}
