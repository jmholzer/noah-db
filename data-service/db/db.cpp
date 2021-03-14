#include "db.h"
#include "../csv/csv.h"

sqlite3 *open_db_connection(const std::string &db_file_name)
{
    sqlite3 *db;

    int conn = sqlite3_open(db_file_name.c_str(), &db);

    if (conn)
        std::cout << "Error opening the database: " << sqlite3_errmsg(db) << std::endl;
    else
        std::cout << "Opened the database successfully." << std::endl;

    return db;
}

void db_holdings_input(sqlite3 *db, std::map<std::string, std::vector<std::string>> raw_fund_data)
{
    std::string insert_into_holdings = read_file("db/insert_into_holdings.sql");

    sqlite3_stmt *pStmt = nullptr;
    sqlite3_prepare_v2(db, insert_into_holdings.c_str(), insert_into_holdings.size() + 1, &pStmt, nullptr);


    for (size_t i = 0; i != raw_fund_data["fund"].size(); ++i)
    {
        sqlite3_bind_text(pStmt, 1, raw_fund_data["fund"][i].c_str(), -1, NULL);
        sqlite3_bind_text(pStmt, 2, raw_fund_data["ticker"][i].c_str(), -1, NULL);
        sqlite3_bind_double(pStmt, 3, std::stod(raw_fund_data["shares"][i]));
        sqlite3_bind_text(pStmt, 4, raw_fund_data["date"][i].c_str(), -1, NULL);

        sqlite3_step(pStmt);
        sqlite3_reset(pStmt);
        sqlite3_clear_bindings(pStmt);
    }

    sqlite3_finalize(pStmt);
}
