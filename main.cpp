#include <iostream>
#include "test/Tester.h"
#include "SequentialFile.h"
#include "ISAM.h"

using namespace std;

int main() {
//    Tester::buffer_test();
//    Tester::record_test();
    /*SequentialFile sequentialFile;
    sequentialFile.load_data("stroke-data.csv");
    sequentialFile.print_all();*/

    ISAM isam;
    isam.loadData("stroke-data.csv");
    //isam.printAll();
    isam.search(49003).print();
    return 0;
}
