#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <regex>
using std::regex;
#include <stdio.h>
#include <curl/curl.h>
#include <cstring>
#include <fstream>
char *url = "https://ark-funds.com/wp-content/fundsiteliterature/csv/ARK_AUTONOMOUS_TECHNOLOGY_&_ROBOTICS_ETF_ARKQ_HOLDINGS.csv";

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
std::string create_file_name(char *url)
{
    // last slash of url becomes file name
    regex r("([^\\/]+$)");
    std::string u(url);

    std::smatch match;
    if (regex_search(u, match, r))
    {
        return match.str(0);
    }
}
int main(void)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    std::string file_str;
    file_str = "data/fund_csvs/" + create_file_name(url);
    curl = curl_easy_init();
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    if (curl)
    {
        fp = fopen(file_str.c_str(), "wb");
        if (fp == NULL) {
            perror("FOPEN()");
            // turns out error handling on opening files is important
            return EXIT_FAILURE;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}