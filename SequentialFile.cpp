//
// Created by Sebastian Knell on 22/05/21.
//

#include "SequentialFile.h"

int countReadSeq = 0;
int countWriteSeq = 0;

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
    bool success = true;
    while (temp.get_key() < key && success) {
        pos += record_size;
        data.seekg(pos);
        success = readRecord(temp, data);
    }
    success = true;
    while (temp.get_key() > key && success) {
        pos -= record_size;
        data.seekg(pos);
        success = readRecord(temp, data);
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
    cout << endl;
}

void SequentialFile::load_data(const string& from_filename) {
    FixedRecord temp;
    fstream data(data_file, ios::out | ios::binary);
    fstream aux(aux_file, ios::out | ios::binary);
    if (!data || !aux) return;
    rapidcsv::Document document(from_filename);
    auto len = document.GetRowCount();
    long offset = 0;
    for (int i = 0; i < len; i++) {
        vector<string> row = document.GetRow<string>(i);
        // Save row to record and write to file
        temp.load_data(row);
        offset += sizeof(temp);
        temp.next_file_type = i == len-1 ? file_type::none : file_type::data;
        temp.next = offset;
        writeRecord(temp, data);
    }
    data.close();
    aux.close();
}

void SequentialFile::print_all() {
    fstream data(data_file, ios::in | ios::binary);
    fstream aux(aux_file, ios::in | ios::binary);
    if (!data || !aux) return;
    FixedRecord temp;
    readRecord(temp, data);
    print_record(temp);
    (temp.next_file_type == file_type::data ? data : aux).seekg(temp.next);
    while (readRecord(temp, temp.next_file_type == file_type::data ? data : aux)) {
        print_record(temp);
        (temp.next_file_type == file_type::data ? data : aux).seekg(temp.next);
    }
    data.close();
}

optional<FixedRecord> SequentialFile::search(int key) {
    fstream data(data_file, ios::in | ios::binary);
    fstream aux(aux_file, ios::in | ios::binary);
    if (!data || !aux) return nullopt;
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

    auto loc = find(begin_key, data, aux);
    FixedRecord temp;
    (loc.fileType == file_type::data ? data : aux).seekg(loc.position);
    readRecord(temp, loc.fileType == file_type::data ? data : aux);
    if (temp.get_key() >= begin_key) records.push_back(temp);

    while (true) {
        (temp.next_file_type == file_type::data ? data : aux).seekg(temp.next);
        readRecord(temp, temp.next_file_type == file_type::data ? data : aux);
        if (temp.get_key() <= end_key) records.push_back(temp);
        else break;
    }
    return records;
}

bool SequentialFile::insert(FixedRecord record) {
    if (record.get_key() <= 0) return false;
    fstream data(data_file, ios::out | ios::in | ios::binary);
    fstream aux(aux_file, ios::out | ios::in | ios::binary);
    if (!data || !aux) {
        return false;
    }
    // Search location
    auto loc = find(record.get_key(), data, aux);
    (loc.fileType == file_type::data ? data : aux).seekg(loc.position);
    FixedRecord temp;
    bool success = readRecord(temp, loc.fileType == file_type::data ? data : aux);
    if (!success) return false;
    // Return if key is already in file
    if (temp.get_key() == record.get_key()) return false;
    // Insert
    // Save temp pointers
    auto next_file_type = temp.next_file_type;
    auto next = temp.next;
    // Update temp pointers and write
    temp.next_file_type = file_type::aux;
    temp.next = aux.tellg();
    (loc.fileType == file_type::data ? data : aux).seekg(loc.position);
    success = writeRecord(temp, (loc.fileType == file_type::data ? data : aux));
    if (!success) return false;
    // Write new record
    record.next_file_type = next_file_type;
    record.next = next;
    success = writeRecord(record, aux); // where is aux?
    if (!success) return false;

    if (aux.tellg() >= max_aux_size * sizeof(FixedRecord)) {
        data.close();
        aux.close();
        merge_data();
    }
    return true;
}

bool SequentialFile::remove(int key) {
    if (key <= 0) return false;
    fstream data(data_file, ios::out | ios::in | ios::binary);
    fstream aux(aux_file, ios::out | ios::in | ios::binary);
    // Search location
    auto loc = find(key, data, aux);
    (loc.fileType == file_type::data ? data : aux).seekg(loc.position);
    FixedRecord temp;
    bool success = readRecord(temp, loc.fileType == file_type::data ? data : aux);
    if (!success) return false;
    // Return if key doesn't exists
    if (temp.get_key() != key) return false;
    // Mark record as removed
    auto next_file_type = temp.next_file_type;
    auto next = temp.next;
    temp.next = -1;
    (loc.fileType == file_type::data ? data : aux).seekg(loc.position);
    success = writeRecord(temp, loc.fileType == file_type::data ? data : aux);
    if (!success) return false;
    // Update record before
    auto prev_loc = find(key-1, data, aux);
    (prev_loc.fileType == file_type::data ? data : aux).seekg(prev_loc.position);
    success = readRecord(temp, prev_loc.fileType == file_type::data ? data : aux);
    if (!success) return false;
    temp.next_file_type = next_file_type;
    temp.next = next;
    (prev_loc.fileType == file_type::data ? data : aux).seekg(prev_loc.position);
    success = writeRecord(temp, prev_loc.fileType == file_type::data ? data : aux);
    if (!success) return false;
    return true;
}

// Merge data file and aux file
void SequentialFile::merge_data() {
    fstream data(data_file, ios::out | ios::in | ios::binary);
    fstream aux(aux_file, ios::out | ios::in | ios::binary);
    FixedRecord temp1;
    readRecord(temp1, data);
    // Skip records before pointer to aux
    while (temp1.next_file_type == file_type::data) {
        data.seekg(temp1.next);
        readRecord(temp1, data);
    }
    auto offset = data.tellg();
    offset -= sizeof(FixedRecord);
    vector<FixedRecord> records;
    while (temp1.next_file_type != file_type::none) {
        records.push_back(temp1);
        (temp1.next_file_type == file_type::data ? data : aux).seekg(temp1.next);
        readRecord(temp1, temp1.next_file_type == file_type::data ? data : aux);
    }

    data.seekg(offset);
    for (auto record : records) {
        record.next_file_type = file_type::data;
        offset += sizeof(record);
        record.next = offset;
        writeRecord(record, data);
    }
    data.close();
    aux.close();
    aux.open(aux_file, fstream ::out | fstream::trunc);
    aux.close();
}

bool readRecord(FixedRecord &record, fstream &stream) {
    stream.read((char*)&record, sizeof(record));
    bool success = stream.good();
    countReadSeq++;
    return success;
}

bool writeRecord(FixedRecord &record, fstream &stream) {
    stream.write((char*)&record, sizeof(record));
    countWriteSeq++;
    return stream.good();
}
