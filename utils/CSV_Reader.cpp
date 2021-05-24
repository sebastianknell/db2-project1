//
// Created by Sebastian Knell on 23/05/21.
//

#include "CSV_Reader.h"

void CSV_Reader::load() {
    rapidcsv::Document document(filename);
    auto len = document.GetRowCount();
    for (int i = 0; i < len; i++) {
        vector<string> row = document.GetRow<string>(0);
        // Save row to record and write to file
    }
}
