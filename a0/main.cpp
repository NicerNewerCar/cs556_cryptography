#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

int determine_key_length(const std::string &s) {
  int max_key_length = 6;
  typedef std::map<int, int> mapType;
  mapType data;

  for (int shift = 1; shift <= max_key_length; shift++) {
    int num_coincidences = 0;
    for (int i = shift; i < s.length(); i++) {
      if (s[i] == s[i - shift])
        num_coincidences++;
    }
    data.insert({shift, num_coincidences});
  }

  mapType::iterator it = std::max_element(
      std::begin(data), std::end(data),
      [](const std::pair<int, int> &p1, const std::pair<int, int> &p2) {
        return p1.second < p2.second;
      });
  return it->first;
}

std::string guess_key(std::string cipher, int key_len) {
  std::vector<int> v(26, 0);
  int count = 0;
  for (int i = 0; i < cipher.length(); i += key_len) {
    int index = cipher[i] - 65;
    v[index]++;
    count++;
  }
  std::vector<float> w;
  for (auto x : v) {
    w.push_back(x / (count * 1.f));
  }

  return "";
}

std::string read_file(const std::string &file_name) {
  std::ifstream file;
  file.open(file_name);
  std::stringstream strStream;
  strStream << file.rdbuf();
  return strStream.str();
}

int main() {
  std::string cipher = read_file("/home/aj/Documents/Crypto/a0/cipher.txt");
  std::cout << cipher << std::endl;
  cipher.erase(
      std::remove_if(cipher.begin(), cipher.end(),
                     [](auto const &c) -> bool { return !std::isalnum(c); }),
      cipher.end());
  std::cout << cipher << std::endl;
  int estimated_key_len = determine_key_length(cipher);
  std::string estimated_key = guess_key(cipher, estimated_key_len);
}
