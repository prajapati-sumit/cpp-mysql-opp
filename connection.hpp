#ifndef l_connection_h
#define l_connection_h

#include <mysql.h>
#include <sys/time.h>

#include <iostream>

#include "result.hpp"
class Connection {
   private:
    int affectedRows;

    MYSQL mysql;
    MYSQL *connection;

   public:
    Connection();
    Connection(const char *, const char *);
    Connection(const char *, const char *, const char *, const char *);
    ~Connection();

    bool connect(const char *host, const char *user, const char *pass,
                 const char *db);
    void close();
    const char *getError();
    Result *query(const char *);
    int getAffectedRows();
    bool isConnected();
};

// intialize the connection object using host and database name
Connection::Connection(const char *host, const char *db) {
    connection = (MYSQL *)NULL;
    connect(host, db, (const char *)NULL, (const char *)NULL);
}

// intializing the connection object with host, user, password and database
Connection::Connection(const char *host, const char *user, const char *pass,
                       const char *db) {
    connection = (MYSQL *)NULL;
    connect(host, user, pass, db);
}

// making the connection to the database
bool Connection::connect(const char *host, const char *user, const char *pass,
                         const char *db) {
    if (isConnected()) {
        std::cerr << ("Connection has already been established.\n");
        return true;
    }

    mysql_init(&mysql);
    connection =
        mysql_real_connect(&mysql, host, user, pass, db, 3306, NULL, 0);
    return isConnected();
}

// freeing the database resources
Connection::~Connection() {
    if (isConnected()) close();
}
// close the connection
void Connection::close() {
    if (!isConnected()) {
        std::cerr << ("There is no connection to close.\n");
        return;
    }
    mysql_close(connection);
    connection = (MYSQL *)NULL;
}

// quering the database
Result *Connection::query(const char *sql) {
    T_RESULT *result;
    int state;
    // if not connected , cannot query
    if (!isConnected()) {
        std::cerr << ("There is no connection to query.\n");
        return NULL;
    }

    state = mysql_query(connection, sql);

    // if query failed, throw error
    if (state < 0) {
        std::cerr << (getError()) << "\n";
        return NULL;
    }

    // store the query result
    result = mysql_store_result(connection);

    if (result == (T_RESULT *)NULL) {
        int fieldCount = mysql_field_count(connection);
        if (fieldCount > 0) {
            affectedRows = mysql_affected_rows(connection);
        } else {
            affectedRows = 0;
            std::cerr << (getError()) << "\n";
        }

        return (Result *)NULL;
    }
    return new Result(result);
}

int Connection::getAffectedRows() { return affectedRows; }

const char *Connection::getError() {
    if (isConnected()) return mysql_error(connection);
    return mysql_error(&mysql);
}

bool Connection::isConnected() {
    return connection;
}

#endif  // l_connection_h
