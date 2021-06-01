#include <iostream>
#include "test/Tester.h"
#include "SequentialFile.h"

using namespace std;

int main() {
//    Tester::buffer_test();
//    Tester::record_test();
    SequentialFile sequentialFile;
//    sequentialFile.load_data("stroke-data.csv");
//    sequentialFile.print_all();
//    auto result = sequentialFile.search(66494);
//    if (result.has_value()) print_record(result.value());

    FixedRecord record;
    vector<string> record_data;
    record_data.push_back("2");
    record_data.push_back("Male");
    record_data.push_back("25");
    record_data.push_back("1");
    record_data.push_back("0");
    record_data.push_back("Yes");
    record_data.push_back("Private");
    record_data.push_back("Urban");
    record_data.push_back("100.01");
    record_data.push_back("30.8");
    record_data.push_back("smokes");
    record_data.push_back("0");
    record.load_data(record_data);
//    if (!sequentialFile.insert(record)) cout << "false\n";

    auto result = sequentialFile.search(2);
    if (result.has_value()) print_record(result.value());
//    sequentialFile.print_all();
    return 0;
}
