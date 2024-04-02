#include <NTL/ZZ.h>

struct Point {
  NTL::ZZ x;
  NTL::ZZ y;

  void negate(){};
};

struct EllipticCurve {
  // Public keys
  NTL::ZZ q;
  NTL::ZZ A;
  NTL::ZZ B;
  Point G;
  Point P;

  // Secret
  NTL::ZZ N;

  // Functions
  Point add(const Point &p1, const Point &p2);
  Point decrypt(const Point &cipher, const Point &halfmask);
};
