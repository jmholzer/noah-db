#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <curl/curl.h>
#include <regex>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <filesystem>
using std::regex;

static const char *urls[] = {
    "https://ark-funds.com/wp-content/fundsiteliterature/csv/ARK_AUTONOMOUS_TECHNOLOGY_&_ROBOTICS_ETF_ARKQ_HOLDINGS.csv",
    // test multiple files
    "https://gitlab.com/jmholzer/noah-db",
    // for some reason it does 1 less url than here
    ""
};

#define MAX_PARALLEL 1
#define NUM_URLS sizeof(urls) / sizeof(char *)

std::string create_file_name(const char *url);

int idx;
namespace fs = std::experimental::filesystem;
static size_t write_cb(char *data, size_t n, size_t l, void *userp)
{
    std::string s(data);
    std::string file_str;
    file_str = "data/fund_csvs/" + create_file_name(urls[idx]);
    std::ofstream outfile;
    // open file in append mode to append chunks of string
    outfile.open(file_str, std::ofstream::app);
    outfile << s;
    (void)userp;
    return n * l;
}

static void add_transfer(CURLM *cm, int i)
{
    CURL *eh = curl_easy_init();
    curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(eh, CURLOPT_URL, urls[i]);
    curl_easy_setopt(eh, CURLOPT_PRIVATE, urls[i]);
    curl_multi_add_handle(cm, eh);
}
std::string create_file_name(const char *url)
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
void delete_files()
{   
    for (const auto& entry : std::filesystem::directory_iterator("data/fund_csvs")) 
        std::filesystem::remove_all(entry.path());
}
int main(void)
{
    std::cout << NUM_URLS;
    CURLM *cm;
    CURLMsg *msg;
    unsigned int transfers = 0;
    int msgs_left = -1;
    int still_alive = 1;

    curl_global_init(CURL_GLOBAL_ALL);
    cm = curl_multi_init();

    /* Limit the amount of simultaneous connections curl should allow: */
    curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)2);
    // curl gets data in chuncks so delete destination folders before data is added to them
    delete_files();
    for (transfers = 0; transfers < MAX_PARALLEL; transfers++)
        add_transfer(cm, transfers);

    do
    {
        curl_multi_perform(cm, &still_alive);

        while ((msg = curl_multi_info_read(cm, &msgs_left)))
        {
            if (msg->msg == CURLMSG_DONE)
            {
                char *url;
                CURL *e = msg->easy_handle;
                curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
                fprintf(stderr, "R: %d - %s <%s>\n",
                        msg->data.result, curl_easy_strerror(msg->data.result), url);
                curl_multi_remove_handle(cm, e);
                curl_easy_cleanup(e);
            }
            else
            {
                fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
            }
            if (transfers < NUM_URLS)
            {
                idx += 1;
                add_transfer(cm, transfers++);
            }
        }
        if (still_alive)
            curl_multi_wait(cm, NULL, 0, 1000, NULL);

    } while (still_alive || (transfers < NUM_URLS));

    curl_multi_cleanup(cm);
    curl_global_cleanup();

    return EXIT_SUCCESS;
}
