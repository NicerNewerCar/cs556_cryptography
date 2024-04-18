import socket

from OpenSSL import crypto

# Server sends RSA public keys
# Client send encrypted string as the passphrase for DES


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

    while True:
        message = input("Enter your message: ")
        client_socket.sendall(message.encode("utf-8"))
        data = client_socket.recv(1024)
        response = data.decode("utf-8")
        print(f"Server response: {response}")


if __name__ == "__main__":
    main()
