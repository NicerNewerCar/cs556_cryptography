#include "rsa.hpp"
#include <NTL/ZZ.h>
#include <fstream>
#include <stdexcept>
#include <thread>
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

NTL::ZZ gcd(const NTL::ZZ &a, const NTL::ZZ &b) {
  if (NTL::IsZero(b))
    return a;
  return gcd(b, a % b);
}

NTL::ZZ expmod_rabin(const NTL::ZZ &a, const NTL::ZZ &b, const NTL::ZZ &m,
                     const NTL::ZZ &v) {
  if (NTL::IsZero(b))
    return v;                // Return the vairence
  else if (!NTL::IsOdd(b)) { // Even path

    NTL::ZZ a_squared = NTL::PowerMod(a, NTL::ZZ(2), m);
    if (NTL::IsOne(a_squared) && !NTL::IsOne(a % m) && (a % m) != (m - 1))
      return NTL::ZZ(0); // Rabin Test
    return expmod_rabin(a_squared, b / 2, m, v);
  } else { // Odd path
    NTL::ZZ va = (v * a) % m;
    return expmod_rabin(a, b - 1, m, va);
  }
}

NTL::ZZ expmod_rabin(const NTL::ZZ &a, const NTL::ZZ &b, const NTL::ZZ &m) {
  return expmod_rabin(a, b, m, NTL::ZZ(1));
}

bool miller_rabin(const NTL::ZZ &m, const long &L) {
  for (int i = 0; i < L; i++) {
    NTL::ZZ a = NTL::RandomBnd(m - 1) + 1; // a = [1, m-1]
    if (expmod_rabin(a, m, m) != a)
      return false;
  }
  return true;
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

void generate_prime(NTL::ZZ &result, const long k) {
  // Generates a single k-bit prime number
  NTL::ZZ p;
  while (true) {
    p = NTL::RandomBits_ZZ(k);
    if (!NTL::IsOdd(p))
      NTL::add(p, p, 1);
    if (miller_rabin(p, 80)) {
      result = p;
      return;
    }
  }
}

std::pair<NTL::ZZ, NTL::ZZ> generate_p_q(const long k) {
  std::cout << "Generating distinct primes with bit-length " << k << "..."
            << std::endl;
  // Generate two k-bit primes, p and q
  NTL::ZZ p;
  NTL::ZZ q;
  // Generate p and q at the same time
  std::thread p_thread(generate_prime, std::ref(p), k);
  std::thread q_thread(generate_prime, std::ref(q), k);

  // Wait for threads to finish
  p_thread.join();
  q_thread.join();

  if (p != q) // Check if they are distinct
    return std::make_pair(p, q);
  while (true) { // If not keep regenerating q until they are.
    NTL::GenPrime(q, k);
    if (p != q)
      return std::make_pair(p, q);
  }
}

NTL::ZZ compute_e(const NTL::ZZ &phi, const long &len) {
  NTL::ZZ e;
  NTL::RandomLen(e, len);
  while (true) {
    if (gcd(phi, e) == 1)
      return e;
    NTL::RandomLen(e, len);
  }
}

RSA::RSA(const long k) {
  // Step 1
  std::pair<NTL::ZZ, NTL::ZZ> pq = generate_p_q(k);
  this->p = pq.first;
  this->q = pq.second;

  // Step 2
  this->n = this->p * this->q;
  this->phi = (this->p - 1) * (this->q - 1);

  // Step 3
  this->e = compute_e(this->phi, k / 3);
  this->d = pulveriser(this->phi, this->e).second;
}

NTL::ZZ RSA::encrypt(const NTL::ZZ &m) {
  return NTL::PowerMod(m, this->e, this->n);
}

NTL::ZZ RSA::decrypt(const NTL::ZZ &c) {
  return NTL::PowerMod(c, this->d, this->n);
}

std::vector<NTL::ZZ> RSA::encrypt(const std::string &message) {
  std::vector<NTL::ZZ> res;
  for (char c : message)
    res.push_back(this->encrypt(NTL::ZZ((long)c)));
  return res;
}

std::string RSA::decrypt(const std::vector<NTL::ZZ> &cipher) {
  std::string res;
  for (NTL::ZZ i : cipher)
    res += (char)NTL::conv<long>(this->decrypt(i));
  return res;
}

void RSA::write_pubkeys(const std::string &filename) {
  std::ofstream file(filename, std::ios::out);
  if (!file) {
    throw std::runtime_error("Failed to open file " + filename +
                             " for writing.");
  }
  file << this->n << "\n";
  file << this->e << "\n";
  file.close();
}

void RSA::write_privatekeys(const std::string &filename) {
  std::ofstream file(filename, std::ios::out);
  if (!file) {
    throw std::runtime_error("Failed to open file " + filename +
                             " for writing.");
  }
  file << this->d << "\n";
  file << this->phi << "\n";
  file << this->p << "\n";
  file << this->q << "\n";
  file.close();
}
