
#ifndef result_hpp
#define result_hpp

#include <mysql.h>
#include<iostream>
#include<string.h>
#include <sys/time.h>
#include "row.hpp"

using namespace std;

class Result {
   private:
    int row_count;
    T_RESULT *result;
    Row *currentRow;
    

   public:
    Result(T_RESULT *);
    ~Result();

    void close();
    Row *getCurrentRow();
    int getRowCount();
    int next();
    void print();
};

Result::Result(T_RESULT *res) {
    result = res;
    row_count = mysql_num_rows(result);
    currentRow = new Row(result, mysql_fetch_row(result));
}

int Result::next() {
    T_ROW row;
    if (result == (T_RESULT *)NULL) {
        std::cerr<<( "No result set found\n");
        return 0;
    }

    row = mysql_fetch_row(result);
    if (!row) {
        currentRow = (Row *)NULL;
        return 0;
    }
    currentRow = new Row(result, row);
    return 1;
}
Row *Result::getCurrentRow() {
    if (result == (T_RESULT *)NULL) {
        std::cerr<<( "No result set found\n");
        return (Row *)NULL;
    }
    return currentRow;
}

void Result::close() {
    if (result == (T_RESULT *)NULL) {
        std::cerr<<( "No result set found\n");
        return;
    }

    mysql_free_result(result);
    result = (T_RESULT *)NULL;
}
int Result::getRowCount() {
    if (result == (T_RESULT *)NULL) {
        std::cerr<<( "No result set found\n");
        return 0;
    }

    return mysql_num_rows(result);
}
void Result::print() {
    MYSQL_FIELD *field;
    int max_length = 16;
    int i;
    for(i = 0; (field = mysql_fetch_field(result)); i++) 
        cout << left << setw(max_length) << setfill(' ') << field->name;
    cout<<"\n"<<string(i*max_length,'-')<<"\n";
    for (int i = 0; i < getRowCount(); i++) {
        getCurrentRow()->print(max_length);
        next();
    }
    cout<<"\n"<<getRowCount()<<" rows in set\n";
}
#endif  // l_result_h