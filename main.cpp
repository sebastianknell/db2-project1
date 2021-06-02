#include <iostream>
#include "test/Tester.h"
#include "SequentialFile.h"

using namespace std;

int main() {
    SequentialFile sequentialFile;
    sequentialFile.load_data("stroke-data.csv");
    FixedRecord record;
    vector<string> record_data;
    record_data.push_back("1");
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
    if (!sequentialFile.insert(record)) cout << "false\n";
    record_data[0] = "2";
    record.load_data(record_data);
    if (!sequentialFile.insert(record)) cout << "false\n";
    record_data[0] = "3";
    record.load_data(record_data);
    if (!sequentialFile.insert(record)) cout << "false\n";
    record_data[0] = "4";
    record.load_data(record_data);
    if (!sequentialFile.insert(record)) cout << "false\n";
    record_data[0] = "5";
    record.load_data(record_data);
    if (!sequentialFile.insert(record)) cout << "false\n";
    record_data[0] = "6";
    record.load_data(record_data);
    if (!sequentialFile.insert(record)) cout << "false\n";
    record_data[0] = "7";
    record.load_data(record_data);
    if (!sequentialFile.insert(record)) cout << "false\n";
    record_data[0] = "8";
    record.load_data(record_data);
    if (!sequentialFile.insert(record)) cout << "false\n";
    record_data[0] = "9";
    record.load_data(record_data);
    if (!sequentialFile.insert(record)) cout << "false\n";
    record_data[0] = "10";
    record.load_data(record_data);
    if (!sequentialFile.insert(record)) cout << "false\n";
    record_data[0] = "11";
    record.load_data(record_data);
    if (!sequentialFile.insert(record)) cout << "false\n";
    sequentialFile.print_all();
    
    return 0;
}
