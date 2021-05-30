#include <iostream>
#include "test/Tester.h"
#include "SequentialFile.h"

using namespace std;

int main() {
//    Tester::buffer_test();
//    Tester::record_test();
    SequentialFile sequentialFile;
    sequentialFile.load_data("aa-data.csv");
//    sequentialFile.print_all();
    auto result = sequentialFile.search(2);
    for (auto &r: result) r.print();
    return 0;
}
