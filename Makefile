all: bmp_encrypt submit

bmp_encrypt: bmp.c bmp.h aes.c aes.h box.c box.h
	gcc -g bmp.c bmp.h aes.h aes.c box.c box.h -o bmp_encrypt

submit: aes.c aes.h box.c box.h submit.c
	gcc -g aes.c aes.h box.c box.h submit.c -o submit

