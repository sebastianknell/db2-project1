//
// Created by Massimo on 30-May-21.
//

#include "ISAM.h"

static void printIndexRecord(IndexRecord &indexRecord) {
    cout << indexRecord.id << " " << indexRecord.pos << endl;
}

void ISAM::loadData(const string& fromFilename) {
    Record temp;
    ofstream data(dataFile, ios::out | ios::binary);
    ofstream index(indexFile, ios::out | ios::binary);
    if(!data) return;
    rapidcsv::Document document(fromFilename);
    auto len = document.GetRowCount();
    unsigned long offset = 0;
    IndexRecord indexRecord;

    for(int i = 0 ; i < len ; i++)
    {
        vector<string> row = document.GetRow<string>(i);
        temp.load_data(row);
        indexRecord.pos = data.tellp();
        writeRecord(temp, data, offset);
        indexRecord.id = temp.get_key();
        index.write((char*)&indexRecord, sizeof(indexRecord));
    }
}

void ISAM::printAll() {
    ifstream data(dataFile, ios::binary);
    ifstream index(indexFile, ios::binary);
    IndexRecord indexRecord;
    Record record;
    while (readIndex(indexRecord, index)) {
        data.seekg(indexRecord.pos);
        bool success = readRecord(record, data);
        if (!success) break; // TODO change
        printIndexRecord(indexRecord);
        record.print();
    }
}

bool ISAM::readIndex(IndexRecord &indexRecord, ifstream &stream) {
    stream.read((char*)&indexRecord, sizeof(indexRecord));
    return stream.good();
}

bool ISAM::writeIndex(IndexRecord &indexRecord, ofstream &stream) {
    stream.write((char*)&indexRecord, sizeof(indexRecord));
    return stream.good();
}

bool ISAM::readRecord(Record &record, ifstream &stream) {
    Buffer buffer;
    bool success = buffer.read(stream);
    if (!success) return false;
    record.unpack(buffer);
    return success;
}

bool ISAM::writeRecord(Record &record, ofstream &stream, unsigned long &offset) {
    Buffer buffer;
    record.pack(buffer);
    bool success = buffer.write(stream);
    return success;
}