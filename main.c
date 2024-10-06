#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"
#include "aes.h"

#define ECB 0x01
#define CBC 0x02
#define IS_ECB(F) ((F & ECB) != 0)
#define IS_CBC(F) ((F & CBC) != 0)

int main(int argc, char* argp[]){
	if (argc < 3 || argp[2][0] != '-') {
		printf("Invalid format: %s file (-e | -c) [output_file]\n", argp[0]);
	       return -1;	
	}
	FILE* data = fopen(argp[1], "r");
	FILE* output;
	char type = 0;

	if (data == NULL){
		printf("File '%s' does not exist\n", argp[1]);
		return -1;
	}
	if (strcmp(argp[2], "-e") == 0) {
		type = ECB;
	}
	else if (strcmp(argp[2], "-c") == 0) {
		type = CBC;
	}
	if (type == 0) {
		printf("Invalid encryption type: %s\n", argp[2]);
		return -1;
	}
	char *out_path;
	if (argc > 3) {
		out_path = argp[3];
	} else {
		out_path = "output";
	}
	output = fopen(out_path, "w");
	if (output == NULL) {
		printf("Cannot open %s\n", out_path);
	}
	unsigned char *key = generate_key(16);
	print_data(key, "Key");
	unsigned char *iv = NULL;
	if (IS_CBC(type)) {
		iv = generate_key(16);
		print_data(iv, "IV");
	}
	unsigned char *block = calloc(16,1);
	char read_size;	
	while ((read_size = fread(block, 1, 16, data)) > 0) {	
		for (char i = 0; i < (16 - read_size); i++) {
			block[15-i] = (16 -read_size);
		}

		if (IS_CBC(type)) {
			for(char i = 0; i < 16; i++) {
				block[i] ^= iv[i];
			}
		}
		unsigned char *out = encrypt(block,key);
		fwrite(out, 1, 16, output);

		if (IS_CBC(type)) {
			for (char i = 0; i < 16; i++) {
				iv[i] = out[i];
			}
		}
		free(out);

	}
	fclose(output);
	fclose(data);
	
	printf("Saved to %s\n", out_path);

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
