#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
float dot(const std::vector<T> &a, const std::vector<T> &b) {
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
  for (int i = 0; i < 26; i++) {
    data.insert({i, compute_w(cipher, i, key_len)});
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
  // Debug this maybe?
  std::map<unsigned int, std::vector<float>> A = compute_As(cipher, key_len);
  std::string key = "";
  for (int j = 0; j < key_len; j++) {
    std::vector<float> w = compute_w(cipher, j, key_len);
    std::vector<float> w_dot_a(26, 0);
    for (int i = 0; i < 26; i++) {
      w_dot_a[i] = dot(w, A[i]);
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

int main() {
  std::string cipher = read_file("/home/aj/Documents/Crypto/a0/cipher.txt");
  // std::cout << cipher << std::endl;
  cipher.erase(
      std::remove_if(cipher.begin(), cipher.end(),
                     [](auto const &c) -> bool { return !std::isalnum(c); }),
      cipher.end());
  std::cout << cipher << std::endl;
  int estimated_key_len = determine_key_length(cipher);
  std::string estimated_key = guess_key(cipher, estimated_key_len);
  std::cout << "Estimated Key: " << estimated_key << std::endl;
  std::string message = decrypt(cipher, estimated_key);
  std::cout << "Message: " << message << std::endl;
}
