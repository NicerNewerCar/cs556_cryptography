#include <NTL/ZZ.h>
#include <utility>

template <typename T> T gcd(const T &a, const T &b) {
  if (b == T(0))
    return a;
  return gcd(b, a % b);
}

// TODO: Make this tail recursive
NTL::ZZ expmod_rabin(const NTL::ZZ &a, const NTL::ZZ &b, const NTL::ZZ &m) {
  if (NTL::IsZero(b))
    return NTL::ZZ(1);
  else if (NTL::IsZero(b % NTL::ZZ(2))) { // b is even
    NTL::ZZ y = expmod_rabin(a, b / 2, m);
    NTL::ZZ z = NTL::power(y, 2) % m;
    if (NTL::IsOne(z) && (!NTL::IsOne(y % m) || (y % m) != (m - 1)))
      return NTL::ZZ(0);
    return z;
  } else { // b is odd
    NTL::ZZ y = expmod_rabin(a, b - 1, m);
    return (y * a) % m;
  }
}

bool miller_rabin(const NTL::ZZ &m, const long &L) {
  for (int i = 0; i < L; i++) {
    NTL::ZZ a = NTL::RandomBnd(m - 1) + 1; // a = [1, m-1]
    if (expmod_rabin(a, m, m) != a)
      return false;
  }
  return true;
}

template <typename T>
std::pair<T, T> pulveriser(const T &a, const T &b, const T &x1, const T &y1,
                           const T &x2, const T &y2) {
  if (b == 0)
    return std::make_pair(x1, y1);
  T q = T(std::floor(a / b));
  T x_prime = x1 - q * x2;
  T y_prime = y1 - q * y2;
  return pulveriser(b, a % b, x2, y2, x_prime, y_prime);
}

template <typename T> std::pair<T, T> pulveriser(const T &a, const T &b) {
  return pulveriser(a, b, T(1), T(0), T(0), T(1));
}
