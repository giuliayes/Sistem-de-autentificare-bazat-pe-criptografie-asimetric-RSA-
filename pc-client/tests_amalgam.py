import serial, struct

PORT = "COM3"       # Zybo COM port
BAUD = 115200

# --- RSA keys ---
p  = 0xe2176ecf
q  = 0xc3a906c9
n  = 0xaccd20dde4a5da87
e  = 0x0000000000010001      # 0x10001 = 65537
d  = 0x15bbfb3d18801bd1      # private exponent - pe pc

def modexp(base, exp, mod):
    res = 1
    base %= mod
    while exp:
        if exp & 1:
            res = (res * base) % mod
        exp >>= 1
        base = (base * base) % mod
    return res

def crc16(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ 0x1021
            else:
                crc <<= 1
        crc &= 0xFFFF
    return crc

def send_msg(ser, mtype, payload: bytes):
    hdr = bytes([0xAA, mtype, (len(payload)>>8)&0xFF, len(payload)&0xFF])
    crc = crc16(hdr[1:]) ^ crc16(payload)
    ser.write(hdr + payload + struct.pack(">H", crc))

def recv_msg(ser):
    while True:
        b = ser.read(1)
        if not b:
            return None
        if b[0] == 0xAA:
            break
    hdr = ser.read(3)
    if len(hdr) < 3:
        return None
    mtype, lhi, llo = hdr
    length = (lhi<<8) | llo
    payload = ser.read(length)
    crc_raw = ser.read(2)
    if len(crc_raw) < 2:
        return None
    crc_recv, = struct.unpack(">H", crc_raw)
    crc = crc16(bytes([mtype,lhi,llo])) ^ crc16(payload)
    if crc != crc_recv:
        print("CRC error")
        return None
    return mtype, payload

# Define what each test does
# "good"      -> correct signature
# "flip"      -> flip 1 bit of sig
# "wrong_d"   -> sign with d+1 (wrong key)
# "zero"      -> sig = 0
test_modes = [
    "good",     # 1
    "good",     # 2
    "good",     # 3
    "good",     # 4
    "good",     # 5
    "flip",     # 6 -> bad
    "wrong_d",  # 7 -> bad
    "zero",     # 8 -> bad
    "good",     # 9
    "good",     # 10
]

with serial.Serial(PORT, BAUD, timeout=1) as ser:
    for i, mode in enumerate(test_modes, start=1):
        print(f"\n=== Test {i} ({mode}) ===")

        # 1. send AUTH_REQUEST
        send_msg(ser, 0x01, b"")
        print("AUTH_REQUEST sent")

        # 2. receive CHALLENGE
        m = recv_msg(ser)
        if not m or m[0] != 0x02:
            print("No challenge received, got:", m)
            break
        chall = int.from_bytes(m[1], "big")
        print("Challenge:", hex(chall))

        # 3. compute base correct signature
        sig = modexp(chall, d, n)

        # apply corruption depending on mode
        if mode == "flip":
            print("!! FLIPPING lowest bit of signature !!")
            sig ^= 1

        elif mode == "wrong_d":
            print("!! Using WRONG private key d+1 !!")
            sig = modexp(chall, d + 1, n)

        elif mode == "zero":
            print("!! Using ZERO as signature !!")
            sig = 0

        # else "good": leave sig as-is

        sig_bytes = sig.to_bytes(8, "big")
        send_msg(ser, 0x03, sig_bytes)
        print("AUTH_RESPONSE sent (sig =", hex(sig), ")")

        # 4. receive result
        m = recv_msg(ser)
        if not m or m[0] != 0x04:
            print("No result received, got:", m)
            break
        print("AUTH", "OK" if m[1][0] == 1 else "FAIL")
