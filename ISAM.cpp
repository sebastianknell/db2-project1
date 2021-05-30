//
// Created by Massimo on 30-May-21.
//

#include "ISAM.h"

void ISAM::loadData(const string& fromFilename) {
    Record temp;
    ofstream outFile(dataFile, ios::out | ios::binary);
    if(!outFile) return;
    rapidcsv::Document document(fromFilename);
    auto len = document.GetRowCount();
    unsigned long offset = 0;

    for(int i = 0 ; i < len ; i++)
    {
        vector<string> row = document.GetRow<string>(i);
        temp.load_data(row);
        writeRecord(temp, outFile, offset);
    }
}

bool ISAM::writeRecord(Record &record, ofstream &stream, unsigned long &offset)
{
    return false;
}