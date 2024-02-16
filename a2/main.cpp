#include "rsa.hpp"
#include <NTL/ZZ.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void num2str(const NTL::ZZ &m) {
  unsigned char *pp = new unsigned char[NTL::NumBytes(m)];
  BytesFromZZ(pp, m, NTL::NumBytes(m));
  std::cout << pp << std::endl;
}

std::pair<NTL::ZZ, NTL::ZZ> get_pq(const NTL::ZZ &n, const NTL::ZZ &phi) {
  NTL::ZZ p_plus_q = n - phi + NTL::ZZ(1);
  NTL::ZZ p_minus_q = NTL::SqrRoot(NTL::power(p_plus_q, 2) - (4 * n));
  NTL::ZZ double_p = p_plus_q + p_minus_q;
  NTL::ZZ double_q = p_plus_q - p_minus_q;
  return std::make_pair(double_p / 2, double_q / 2);
}

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

int main(int argc, char *argv[]) {

  NTL::ZZ cipher = read("/home/aj/Documents/Crypto/a2/a2.cipher")[0];
  // One number hiding larger amount of letters
  // Each letter is 8-bits
  std::vector<NTL::ZZ> ne = read("/home/aj/Documents/Crypto/a2/a2.pubkeys");
  NTL::ZZ n = ne[0];
  NTL::ZZ e = ne[1];
  NTL::ZZ phi = read("/home/aj/Documents/Crypto/a2/a2.hint")[0];
  std::pair<NTL::ZZ, NTL::ZZ> pq = get_pq(n, phi);
  // std::cout << "p: " << pq.first << " q: " << pq.second << std::endl;
  std::cout << "Validate with n: " << (n == pq.first * pq.second) << std::endl;
  std::cout << "Validate with phi: "
            << (phi == (pq.first - 1) * (pq.second - 1)) << std::endl;

  NTL::ZZ d = pulveriser(phi, e).second;

  RSA r;
  r.phi = phi;
  r.n = n;
  r.e = e;
  r.d = d;
  r.p = pq.first;
  r.q = pq.second;

  NTL::ZZ message = r.decrypt(cipher);

  num2str(message);

  return 0;
}
