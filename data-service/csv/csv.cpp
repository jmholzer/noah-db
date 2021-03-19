#include "csv.h"

std::string read_file(const std::string &in_file_name)
{
    std::ifstream in_file(in_file_name);

    if (!in_file)
    {
        std::cerr << "file open failed"
                  << in_file_name
                  << "\n";
    }

    std::string out((std::istreambuf_iterator<char>(in_file)),
               std::istreambuf_iterator<char>());

    return out;
}

template <class C>
void split(const std::string &str, const char &delimiter, C &out)
{
    std::string temp_str;

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

void write_to_csv_file(const std::string &file_name, const std::string &output)
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
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

bool ark_schema_test(const std::vector<std::string>& row)
{
    if(row.size() < 8){
        return false;
    }

    const std::regex us_date("^([1-9]|1[0-2])\\/([1-9]|[1-2][0-9]|3[0-1])\\/20[0-9]{2}$");
    const std::regex eu_date("^([1-9]|[1-2][0-9]|3[0-1])\\/([1-9]|1[0-2])\\/20[0-9]{2}$");
    
    if(!std::regex_match(row[0], us_date) && !std::regex_match(row[0], eu_date)){
        return false;
    }

    const std::regex fund("^ARK[A-Z]$");

    if(!std::regex_match(row[1], fund)){
        return false;
    }

    const std::regex numeric("^[0-9]+(\\.[0-9]+)?$");

    if(!std::regex_match(row[5], numeric)){
        return false;
    }

    if(!std::regex_match(row[6], numeric)){
        return false;
    }

    if(!std::regex_match(row[7], numeric)){
        return false;
    }

    return true;
}

std::map<std::string, std::vector<std::string>> csv_to_map(
    const std::string &file_name,
    bool (*row_schema_test)(const std::vector<std::string>&)
)
{
    std::map<std::string, std::vector<std::string>> csv_map;

    std::string in_file_contents = read_file(file_name);
    std::vector<std::string> in_rows;
    split<std::vector<std::string>>(in_file_contents, '\n', in_rows);

    // Initialise the std::map with pairs of header values and empty std::vectors.
    std::vector<std::string> headers;
    split<std::vector<std::string>>(*in_rows.begin(), ',', headers);

    for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
        csv_map.insert({*it, std::vector<std::string>()});

    // Fill the std::vectors associated with each header with the corresponding
    // values, start iterating from 1 to avoid reading header into std::vectors.
    for (size_t i = 1; i != in_rows.size(); i++)
    {
        std::vector<std::string> line;
        if(!in_rows[i].empty())
            split(in_rows[i], ',', line);
        else
            continue;

        if (row_schema_test == NULL || row_schema_test(line))
        {
            for (size_t j = 0; j != line.size(); ++j)
            {
                csv_map[headers[j]].push_back(line[j]);
            }
        }
        else
        {
            continue;
        }
    }

    return csv_map;
}
