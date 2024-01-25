#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T> T dot(const std::vector<T> &a, const std::vector<T> &b) {
  if (a.size() != b.size())
    throw std::runtime_error("dot(): len(a) != len(b)");
  float sum = 0.0f;
  for (int i = 0; i < a.size(); i++)
    sum += a[i] * b[i];
  return sum;
}

std::vector<float> compute_w(const std::string &cipher, unsigned int shift,
                             int key_len) {
  std::vector<float> v(26, 0);
  int count = 0;
  for (int i = shift; i < cipher.length(); i += key_len) {
    int index = cipher[i] - 65;
    v[index]++;
    count++;
  }
  std::vector<float> w;
  for (auto x : v) {
    w.push_back(x / (count * 1.f));
  }
  return w;
}

std::map<unsigned int, std::vector<float>> compute_As(const std::string &cipher,
                                                      int key_len) {
  std::map<unsigned int, std::vector<float>> data;
  std::vector<float> a_0 = {.082, .015, .028, .043, .127, .022, .020,
                            .061, .070, .002, .008, .040, .024, .067,
                            .075, .019, .001, .060, .063, .091, .028,
                            .010, .023, .001, .020, .001};
  data.insert({0, a_0});
  for (int i = 1; i < 26; i++) {
    int shift = 26 - i;
    std::vector<float> a_i(a_0);
    std::rotate(a_i.begin(), a_i.begin() + shift, a_i.end());
    data.insert({i, a_i});
  }
  return data;
}

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

std::string guess_key(const std::string &cipher, int key_len) {
  typedef std::map<unsigned int, std::vector<float>> A_map;
  A_map A = compute_As(cipher, key_len);
  std::string key = "";
  for (int j = 0; j < key_len; j++) {
    std::vector<float> w = compute_w(cipher, j, key_len);
    std::vector<float> w_dot_a;
    // Dot product is not correct, w seems to be okay
    // Investigate A
    for (A_map::iterator it = A.begin(); it != A.end(); ++it) {
      w_dot_a.push_back(dot(w, it->second));
    }
    int max_pos = std::distance(
        w_dot_a.begin(), std::max_element(w_dot_a.begin(), w_dot_a.end()));

    key.push_back((char)(max_pos + 65));
  }
  return key;
}

std::string decrypt(const std::string &cipher, const std::string &key) {
  std::string message = "";
  int key_cur = 0;
  for (auto c : cipher) {
    int diff = c - key[key_cur];
    if (diff < 0)
      diff = 26 + diff;
    message.push_back(diff + 65);
    key_cur++;
    key_cur = key_cur % key.length();
  }
  return message;
}

std::string read_file(const std::string &file_name) {
  std::ifstream file;
  file.open(file_name);
  std::stringstream strStream;
  strStream << file.rdbuf();
  return strStream.str();
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: ./poly_cipher <cipher file name>" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string cipher = read_file(argv[1]);
  cipher.erase(
      std::remove_if(cipher.begin(), cipher.end(),
                     [](auto const &c) -> bool { return !std::isalnum(c); }),
      cipher.end());
  std::cout << "Cipher: " << cipher << std::endl;
  int estimated_key_len = determine_key_length(cipher);
  std::string estimated_key = guess_key(cipher, estimated_key_len);
  std::cout << "Estimated Key: " << estimated_key << std::endl;
  std::string message = decrypt(cipher, estimated_key);
  std::cout << "Message: " << message << std::endl;
}
