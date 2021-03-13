#ifndef GUARD_requests_h
#define GUARD_requests_h

#include <curl/curl.h>
#include <fstream>
#include <string>
#include <ctime>

struct fund {
    const char* url;
    std::string name;
};

bool save_fund_data(fund target);

#endif
