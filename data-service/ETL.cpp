#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
#include <streambuf>

#include <map>
using std::map;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <regex>
using std::regex;
using std::regex_match;

#include <ctime>
using std::time;
using std::time_t;

#include <iterator>
using std::back_inserter;
using std::next;

#include <regex>

#include <sqlite3.h>
#include "ETL_sql.h"

string read_file(const string &in_file_name);
template <class C> void split(const string &str, const char &delimiter, C &out);
void write_to_csv_file(const string &file_name, const string &output);
map<string, vector<string>> csv_to_map(const string &file_name, bool (*row_schema_test)(const vector<string>&));
int db_input(const string &in_file_name, sqlite3 *db, const string &fund_format);
sqlite3 *open_db_connection(const string &db_file_name);


string read_file(const string &in_file_name)
{
    std::ifstream in_file(in_file_name);

    if (!in_file)
    {
        std::cerr << "file open failed"
                  << "\n";
    }

    string out((std::istreambuf_iterator<char>(in_file)),
               std::istreambuf_iterator<char>());

    return out;
}

template <class C>
void split(const string &str, const char &delimiter, C &out)
{
    string temp_str;

    for (char c : str)
    {
        if (c != delimiter)
        {
            temp_str += c;
        }
        else
        {
            out.insert(out.end(), temp_str);
            temp_str = "";
        }
    }

    out.insert(out.end(), temp_str);
}

void write_to_csv_file(const string &file_name, const string &output)
{
    std::ofstream output_file{file_name};
    if (!output_file)
    {
        std::cerr << "file open failed"
                  << "\n";
    }
    output_file << output;
    output_file.close();
}

bool is_number(const std::string& s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

bool ark_schema_test(const vector<string>& row)
{
    if(row.size() < 8)
        return false;

    const regex us_date("^([1-9]|1[0-2])\\/([1-9]|[1-2][0-9]|3[0-1])\\/20[0-9]{2}$");
    const regex eu_date("^([1-9]|[1-2][0-9]|3[0-1])\\/([1-9]|1[0-2])\\/20[0-9]{2}$");
    
    if(!regex_match(row[0], us_date) || !regex_match(row[0], eu_date))
        return false;

    const regex fund("^ARK[A-Z]$");

    if(!regex_match(row[1], fund))
        return false;

    const regex numeric("^[0-9]+(\\.[0-9]+)?$");

    if(!regex_match(row[5], numeric))
        return false;

    if(!regex_match(row[6], numeric))
        return false;

    if(!regex_match(row[7], numeric))
        return false;

    return true;
}

map<string, vector<string>> csv_to_map(
    const string &file_name,
    bool (*row_schema_test)(const vector<string>&)
)
{
    map<string, vector<string>> csv_map;

    string in_file_contents = read_file(file_name);
    vector<string> in_rows;
    split<vector<string>>(in_file_contents, '\n', in_rows);

    // Initialise the map with pairs of header values and empty vectors.
    vector<string> headers;
    split<vector<string>>(*in_rows.begin(), ',', headers);

    for (vector<string>::iterator it = headers.begin(); it != headers.end(); ++it)
        csv_map.insert({*it, vector<string>()});

    // Fill the vectors associated with each header with the corresponding
    // values, start iterating from 1 to avoid reading header into vectors.
    for (vector<string>::iterator it = next(in_rows.begin()); it != in_rows.end(); ++it)
    {
        vector<string> line;
        split(*it, ',', line);

        if (row_schema_test(line))
        {
            for (size_t j = 0; j != headers.size(); ++j)
                csv_map[headers[j]].push_back(line[j]);
        }
        else
        {
            continue;
        }
    }

    return csv_map;
}

void db_holdings_input(sqlite3 *db, map<string, vector<string>> raw_fund_data)
{
    string exchange = "NYSE";
    time_t t = time(0);

    sqlite3_stmt *pStmt = nullptr;
    sqlite3_prepare_v2(db, insert_into_holdings.c_str(), insert_into_holdings.size() + 1, &pStmt, nullptr);

    for (size_t i = 0; i != raw_fund_data["fund"].size(); ++i)
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

int db_input(const string &in_file_name, sqlite3 *db, const string &fund_format)
{
    map<string, vector<string>> fund_data = csv_to_map(in_file_name, ark_schema_test);
    return 20;
}

sqlite3 *open_db_connection(const string &db_file_name)
{
    sqlite3 *db;

    int conn = sqlite3_open(db_file_name.c_str(), &db);

    if (conn)
        cout << "Error opening the database: " << sqlite3_errmsg(db) << endl;
    else
        cout << "Opened the database successfully." << endl;

    return db;
}

int main()
{
    map<string, vector<string>> raw_fund_data = csv_to_map("ARK_AUTONOMOUS_TECHNOLOGY_&_ROBOTICS_ETF_ARKQ_HOLDINGS.csv", ark_schema_test);

    // Pointer to SQLite connection
    sqlite3 *db = open_db_connection("noah.db");

    db_holdings_input(db, raw_fund_data);

    // Close the connection
    sqlite3_close(db);

    return 0;
}
