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
    //isam.loadData("stroke-data.csv");

    Record record;
    record.data.push_back("53106");
    record.data.push_back("Male");
    record.data.push_back("25");
    record.data.push_back("1");
    record.data.push_back("0");
    record.data.push_back("Yes");
    record.data.push_back("Private");
    record.data.push_back("Urban");
    record.data.push_back("100.01");
    record.data.push_back("30.8");
    record.data.push_back("smokes");
    record.data.push_back("0");

    //isam.insert(record);
    isam.search(53106);
    //isam.printAll();
    //isam.printIndex();

    return 0;
}
