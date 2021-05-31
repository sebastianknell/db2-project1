#include <iostream>
#include "test/Tester.h"
#include "SequentialFile.h"
#include "ISAM.h"
#include <iterator>

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
    //isam.search(49003).print();
    cout << "\n";

    vector<Record> v = isam.rangeSearch(49003, 49086);
    for(auto & i : v){
        i.print();
    }

    return 0;
}
