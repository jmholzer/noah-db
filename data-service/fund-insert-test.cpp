#include "sqlite3.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

string sql = "INSERT INTO funds (fund_id, fund_name, fund_manager)" \
                "VALUES (?1, ?2, ?3);";

int fund_id = 100;
string fund = "ARK";
string fund_name = "Innovation";
string fund_manager = "Ark Investments";


int main() {
    // Pointer to SQLite connection
    sqlite3* db;

    // Save any error messages
    char *zErrMsg = 0;

    // Save the connection result
    int rc = 0;
    rc = sqlite3_open("noah.db", &db);
    if (rc)
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl; 
    else
        cout << "Opened Database Successfully!" << endl; 

    sqlite3_stmt* pStmt = nullptr;
    sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &pStmt, nullptr);

    sqlite3_bind_text(pStmt, 1, fund.c_str(), fund.size() + 1, NULL);
    sqlite3_bind_text(pStmt, 2, fund_name.c_str(), fund_name.size() + 1, NULL);
    sqlite3_bind_text(pStmt, 3, fund_manager.c_str(), fund_manager.size() + 1, NULL);

    sqlite3_step(pStmt);

    sqlite3_reset(pStmt);

    sqlite3_clear_bindings(pStmt);

    sqlite3_finalize(pStmt);

    // Close the connection
    sqlite3_close(db);

    return 0;
}