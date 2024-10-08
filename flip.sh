#!/bin/bash
echo "hello world:admin-true:" > msg
./submit < msg
python3 bitflip.py
python3 verify.py < message
