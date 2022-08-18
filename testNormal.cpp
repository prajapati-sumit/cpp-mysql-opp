//Command to Compile: g++ demo.cpp -o out -lmysql && ./out
#include <iostream>
#include <windows.h>
#include <mysql.h>

using namespace std;
/*-----------------Debugging---------------------*/

template<class Fun> class y_combinator_result {
    Fun fun_;
public:
    template<class T> explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}
    template<class ...Args> decltype(auto) operator()(Args &&...args) { return fun_(std::ref(*this), std::forward<Args>(args)...); }
};
template<class Fun> decltype(auto) y_combinator(Fun &&fun) { return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun)); }
template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { return os << '(' << p.first << ", " << p.second << ')'; }
template < typename T_container, typename T = typename enable_if < !is_same<T_container, string>::value, typename T_container::value_type >::type > ostream & operator<<(ostream &os, const T_container &v) { os << '{'; string sep; for (const T &x : v) os << sep << x, sep = ", "; return os << '}'; }
void dbg_out() { cerr << endl; }
template<typename TT, typename... UU> void dbg_out(TT H, UU... T) { cerr << ' ' << H; dbg_out(T...); }
#ifndef ONLINE_JUDGE
#define dbg(...) cerr << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
#define stop(...) cerr << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__);cerr<<"exited\n";exit(0);
#else
#define dbg(...)
#define stop(...)
#endif

struct connection_details {
    const char *server, *user, *password, *database;

};

MYSQL* mysql_connection_setup(struct connection_details mysql_details) {
    MYSQL *connection = mysql_init(NULL);
    if (!mysql_real_connect(
                connection, mysql_details.server,
                mysql_details.user,
                mysql_details.password,
                mysql_details.database,
                0,
                NULL,
                0)
       ) {

        cout << "Connection Error: " << mysql_error(connection) << "\n";
        exit(1);
    }
    return connection;

}
MYSQL_RES* mysql_execute_query(
    MYSQL *connection,
    const char *sql_query) {

    if (mysql_query(connection, sql_query)) {
        cout << "MYSQL Query Error: " << mysql_error(connection) << "\n";
        exit(1);

    }
    return mysql_use_result(connection);

}
int main(int argc, char const *argv[]) {

    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    struct connection_details mysqlD;

    mysqlD.server = "localhost";
    mysqlD.user = "sumit";
    mysqlD.password = "sumit";
    mysqlD.database = "db_course";

    conn = mysql_connection_setup(mysqlD);

    res = mysql_execute_query(conn,
                              "DESC Persons;"
                             );
    cout << "Displaying database output:\n\n";

    while ((row = mysql_fetch_row(res)) != NULL) {
        dbg(row);
        //cout << row[0] << " | " << row[1] << "| " << row[2] << " | " << row[3] << " | " << row[4] << "\n\n";
    }
    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}