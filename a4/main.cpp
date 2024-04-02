#include "EllipticCurve.hpp"
#include <NTL/ZZ.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
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

std::vector<std::pair<Point, Point>>
read_elgamal_curve(const std::string &filename) {
  std::vector<NTL::ZZ> raw = read(filename);
  std::vector<std::pair<Point, Point>> result;
  for (int i = 0; i < raw.size(); i += 4) {
    Point p1;
    p1.x = raw[i];
    p1.y = raw[i + 1];

    Point p2;
    p2.x = raw[i + 2];
    p2.y = raw[i + 3];

    result.push_back(std::make_pair(p1, p2));
  }
  return result;
}

int main() {
  auto cipher_halfmask =
      read_elgamal_curve("/home/aj/Documents/Crypto/a4/a4.cipher_halfmask");

  // p, A, B, G_x, G_y, P_x, P_y, N
  auto keys = read("/home/aj/Documents/Crypto/a4/a4.keys");
  EllipticCurve e;
  e.q = keys[0];
  e.A = keys[1];
  e.B = keys[2];

  Point G;
  G.x = keys[3];
  G.y = keys[4];
  e.G = G;

  Point P;
  P.x = keys[5];
  P.y = keys[6];
  e.P = P;

  e.N = keys[7];

  for (auto pair : cipher_halfmask) {
    Point m = e.decrypt(pair.first, pair.second);
    std::cout << (char)NTL::conv<long>(m.x); //<< (char)NTL::conv<long>(m.y);
  }
  std::cout << std::endl;
}
