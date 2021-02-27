// reading a text file
#include <iostream>
#include <fstream>
#include <streambuf>
#include <map>
#include <string>
#include <vector>
#include <iterator>


using std::vector;
using std::string;
using std::map;


string file_to_string(const string &filename)
{
    std::ifstream in(filename);

    string result((std::istreambuf_iterator<char>(in)),
                    std::istreambuf_iterator<char>());

    return result;
}


vector<string> split(const string &str, const char &delimiter)
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
    vector<string> file_vec = split(file_string, '\n');

    // Initialise the map with pairs of header values and empty vectors.
    vector<string> headers = split(file_vec[0], ',');

    for(size_t i = 0; i != headers.size(); ++i)
        csv_map.insert({ headers[i], vector<string>() });

    for(size_t i = 1; i != file_vec.size(); ++i)
    {
        vector<string> line_vec = split(file_vec[i], ',');

        for(size_t j = 0; j != headers.size(); ++j)
        {
            csv_map[headers[j]].push_back(line_vec[j]);
        }
    }

    return csv_map;
}





int main()
{
    map<string, vector< string > > csv_map = csv_to_map("ARK_INNOVATION_ETF_ARKK_HOLDINGS.csv");

    for(size_t i = 0; i != csv_map["company"].size(); ++i)
        std::cout << csv_map["company"][i] << std::endl;

    return 0;
}