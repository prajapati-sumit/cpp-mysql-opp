#ifndef l_row_h
#define l_row_h
#include <malloc.h>
#include <mysql.h>
#include <sys/time.h>
#include<iomanip>

#include <iostream>
#define T_RESULT MYSQL_RES
#define T_ROW MYSQL_ROW

using namespace std;

class Row {
   private:
    T_RESULT *result;
    T_ROW fields;

   public:
    Row(T_RESULT *, T_ROW);
    ~Row();

    const char *getField(int);
    int getFieldCount();
    int isClosed();
    void close();
    void print(int);
};

Row::Row(T_RESULT *res, T_ROW row) {
    result = res;
    fields = row;
}
Row::~Row() {
    if (!isClosed()) close();
}

void Row::close() {
    if (!isClosed()) {
        return;
    }
    fields = (T_ROW)NULL;
    result = (T_RESULT *)NULL;
}
int Row::getFieldCount() {
    if (isClosed()) return 0;

    return mysql_num_fields(result);
}

const char *Row::getField(int field) {
    if (isClosed()) return "Row closed";
    if (field < 0 || field >= getFieldCount()) throw "Field out of range.";
    return fields[field] ? fields[field] : "NULL";
}
int Row::isClosed() { return (fields == (T_ROW)NULL); }
void Row::print(int width) {
    const char separator = ' ';
    for (int i = 0; i < getFieldCount(); i++) {
        cout << left << setw(width) << setfill(separator) << getField(i);
    }
    cout << endl;
}
#endif
