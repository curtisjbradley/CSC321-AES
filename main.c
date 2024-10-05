#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"
#include "aes.h"

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
	//char *key = generate_key(16);
	unsigned char key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	print_data(key, "Key");
	unsigned char message[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
	print_data(message, "Message");
	unsigned char *out = encrypt(message,key);

	print_data(out, "Encrypted Text");

	free(out);

	printf("%02x\n", gf_mul(0x57, 0x13));

}

unsigned char* generate_key(int byte_len) {

	char *key = calloc(byte_len, 1);
	
	char i;
	srand(time(NULL));
	for (i = 0; i < byte_len; i++) {
		char c = (rand() % 256);
		key[i] = c;
	}
	return key;
}
