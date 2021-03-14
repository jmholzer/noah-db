/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2020, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 

#include "dl.h"

std::vector<std::string> batch_dl(std::map<std::string, std::vector<std::string>> funds)
{
    std::vector<std::string> file_names;

    for(size_t i = 0; i != funds["name"].size(); ++i)
    {
        std::string file_name = save_fund_data(funds["name"][i], funds["url"][i].c_str());
        file_names.push_back(file_name);
    }

    return file_names;
}

std::string save_fund_data(std::string name, const char *url)
{
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    std::string file_name = "data/" + name
                            + "_" + std::to_string(now->tm_year + 1900)
                            + "_" + std::to_string(now->tm_mon + 1)
                            + "_" + std::to_string(now->tm_mday)
                            + ".csv";

    FILE * out = fopen(file_name.c_str(), "w");


    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl;
    curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        fprintf(stderr,"%s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return file_name;
}
