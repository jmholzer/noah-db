#ifndef GUARD_requests_h
#define GUARD_requests_h

#include <curl/curl.h>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <ctime>

std::string get_scrape_data(const char *url);
std::vector<std::string> batch_dl(std::map<std::string, std::vector<std::string>> funds);
std::string save_fund_data(std::string name, const char *url);

#endif
