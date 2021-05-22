//
// Created by Sebastian Knell on 22/05/21.
//

#ifndef DB2_PROJECT1_SEQUENTIALFILE_H
#define DB2_PROJECT1_SEQUENTIALFILE_H

#include <vector>
using namespace std;

struct Register {
    int id;
};

class SequentialFile {
public:
    vector<Register> search(int key);
    vector<Register> range_search(int begin_key, int end_key);
    void insert(Register r);
    void remove(int key);
};


#endif //DB2_PROJECT1_SEQUENTIALFILE_H
