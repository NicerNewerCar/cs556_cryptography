import os
import socket
import threading

import cryptography
from OpenSSL import crypto


def pad(string: str):
    size = int(256 / 8)
    if len(string) > size:
        return string[0:size]
    diff = size - len(string)
    return string + b"\0" * diff


def handle_client(client_socket):
    # Listen for bitlength
    bitlength = int(client_socket.recv(1024))
    print("Bit length received: ", bitlength)
    k = crypto.PKey()
    k.generate_key(crypto.TYPE_RSA, bitlength)
    print("Sending public keys...")
    client_socket.sendall(crypto.dump_publickey(crypto.FILETYPE_PEM, k))

    crypto_key = k.to_cryptography_key()
    hash_ = cryptography.hazmat.primitives.hashes.SHA256()
    mfg = cryptography.hazmat.primitives.asymmetric.padding.MGF1(hash_)
    padding = cryptography.hazmat.primitives.asymmetric.padding.OAEP(mfg, hash_, None)

    encrypyted_aes_string = client_socket.recv(1024)
    aes_string = crypto_key.decrypt(encrypyted_aes_string, padding)
    print("Received AES key: ", encrypyted_aes_string)
    print("Decrypted AES key", aes_string)
    aes = cryptography.hazmat.primitives.ciphers.algorithms.AES(pad(aes_string))

    iv = ("\0" * 16).encode("utf-8")
    decryptor = cryptography.hazmat.primitives.ciphers.Cipher(
        aes, mode=cryptography.hazmat.primitives.ciphers.modes.OFB(iv)
    ).decryptor()

    while True:
        data = client_socket.recv(1024)
        if not data:
            break
        decrypted = decryptor.update(data)
        print(f"Received message: {data}")
        print(f"Decrypted message: {decrypted}")
    client_socket.close()


def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host = "127.0.0.1"
    port = 12346
    server_socket.bind((host, port))
    server_socket.listen(5)
    print(f"Server listening on {host}:{port}")

    while True:
        client_socket, client_address = server_socket.accept()
        print(f"Accepted connection from {client_address}")
        client_handler = threading.Thread(target=handle_client, args=(client_socket,))
        client_handler.start()


if __name__ == "__main__":
    main()
