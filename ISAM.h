//
// Created by Massimo on 30-May-21.
//

#ifndef DB2_PROJECT1_ISAM_H
#define DB2_PROJECT1_ISAM_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include "utils/rapidcsv.h"
#include "utils/Buffer.h"
#include "Record.h"

using namespace std;

struct IndexRecord {
    int id{};
    long long pos{};
};

class ISAM {
    string dataFile;
    string indexFile = "index.dat";
    int maxIndexSize;

    bool readIndex(IndexRecord&, ifstream&);
    bool readIndex(IndexRecord&, fstream&);
    bool writeIndex(IndexRecord&, ofstream&);
    bool writeIndex(IndexRecord &indexRecord, fstream &stream);
    bool readRecord(Record &, ifstream&);
    bool writeRecord(Record &, ofstream&, unsigned long&);

public:
    explicit ISAM(string dataFile = "ISAM-data.dat", int maxIndexSize = 1000): dataFile(move(dataFile)), maxIndexSize(maxIndexSize) {};
    void loadData(const string&);
    void printAll();
    Record search(int);
    vector<Record> rangeSearch(int, int);
    void insert(Record);
    void remove(int);
    void addIndexLevel();
    static long getFileSize(ifstream&);
    static long getFileSize(fstream&);
    static long findPos(int, fstream&);

    long find(int id, fstream &stream);

    void printIndex();
};


#endif //DB2_PROJECT1_ISAM_H
