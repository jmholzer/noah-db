#include "ETL.h"

int main()
{
    std::map<std::string, std::vector<std::string>> funds = csv_to_map("funds.csv");
    std::vector<std::string> file_names =  batch_dl(funds);

    // Pointer to SQLite connection
    sqlite3 *db = open_db_connection("db/noah.db");

    for(size_t i = 0; i != file_names.size(); ++i)
    {
        std::map<std::string, std::vector<std::string>> raw_fund_data = csv_to_map(file_names[i], ark_schema_test);
        db_holdings_input(db, raw_fund_data);
    }

    // Close the connection
    sqlite3_close(db);
    
    
    return 0;
}
