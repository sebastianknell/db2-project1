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

    // LOAD ALL DATA
    ISAM isam;
    isam.loadData("stroke-data.csv");

    // TEST RECORD INSTANCE
    Record record;
    record.data.emplace_back("53106");
    record.data.emplace_back("Male");
    record.data.emplace_back("25");
    record.data.emplace_back("1");
    record.data.emplace_back("0");
    record.data.emplace_back("Yes");
    record.data.emplace_back("Private");
    record.data.emplace_back("Urban");
    record.data.emplace_back("100.01");
    record.data.emplace_back("30.8");
    record.data.emplace_back("smokes");
    record.data.emplace_back("0");

    // INSERT TEST / SEARCH TEST
    isam.insert(record);
    //isam.search(53106);
    //isam.printAll();
    //isam.printIndex();


    // RANGE SEARCH TEST
    /*vector<Record> range = isam.rangeSearch(53105, 53195);
    for(auto & i : range) {
        i.print();
    }*/

    // REMOVE TEST
    isam.remove(53106);
    //isam.search(53106);
    //isam.printIndex();
    isam.printAll();

    return 0;
}
