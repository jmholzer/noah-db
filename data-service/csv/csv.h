#ifndef GUARD_csv_h
#define GUARD_csv_h

#include <iostream>
#include <fstream>
#include <streambuf>
#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <regex>

std::string read_file(const std::string &in_file_name);
template <class C> void split(const std::string &str, const char &delimiter, C &out);
void write_to_csv_file(const std::string &file_name, const std::string &output);
bool is_number(const std::string& s);
bool ark_schema_test(const std::vector<std::string>& row);
std::map<std::string, std::vector<std::string>> csv_to_map(
    const std::string &file_name,
    bool (*row_schema_test)(const std::vector<std::string>&) = NULL
);

#endif
