

data = ""

with open("message", "rb") as f:
    data = bytearray(f.read())

data[27] = data[27] ^ 1
data[22] = data[22] ^ (ord("=") ^ ord("-"))
data[16] = data[16] ^ 1
with open("message", "wb") as f:
    f.write(bytes(data))



