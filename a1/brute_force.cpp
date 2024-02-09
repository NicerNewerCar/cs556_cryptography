#include "brute_force.hpp"
#include <NTL/ZZ.h>
#include <fstream>
#include <map>
#include <vector>
typedef std::map<NTL::ZZ, long> tmap;

tmap generate_acii_map(const NTL::ZZ &n, const NTL::ZZ &e) {
  tmap map;
  for (long i = 0; i < 256; i++) {
    NTL::ZZ enc = NTL::PowerMod(NTL::ZZ(i), e, n);
    map.insert({enc, i});
  }
  return map;
}

std::vector<NTL::ZZ> read(const std::string &filename) {
  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  NTL::ZZ line;
  std::vector<NTL::ZZ> res;
  while (file >> line)
    res.push_back(line);
  return res;
}

std::string decrypt_message(const std::vector<NTL::ZZ> &cipher,
                            const tmap &ascii_map) {
  std::string message;
  for (NTL::ZZ c : cipher) {
    long m = ascii_map.at(c);
    message += (char)m;
  }
  return message;
}

std::string brute_force(const std::string &cipher_fname,
                        const std::string &pub_fname) {

  auto cipher = read("/home/aj/Documents/Crypto/a1/a1.cipher");
  auto pub_keys = read("/home/aj/Documents/Crypto/a1/a1.pubkeys");
  NTL::ZZ n = pub_keys.at(0);
  NTL::ZZ e = pub_keys.at(1);

  tmap ascii_map = generate_acii_map(n, e);

  return decrypt_message(cipher, ascii_map);
}
