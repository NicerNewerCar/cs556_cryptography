#! /bin/python3


def pulv(a, b, x1, y1, x2, y2):
    if b == 0:
        return x1, y1
    q = a // b
    r = a % b
    nx2 = x1 - q * x2
    ny2 = y1 - q * y2
    return pulv(b, r, x2, y2, nx2, ny2)


def gcd(a, b):
    if b == 0:
        return a
    return gcd(b, a % b)


if __name__ == "__main__":
    g = 2
    p = 179
