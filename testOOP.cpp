// Command to Compile: g++ demo.cpp -o out -lmysql && ./out
#include <windows.h>

#include <iostream>
#include <typeinfo>
#include "connection.hpp"
using namespace std;

int main() {
    Connection conn("localhost", "sumit", "sumit", "db_course");
    if (!conn.isConnected()) {
        cout << "Connection Failed\n";
        return 1;
    }
    cout << "Connection Successful\n";

    Result *res = conn.query("DESC Persons;");

    
    res->print();
    // while (res->getCurrentRow()) {
    //     res->next();
    // }
    conn.close();
    return 0;
}