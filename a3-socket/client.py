import os
import socket

import cryptography
from OpenSSL import crypto

# Server sends RSA public keys
# Client send encrypted string as the passphrase for DES


def pad(string: str):
    size = int(256 / 8)
    if len(string) > size:
        return string[0:size]
    diff = size - len(string)
    return string + b"\0" * diff


def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host = "127.0.0.1"
    port = 12346
    client_socket.connect((host, port))
    # Set up phase
    bit_length = input("RSA bitlength: ")
    client_socket.sendall(bit_length.encode("utf-8"))
    publickey = client_socket.recv(1024)
    print("public keys recieved")
    k = crypto.load_publickey(crypto.FILETYPE_PEM, publickey)

    crypto_key = k.to_cryptography_key()
    hash_ = cryptography.hazmat.primitives.hashes.SHA256()
    mfg = cryptography.hazmat.primitives.asymmetric.padding.MGF1(hash_)
    padding = cryptography.hazmat.primitives.asymmetric.padding.OAEP(mfg, hash_, None)

    aes_string = input("Secret string for AES: ")
    aes_string = aes_string.encode("utf-8")
    encrpyted_message = crypto_key.encrypt(aes_string, padding)
    client_socket.sendall(encrpyted_message)

    aes = cryptography.hazmat.primitives.ciphers.algorithms.AES(pad(aes_string))

    iv = ("\0" * 16).encode("utf-8")
    encryptor = cryptography.hazmat.primitives.ciphers.Cipher(
        aes, mode=cryptography.hazmat.primitives.ciphers.modes.OFB(iv)
    ).encryptor()

    while True:
        message = input("Enter your message: ")
        client_socket.sendall(encryptor.update(message.encode("utf-8")))


if __name__ == "__main__":
    main()
