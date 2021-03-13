#include "ETL.h"

int main()
{
    // Pointer to SQLite connection
    sqlite3 *db = open_db_connection("db/noah.db");

    // Close the connection
    sqlite3_close(db);

    return 0;
}
