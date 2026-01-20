import random
from math import gcd

def is_prime(n):
    if n < 2: return False
    if n % 2 == 0:
        return n == 2
    i = 3
    while i * i <= n:
        if n % i == 0:
            return False
        i += 2
    return True

def rand_prime_32():
    while True:
        x = random.randrange(2**31, 2**32-1) | 1  # impar
        if is_prime(x):
            return x

def egcd(a, b):
    if b == 0:
        return 1, 0, a
    x1, y1, g = egcd(b, a % b)
    return y1, x1 - (a // b) * y1, g

def modinv(a, m):
    x, y, g = egcd(a, m)
    if g != 1:
        raise ValueError("nu exista invers modular")
    return x % m

def gen_keys():
    p = rand_prime_32()
    q = rand_prime_32()
    while q == p:
        q = rand_prime_32()

    n = p * q
    phi = (p - 1) * (q - 1)
    e = 65537
    if gcd(e, phi) != 1:
        # rareori, atunci repeți
        return gen_keys()

    d = modinv(e, phi)
    return p, q, n, e, d, phi

if __name__ == "__main__":
    p, q, n, e, d, phi = gen_keys()
    print(f"p  = {p:#x}")
    print(f"q  = {q:#x}")
    print(f"n  = {n:#x}")
    print(f"e  = {e:#x}")
    print(f"d  = {d:#x}")
    print(f"phi= {phi:#x}")
