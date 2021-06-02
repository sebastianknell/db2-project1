//
// Created by Massimo on 30-May-21.
//

#include "ISAM.h"

static void printIndexRecord(IndexRecord &indexRecord) {
    cout << indexRecord.id << " " << indexRecord.pos << endl;
}

void ISAM::printIndex() {
    ifstream index(indexFile, ios::binary);
    IndexRecord indexRecord;
    while(readIndex(indexRecord, index)) {
        printIndexRecord(indexRecord);
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
        if (!success) break;
        record.print();
    }
}

long long ISAM::getFileSize(ifstream &stream) {
    auto pos = stream.tellg();
    stream.seekg(0, ios::end);
    auto size = stream.tellg();
    stream.seekg(pos);
    return size;
}

long long ISAM::getFileSize(fstream &stream) {
    auto pos = stream.tellg();
    stream.seekg(0, ios::end);
    auto size = stream.tellg();
    stream.seekg(pos);
    return size;
}

long long ISAM::find(int id, fstream& stream){
    long indexSize = sizeof(IndexRecord);
    long long l = 0;
    long long r = (getFileSize(stream) / indexSize) - 1;
    long long pos;
    IndexRecord indexRecord;

    while(r > l) {
        long long m = (r+l) / 2;
        pos = m * indexSize;
        stream.seekg(m*indexSize);
        readIndex(indexRecord, stream);
        if(indexRecord.id < id) l = m + 1;
        else if(indexRecord.id > id) r = m-1;
        else {
            return pos;
        }
    }
    while(indexRecord.id < id) {
        pos += indexSize;
        stream.seekg(pos);
        readIndex(indexRecord, stream);
    }
    while (indexRecord.id > id) {
        pos -= indexSize;
        stream.seekg(pos);
        readIndex(indexRecord, stream);
    }
    return pos;
}

int countReadIndex=0;
int countWriteIndex=0;
int countReadData=0;
int countWriteData=0;

bool ISAM::readIndex(IndexRecord &indexRecord, ifstream &stream) {
    stream.read((char*)&indexRecord, sizeof(indexRecord));
    countReadIndex++;
    return stream.good();
}

bool ISAM::readIndex(IndexRecord &indexRecord, fstream &stream) {
    stream.read((char*)&indexRecord, sizeof(indexRecord));
    countReadIndex++;
    return stream.good();
}

bool ISAM::writeIndex(IndexRecord &indexRecord, ofstream &stream) {
    stream.write((char*)&indexRecord, sizeof(indexRecord));
    countWriteIndex++;
    return stream.good();
}

bool ISAM::writeIndex(IndexRecord &indexRecord, fstream &stream) {
    stream.write((char*)&indexRecord, sizeof(IndexRecord));
    return stream.good();
}

bool ISAM::readRecord(Record &record, ifstream &stream) {
    Buffer buffer;
    bool success = buffer.read(stream);
    if (!success) return false;
    record.unpack(buffer);
    countReadData++;
    return success;
}

bool ISAM::writeRecord(Record &record, ofstream &stream, unsigned long &offset) {
    Buffer buffer;
    record.pack(buffer);
    bool success = buffer.write(stream);
    countWriteData++;
    return success;
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

void ISAM::insert(Record record) {
    ofstream data(dataFile, ios::ate | ios::app | ios::binary);
    fstream index(indexFile, ios::in | ios::out | ios::binary);
    if(!data || !index) return;
    IndexRecord indexRecord;
    vector<IndexRecord> rewrite;
    unsigned long offset = 0;
    int id = record.get_key();

    auto pos = find(id, index);
    index.seekg(0);
    while(index.tellg() <= pos) {
        readIndex(indexRecord, index);
        rewrite.push_back(indexRecord);
    }

    index.seekg(pos + sizeof(IndexRecord)); // located at insertion pos

    // iterate indexes
    indexRecord.pos = data.tellp();
    indexRecord.id = id;
    rewrite.push_back(indexRecord);
    while(readIndex(indexRecord, index)) {
        rewrite.push_back(indexRecord); // add to vector
    }

    writeRecord(record, data, offset);

    data.close();
    index.close();
    ofstream indexOut(indexFile, ios::out | ios::trunc | ios::binary);
    for(auto & i : rewrite) {
        writeIndex(i, indexOut);
    }
}

optional<Record> ISAM::search(int id){
    ifstream data(dataFile, ios::binary);
    ifstream index(indexFile, ios::binary);
    IndexRecord indexRecord;
    Record temp;
    long indexSize = sizeof(indexRecord);
    long long l = 0;
    long long r = (getFileSize(index) / indexSize) -1 ;
    while(r > l) {
        long long mid = (r+l) / 2;
        index.seekg(mid*indexSize);
        readIndex(indexRecord, index);
        if(indexRecord.id < id) l = mid+1;
        else if (indexRecord.id > id) r = mid-1;
        else {
            data.seekg(indexRecord.pos);
            if (readRecord(temp, data)) {
                data.close();
                index.close();
                return temp;
            }
            else return nullopt;
        }
    }
    readIndex(indexRecord, index);
    if(indexRecord.id == id) {
        data.seekg(indexRecord.pos);
        if(readRecord(temp, data)) {
            printIndexRecord(indexRecord);
            data.close();
            index.close();
            return temp;
        }
    } else return nullopt;
    return nullopt;
}

vector<Record> ISAM::rangeSearch(int id1, int id2){
    ifstream data(dataFile, ios::binary);
    fstream index(indexFile, ios::in | ios::binary);
    IndexRecord indexRecord;
    Record temp;
    vector<Record> result;
    auto pos = find(id1, index);
    index.seekg(pos);
    readIndex(indexRecord, index);
    while(indexRecord.id < id2) {
        data.seekg(indexRecord.pos);
        readRecord(temp, data);
        result.push_back(temp);
        readIndex(indexRecord, index);
    }
    data.close();
    index.close();
    return result;
}

bool ISAM::remove(int id) {
    ifstream dataIn(dataFile, ios::binary);
    fstream index(indexFile, ios::in | ios::out | ios::binary);
    if(!dataIn || !index) return false;
    IndexRecord indexRecord;
    Record dataRecord;
    vector<IndexRecord> indexRewrite;
    vector<Record> dataRewrite;
    //vector<long> newPositions;
    unsigned long offset = 0;

    if(!search(id).has_value()) return false;
    auto pos = find(id, index);
    index.seekg(0);
    // save data before removed record
    while(index.tellg() < pos) {
        readIndex(indexRecord, index);
        indexRewrite.push_back(indexRecord);
        dataIn.seekg(indexRecord.pos);
        readRecord(dataRecord, dataIn);
        dataRewrite.push_back(dataRecord);
    }

    readIndex(indexRecord, index);

    // save data after removed record
    while(readIndex(indexRecord, index)) {
        dataIn.seekg(indexRecord.pos);
        readRecord(dataRecord, dataIn);
        dataRewrite.push_back(dataRecord);
        indexRewrite.push_back(indexRecord);
    }
    dataIn.close();
    index.close();
    ofstream dataOut(dataFile, ios::trunc | ios::binary);
    ofstream indexOut(indexFile, ios::trunc | ios::binary);
    for(auto & d : dataRewrite) {
        indexRecord.id = d.get_key();
        indexRecord.pos = dataOut.tellp();
        writeRecord(d, dataOut, offset);
        indexOut.write((char*)&indexRecord, sizeof(indexRecord));
    }
    dataOut.close();
    indexOut.close();
    return true;
}