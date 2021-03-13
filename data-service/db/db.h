#ifndef GUARD_db_h
#define GUARD_db_h 

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sqlite3.h>

sqlite3 *open_db_connection(const std::string &db_file_name);
void db_holdings_input(sqlite3 *db, std::map<std::string, std::vector<std::string>> raw_fund_data);

#endif
