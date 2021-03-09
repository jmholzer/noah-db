#include "ETL_sql.h"

std::string insert_into_holdings = "INSERT INTO holdings (fund_id, ticker, exchange, quantity, unix_time) VALUES (?1, ?2, ?3, ?4, ?5);";
