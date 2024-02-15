#include <NTL/ZZ.h>
#include <vector>
/*
 * RSA Set up:
 * 1) Chose 2 k-bit prime numbers p, q where p != q [Miller-Rabin]
 * 2) N = p*q, phi = (p-1)(q-1)
 * 3) Choose two integers e and d s.t.
 * ed = 1 (mod phi) [Pulveriser]
 *
 * Enc(x) = x**e (mod n)
 * Dec(x) = x**d (mod n)
 *
 * Public Keys: n, e
 * Private Keys: p, q, phi, k, d
 */
struct RSA {
  NTL::ZZ p;
  NTL::ZZ q;
  NTL::ZZ n;
  NTL::ZZ phi;
  NTL::ZZ e;
  NTL::ZZ d;
  RSA(const long k);
  void write_pubkeys(const std::string &filename);
  void write_privatekeys(const std::string &filename);
  NTL::ZZ encrypt(const NTL::ZZ &m);
  NTL::ZZ decrypt(const NTL::ZZ &c);

  std::vector<NTL::ZZ> encrypt(const std::string &message);
  std::string decrypt(const std::vector<NTL::ZZ> &cipher);
};

NTL::ZZ gcd(const NTL::ZZ &a, const NTL::ZZ &b);

NTL::ZZ expmod_rabin(const NTL::ZZ &a, const NTL::ZZ &b, const NTL::ZZ &m);

NTL::ZZ expmod_rabin(const NTL::ZZ &a, const NTL::ZZ &b, const NTL::ZZ &m,
                     const NTL::ZZ &v);

bool miller_rabin(const NTL::ZZ &m, const long &L);

std::pair<NTL::ZZ, NTL::ZZ> pulveriser(const NTL::ZZ &a, const NTL::ZZ &b,
                                       const NTL::ZZ &x1, const NTL::ZZ &y1,
                                       const NTL::ZZ &x2, const NTL::ZZ &y2);

std::pair<NTL::ZZ, NTL::ZZ> pulveriser(const NTL::ZZ &a, const NTL::ZZ &b);

void generate_prime(NTL::ZZ &result, const long k);

std::pair<NTL::ZZ, NTL::ZZ> generate_p_q(const long k);
