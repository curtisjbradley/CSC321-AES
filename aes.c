#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "box.h"
#include "aes.h"
#include <stdint.h>

#define BLOCK_SIZE 16
#define ROUNDS 10

// Source: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf
// AES-128 does 10 rounds


const unsigned char rcon[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
//Round Constant


unsigned char* encrypt(unsigned char* data, unsigned char* key){
	char* state = calloc(BLOCK_SIZE, 1);
	char i;
	uint32_t *expansion = key_expansion(key);
	for (i = 0; i < BLOCK_SIZE; i++) {
		state[i] = data[i];
	}
	for (int i = 0; i < 44; i++) {
		printf("%08x - %d\n", expansion[i], i);
	}
	add_round_key(state, expansion);
	for (i = 1; i < ROUNDS; i++) {
		printf("Round %d\n", i);
		sub_bytes(state);
		print_data(state, "Byte Sub");
		shift_rows(state);
		print_data(state, "Row Shift");
		mix_cols(state);
		print_data(state, "Col Mix");
		add_round_key(state, expansion + (4 * i));
		print_data(state, "Added Round Key");
	}
	sub_bytes(state);
	print_data(state, "Subbed Bytes");
	shift_rows(state);
	print_data(state, "Shift Row");
	add_round_key(state, expansion + (4*ROUNDS));

	free(expansion);

	return state;
}

void sub_bytes(unsigned char* data) {
	char i;
	for (i = 0; i < BLOCK_SIZE; i++) {
		data[i] = s_box_lookup(data[i]);
	}

}

void shift_rows(unsigned char *data) {
	char temp[] = {0,0,0,0};

	for (char r = 1; r < 4; r++) {
		for (char c = 0; c < 4; c++) {
			temp[c] = data[((c+r) % 4) * 4 +r];
		}
		for (char c = 0; c < 4; c++) {
			data[4*c + r] = temp[c];
		}
		
	}

}

unsigned char xtime(unsigned char c) {
if (c >> 7) {
		return (c << 1) ^ 0x1b;
	} else {
		return c << 1;
	}

}
unsigned char xtimes(unsigned char c, unsigned char n){
	if (n == 1) {
		return c;
	}
	if ( n > 1) {
		return xtime(xtimes(c, n >> 1));
	}
	return 0;
}

//Multiply in GF(2^8)
unsigned char gf_mul(unsigned char a, unsigned char b) {
	unsigned char out = 0;
	for (char c = 0; c < 8; c ++) {
		out ^= xtimes(a, (((b >> c) & 0x1) << c));
	}
	return out;
}

void mix_cols(unsigned char *data) {
	for (char i = 0; i < 4; i++) {
		unsigned char r1 = data[4 * i];
		unsigned char r2 = data[4 * i + 1];
		unsigned char r3 = data[4 * i + 2];
		unsigned char r4 = data[4 * i + 3];
		//046681e5
		data[4 * i] = gf_mul(r1, 2)^ gf_mul(r2, 3) ^ r3 ^ r4;
		data[4 * i + 1] = r1 ^ gf_mul(r2, 0x2) ^ gf_mul(r3, 0x3) ^ r4;
		data[4 * i + 2] = r1 ^ r2 ^ gf_mul(r3, 0x2) ^ gf_mul(r4, 0x3);
		data[4 * i + 3] = gf_mul(r1, 0x3) ^ r2 ^ r3 ^ gf_mul(r4, 0x2);
	}
}


uint32_t *key_expansion(unsigned char *key) {
	char i = 0;
	uint32_t *key_schedule = (uint32_t*) calloc(44,sizeof(uint32_t));
	for ( i = 0; i <= BLOCK_SIZE; i++) {
		key_schedule[i] = (key[4 * i + 3]) | (key[4 * i + 2] << 8) | (key[4* i + 1] << 16) | (key[4 * i + 0] << 24);
	}
	for (i = 4; i <= (4 * ROUNDS + 3); i++) {
		uint32_t temp = key_schedule[i-1];
		if ((i % 4) == 0) {
			temp = sub_word(rot_word(temp));
			temp ^=  rcon[(i/4) - 1] << 24;
		}
		key_schedule[i] = key_schedule[i- 4] ^ temp;
	}

	return key_schedule;

}

uint32_t rot_word(uint32_t word) {
	unsigned char first = (unsigned char) ((word & 0xff000000) >> 24 );
       	uint32_t out =	(word << 8) | (first);
	return out;

}

void add_round_key(unsigned char *data, uint32_t *keyschedule) {
	for (int i = 0; i < 4; i++) {
		uint32_t word = keyschedule[i];
		for (int j = 0; j < 4; j++) {
			data[4 * i + j] = data[4 * i + j] ^ (((word & ((0xff) << ((3-j) * 8))) >> ((3-j) * 8))); 
		}
	}
}

void print_data(unsigned char *data, char *message) {
	for (int i = 0; i < 16; i++) {
		printf("%02x", data[i]);
	}

	printf(" - %s\n", message);
}
