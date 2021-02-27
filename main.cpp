// reading a text file
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

std::string str_split(const std::string &str)
{
  std::vector<std::string> result;
  // array of strings
  std::string temp_str;
  for (char c : str)
  {
    if (c != ',')
    {
      temp_str += c;
    }
    else
    {
      result.push_back(temp_str);
      temp_str = ' ';
    }
  }
  result.push_back(temp_str);
  for (auto i : result)
  {
    // std::cout << "res  " << i << "\n";
  }
  std::cout << result[2] << std::endl;
  return temp_str;
}

void read_csv()
{
  std::string file_name = "test_data/test.txt";

  // attach an input stream to the wanted file
  std::ifstream input_stream(file_name);

  // check stream status
  if (!input_stream)
    std::cerr << "Can't open input file!";
  if (input_stream)
  {
    // std::cout << "correct";
  }
  // file contents
  std::vector<std::string> text;
  // one line
  std::string line;

  // extract all the text from the input file
  while (std::getline(input_stream, line))
  {
    // store each line in the vector
    text.push_back(line);
  }
  for (size_t i = 1; i < text.size(); ++i)
  {
    str_split(text[i]);
  }
}

int main()
{
  read_csv();
  return 0;
}
class WordDelimitedByCommas : public std::string
{
};