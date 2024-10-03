#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"


#define ECB 0x01
#define CBC 0x02
#define IS_ECB(F) ((F | ECB) != 0)
#define IS_CBC(F) ((F | CBC) != 0)

int main(int argc, char* argp[]){
	if (argc < 3 || argp[2][0] != '-') {
		printf("Invalid format: %s file (-e | -c) [output_file]\n", argp[0]);
	       return -1;	
	}
	FILE* data = fopen(argp[1], "r");
	char type = 0;

	if (data == NULL){
		printf("File '%s' does not exist\n", argp[1]);
		return -1;
	}
	if (strcmp(argp[2], "-e") == 0) {
		type = ECB;
	}
	if (strcmp(argp[2], "-c") == 0) {
		type = CBC;
	}
	if (type == 0) {
		printf("Invalid encryption type: %s\n", argp[2]);
		return -1;
	}
	generate_key(16);
}

char* generate_key(int byte_len) {

	char *key = calloc(byte_len, 1);
	
	char i;
	srand(time(NULL));
	for (i = 0; i < byte_len; i++) {
		char c = (rand() % 256);
		key[i] = c;
	}
	return key;
}
