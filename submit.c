#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aes.h"


int main (int argc, char* argp[]) {
	char key[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	char iv[] = {17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};

	print_data(key, "Key");
	print_data(iv, "IV");
	FILE *message = tmpfile();
	if (message == NULL) {
		printf("Could not create temp file\n");
		return -1;
	}
	char pre[] = "userid=456; userdata=";
	fwrite(pre, 1, strlen(pre), message);

	char c;

	while ((c = getchar()) != EOF && (c != 255)) {
		if (c == ';') {
			fwrite("%3b", 1, 3, message);
		} else if ( c == '=') {
			fwrite("%3d", 1 ,3, message);
		} else {
			fwrite(&c, 1, 1, message);
		}
	}
	char post[] = ";session-id=31337";
	fwrite(post, sizeof(char), strlen(post), message);
	fflush(message);
	rewind(message);
	FILE *encrypted = fopen("message", "w");

	if (encrypted == NULL) {
		printf("Could not create output file");
		return -1;
	}

	unsigned char *block = calloc(16,1);
        char read_size;

        while ((read_size = fread(block, 1, 16, message)) > 0) {
                for (char i = 0; i < (16 - read_size); i++) {
                        block[15-i] = (16 -read_size);
                }

                for(char i = 0; i < 16; i++) {
			block[i] ^= iv[i];
                }
                unsigned char *out = encrypt(block,key);
                fwrite(out, 1, 16, encrypted);

                for (char i = 0; i < 16; i++) {
			iv[i] = out[i];
                        
                }
                free(out);

        }

	fclose(encrypted);
	fclose(message);
	return 0;
}
