//
// Created by Sebastian Knell on 22/05/21.
//

#include "SequentialFile.h"

void SequentialFile::load_data(string filename) {
    rapidcsv::Document document(filename);
    auto len = document.GetRowCount();
    for (int i = 0; i < len; i++) {
        vector<string> row = document.GetRow<string>(0);
        // Save row to record and write to file
    }
}

vector<Record> SequentialFile::search(int key) {
    return vector<Record>();
}

vector<Record> SequentialFile::range_search(int begin_key, int end_key) {
    return vector<Record>();
}

void SequentialFile::insert(Record r) {

}

void SequentialFile::remove(int key) {

}
