#include <iostream>
#include "test/Tester.h"
#include "SequentialFile.h"

using namespace std;

int main() {
//    Tester::buffer_test();
//    Tester::record_test();
    SequentialFile sequentialFile;
    sequentialFile.load_data("stroke-data.csv");
    sequentialFile.print_all();
    return 0;
}
