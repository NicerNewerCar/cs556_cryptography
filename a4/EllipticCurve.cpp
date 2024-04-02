#include "EllipticCurve.hpp"
#include <NTL/ZZ.h>
#include <NTL/sp_arith.h>

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

Point EllipticCurve::add(const Point &p1, const Point &p2) {
  if (p1.x == p2.x) {
    if (p1.y == p2.y) { // Point Doubling
      NTL::ZZ M_nume =
          (NTL::ZZ(3) * NTL::SqrMod(p1.x, this->q) + this->A) % this->q;
      NTL::ZZ M_denom = (NTL::ZZ(2) * p1.y) % this->q;
      NTL::ZZ m = (M_nume * pulveriser(this->q, M_denom).second) %
                  this->q; // m = (3x1^2 + A) * (2y1) ^ -1 (mod q)
      NTL::ZZ c = (p1.y - m * p1.x) % this->q; // c = y1-m*x1 (mod q)
      NTL::ZZ x3 = (NTL::SqrMod(m, this->q) - NTL::ZZ(2) * p1.x) %
                   this->q;                // x3 = m^2 - 2x1 (mod q)
      NTL::ZZ y3 = (m * x3 + c) % this->q; // y3 = m*x3 + c (mod q)

      Point p3;
      p3.x = x3;
      p3.y = NTL::NegateMod(y3, this->q);
      return p3;
    } else if (p1.y == NTL::NegateMod(p2.y, this->q)) {
      Point p;
      return p; // Point at infinity
    }
  }
  NTL::ZZ M_nume = p2.y - p1.y;
  NTL::ZZ M_denom = (p2.x - p1.x) % this->q;
  NTL::ZZ m = (M_nume * pulveriser(this->q, M_denom).second) %
              this->q;                     // m = (y2-y1) * (x2-x1) ^ -1 (mod q)
  NTL::ZZ c = (p1.y - m * p1.x) % this->q; // c = y1-m*x1 (mod q)
  NTL::ZZ x3 = (NTL::SqrMod(m, this->q) - (p1.x + p2.x)) %
               this->q;                // x3 = m^2 - (x1+x2) (mod q)
  NTL::ZZ y3 = (m * x3 + c) % this->q; // y3 = m*x3 + c (mod q)

  Point p3;
  p3.x = x3;
  p3.y = NTL::NegateMod(y3, this->q);
  return p3;
}

Point EllipticCurve::decrypt(const Point &cipher, const Point &halfmask) {
  Point fullmask = this->add(halfmask, halfmask);
  for (long i = 2; i < this->N; i++) {
    fullmask = this->add(fullmask, halfmask);
  }
  fullmask.y = NTL::NegateMod(fullmask.y, this->q);
  return this->add(cipher, fullmask);
}
