import sys
import fileinput

from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

decrypted = ""

data = sys.stdin.buffer.read()

key = "0102030405060708090a0b0c0d0e0f10"
iv = "1112131415161718191a1b1c1d1e1f20" 
cipher = AES.new(bytearray.fromhex(key), AES.MODE_CBC, bytearray.fromhex(iv))

decrypted = str(cipher.decrypt(data))


print(";admin=true;" in decrypted)

