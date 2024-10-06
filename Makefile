all: aes submit

aes: main.c main.h aes.c aes.h box.c box.h
	gcc -g main.c main.h aes.h aes.c box.c box.h -o aes

submit: main.c main.h aes.c aes.h box.c box.h submit.c
	gcc -g aes.c aes.h box.c box.h submit.c -o submit
