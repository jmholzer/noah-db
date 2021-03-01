#include <iostream>
#include <fstream>
#include <streambuf>

#include <map>
#include <string>
#include <vector>
#include <iterator>

#include "sqlite3.h"

#include "ETL_sql.h"

#include <ctime>
using std::time_t;
using std::time;

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::map;

string file_to_string(const string &filename);
vector<string> split_to_vec(const string &str, const char &delimiter);
map<string, vector<string> > csv_to_map(const string &file_name);
void db_holdings_input(sqlite3* db, map<string, vector<string> > raw_fund_data);
int db_input(const string &in_file_name, sqlite3* db, const string &fund_format);
sqlite3* open_db_connection(const string &db_file_name);

string file_to_string(const string &filename)
{
    std::ifstream in(filename);

    string result((std::istreambuf_iterator<char>(in)),
                    std::istreambuf_iterator<char>());

    return result;
}


vector<string> split_to_vec(const string &str, const char &delimiter)
{
    vector<string> result;

    string temp_str;
    
    for (char c : str)
    {
        if (c != delimiter)
        {
            temp_str += c;
        }
        else
        {
            result.push_back(temp_str);
            temp_str = "";
        }
    }
    result.push_back(temp_str);

    return result;
}


map<string, vector<string> > csv_to_map(const string &file_name)
{
    map<string, vector<string> > csv_map;

    string file_string = file_to_string(file_name);
    vector<string> file_vec = split_to_vec(file_string, '\n');

    // Initialise the map with pairs of header values and empty vectors.
    vector<string> headers = split_to_vec(file_vec[0], ',');

    for(size_t i = 0; i != headers.size(); ++i)
        csv_map.insert({ headers[i], vector<string>() });

    // Fill the vectors associated with each header with the corresponding 
    // values, start iterating from 1 to avoid reading header into vectors.
    for(size_t i = 1; i != file_vec.size(); ++i)
    {
        vector<string> line_vec = split_to_vec(file_vec[i], ',');

        for(size_t j = 0; j != headers.size(); ++j)
        {
            csv_map[headers[j]].push_back(line_vec[j]);
        }
    }

    return csv_map;
}


void db_holdings_input(sqlite3* db, map<string, vector<string> > raw_fund_data)
{
    string exchange = "NYSE";
    time_t t = time(0);

    sqlite3_stmt* pStmt = nullptr;
    sqlite3_prepare_v2(db, insert_into_holdings.c_str(), insert_into_holdings.size() + 1, &pStmt, nullptr);

    for(size_t i = 1; i != raw_fund_data["fund"].size(); ++i)
    {
        sqlite3_bind_text(pStmt, 1, raw_fund_data["fund"][i].c_str(), raw_fund_data["fund"][i].size() + 1, NULL);
        sqlite3_bind_text(pStmt, 2, raw_fund_data["ticker"][i].c_str(), raw_fund_data["ticker"][i].size() + 1, NULL);
        sqlite3_bind_text(pStmt, 3, exchange.c_str(), exchange.size() + 1, NULL);
        sqlite3_bind_double(pStmt, 4, std::stod(raw_fund_data["shares"][i]));
        sqlite3_bind_int(pStmt, 5, t);

        sqlite3_step(pStmt);

        sqlite3_reset(pStmt);

        sqlite3_clear_bindings(pStmt);
    }

    sqlite3_finalize(pStmt);
}


int db_input(const string &in_file_name, sqlite3* db, const string &fund_format)
{
    map<string, vector<string> > fund_data = csv_to_map(in_file_name);
}


sqlite3* open_db_connection(const string &db_file_name)
{
    sqlite3* db;

    int conn = sqlite3_open(db_file_name.c_str(), &db);

    if(conn)
        cout << "Error opening the database: " << sqlite3_errmsg(db) << endl;
    else
        cout << "Opened the database successfully." << endl;
    
    return db;
}
 

int main()
{
    map<string, vector<string> > raw_fund_data = csv_to_map("ARK_INNOVATION_ETF_ARKK_HOLDINGS.csv");

    // Pointer to SQLite connection
    sqlite3* db = open_db_connection("noah.db");

    db_holdings_input(db, raw_fund_data);

    // Close the connection
    sqlite3_close(db);

    return 0;
}