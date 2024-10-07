all: bmp_encrpyt submit verify

bmp_encrpyt: bmp.c bmp.h aes.c aes.h box.c box.h
	gcc -g bmp.c bmp.h aes.h aes.c box.c box.h -o bmp_encrpyt

submit: aes.c aes.h box.c box.h submit.c
	gcc -g aes.c aes.h box.c box.h submit.c -o submit

verify: aes.c aes.h box.c box.h verify.c
	gcc -g aes.c aes.h box.c box.h verify.c -o verify
