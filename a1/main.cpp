#include "lib.hpp"
#include <iostream>
#include <utility>

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

NTL::ZZ generate_prime(const long k) {
  // Generates a single k-bit prime number
  NTL::ZZ p;
  while (true) {
    p = NTL::RandomBits_ZZ(k);
    std::cout << "Random p: " << p << std::endl;
    if (miller_rabin(p, 80))
      return p;
  }
}

std::pair<NTL::ZZ, NTL::ZZ> generate_p_q(const long k) {
  // Generate two k-bit primes, p and q
  NTL::ZZ p = generate_prime(k);
  NTL::ZZ q;
  while (true) {
    q = generate_prime(k);
    if (p != q)
      return std::make_pair(p, q);
  }
}

int main(int argc, char *argv[]) {
  // auto res = generate_p_q(50);
  // std::cout << res.first << ' ' << res.second << std::endl;
  NTL::ZZ p = generate_prime(50);
  std::cout << p << std::endl;

  return 0;
}
