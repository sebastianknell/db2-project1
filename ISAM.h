//
// Created by Massimo on 30-May-21.
//

#ifndef DB2_PROJECT1_ISAM_H
#define DB2_PROJECT1_ISAM_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <optional>
#include "utils/rapidcsv.h"
#include "utils/Buffer.h"
#include "Record.h"

using namespace std;

struct IndexRecord {
    int id{};
    long long pos{};
};

extern int countReadIndex;
extern int countWriteIndex;
extern int countReadData;
extern int countWriteData;

class ISAM {
    string dataFile;
    string indexFile = "index.dat";
    //int maxIndexSize;

    static bool readIndex(IndexRecord&, ifstream&);
    static bool readIndex(IndexRecord&, fstream&);
    static bool writeIndex(IndexRecord&, ofstream&);
    static bool writeIndex(IndexRecord &indexRecord, fstream &stream);
    static bool readRecord(Record &, ifstream&);
    static bool writeRecord(Record &, ofstream&, unsigned long&);

public:
    explicit ISAM(string dataFile = "ISAM-data.dat"): dataFile(move(dataFile))  {};
    void loadData(const string&);
    void printAll();
    optional<Record> search(int);
    vector<Record> rangeSearch(int, int);
    void insert(Record);
    bool remove(int);
    //void addIndexLevel();
    static long long getFileSize(ifstream&);
    static long long getFileSize(fstream&);
    static long long find(int id, fstream &stream);

    void printIndex();
};


#endif //DB2_PROJECT1_ISAM_H
