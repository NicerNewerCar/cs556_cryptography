#include <NTL/ZZ.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<NTL::ZZ> read(const std::string &filename) {
  std::ifstream file(filename.c_str());
  if (!file) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  NTL::ZZ line;
  std::vector<NTL::ZZ> res;
  while (file >> line)
    res.push_back(line);
  return res;
}

std::vector<std::pair<NTL::ZZ, NTL::ZZ>>
read_elgamal(const std::string &filename) {
  std::ifstream file(filename.c_str());
  if (!file) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  std::string line;
  std::vector<std::pair<NTL::ZZ, NTL::ZZ>> result;
  while (file >> line) {
    std::stringstream ss(line);
    std::string substr;
    std::getline(ss, substr, ',');

    NTL::ZZ num1;
    std::stringstream substream(substr);
    substream >> num1;

    std::getline(ss, substr, ',');
    substream = std::stringstream(substr);
    NTL::ZZ num2;
    substream >> num2;

    result.push_back(std::make_pair(num1, num2));
  }
  return result;
}

std::pair<NTL::ZZ, NTL::ZZ> pulveriser(const NTL::ZZ &a, const NTL::ZZ &b,
                                       const NTL::ZZ &x1, const NTL::ZZ &y1,
                                       const NTL::ZZ &x2, const NTL::ZZ &y2) {
  if (b == 0)
    return std::make_pair(x1, y1);
  NTL::ZZ q = a / b;
  NTL::ZZ x_prime = x1 - q * x2;
  NTL::ZZ y_prime = y1 - q * y2;
  return pulveriser(b, a % b, x2, y2, x_prime, y_prime);
}

std::pair<NTL::ZZ, NTL::ZZ> pulveriser(const NTL::ZZ &a, const NTL::ZZ &b) {
  return pulveriser(a, b, NTL::ZZ(1), NTL::ZZ(0), NTL::ZZ(0), NTL::ZZ(1));
}

NTL::ZZ gcd(const NTL::ZZ &a, const NTL::ZZ &b) {
  if (NTL::IsZero(b))
    return a;
  return gcd(b, a % b);
}

NTL::ZZ babyStep_giantStep(const NTL::ZZ &a, const NTL::ZZ &b,
                           const NTL::ZZ &p) {
  // Finds x s.t. a ^ x = b (mod p)
  NTL::ZZ n = NTL::SqrRoot(p - 1);

  NTL::ZZ inverse_n = pulveriser(p, n).second;
  std::vector<NTL::ZZ> list1;
  std::vector<NTL::ZZ> list2;
  for (long i = 0; i < n; i++) {
    list1.push_back(NTL::PowerMod(a, i, p));
    list2.push_back((b * NTL::PowerMod(a, inverse_n * i, p)) % p);
  }

  for (int j = 0; j < n; j++) {
    auto it = std::find(list2.begin(), list2.end(), list1.at(j));
    if (it != list2.end()) {
      // Found the element from list1 in list2
      long k = it - list2.begin();
      return j + n * k;
    }
  }
  throw std::runtime_error("Failed to find exp!");
}

int main(int argc, char *argv[]) {
  // Prime, generator, mask
  std::vector<NTL::ZZ> pubkeys =
      read("/home/aj/Documents/Crypto/a3/a3.pubkeys");

  NTL::ZZ p = pubkeys[0];
  NTL::ZZ g = pubkeys[1];
  NTL::ZZ B = pubkeys[2];

  // Alice's secret bit is a
  // Bob's secret bit is b
  // A = g ^ a // This is the half mask
  // B = g ^ b
  // Fullmask (F) = B ^ a
  // Inverse Fullmask (F ^ -1) = Pulv(Fullmask)
  // M = C * F^-1

  // Halfmask - Cipher
  std::vector<std::pair<NTL::ZZ, NTL::ZZ>> halfmask_cipher =
      read_elgamal("/home/aj/Documents/Crypto/a3/a3.halfmask_cipher");

  // Lets calculate Bob's secret bit b
  // We know that B = g ^ b (mod p)
  NTL::ZZ b = babyStep_giantStep(g, B, p);

  return 0;
}
