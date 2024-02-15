#include "brute_force.hpp"
#include "rsa.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[]) {
  std::string message = brute_force("/home/aj/Documents/Crypto/a1/a1.cipher",
                                    "/home/aj/Documents/Crypto/a1/a1.pubkeys");
  std::cout << "Original Message:\n";
  std::cout << message << std::endl;

  RSA r = RSA(1024); // 1024-bit

  std::vector<NTL::ZZ> cipher = r.encrypt(message);
  std::string test_decrypt = r.decrypt(cipher);

  std::cout
      << "\n\nTest encrypt then decrypt (This should be the same as above):\n";
  std::cout << test_decrypt << std::endl;

  r.write_privatekeys("my.privatekeys");
  r.write_pubkeys("my.pubkeys");

  std::ofstream file("my.cipher", std::ios::out);
  if (!file) {
    throw std::runtime_error("Failed to open file my.cipher");
  }
  for (NTL::ZZ c : cipher)
    file << c << "\n";
  file.close();

  return 0;
}
