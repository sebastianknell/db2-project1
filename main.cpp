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
//    auto result = sequentialFile.search(10245);
//    if (result.has_value()) print_record(result.value());
    return 0;
}
