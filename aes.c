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

	add_round_key(state, expansion);
	print_data(state, "Round 1");
	for (i = 0; i < ROUNDS - 1; i++) {
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
	shift_rows(state);
	add_round_key(state, expansion + ROUNDS);

	free(expansion);

	return state;
}

void sub_bytes(unsigned char* data) {
	char i;
	for (i = 0; i < BLOCK_SIZE; i++) {
		data[i] = s_box_lookup(data[i]);
	}

}

/*
 *00 04 08 12
 *01 05 09 13
 *02 06 10 14
 *03 07 11 15
 *
 *00 04 08 12
 *05 09 13 01
 *10 14 02 06
 *15 03 07 11
 *
 *
 */

void shift_rows(unsigned char *data) {
	char *temp = malloc(16);
	char i;

	for (i = 4; i < 16; i++) {
		//rotate i % 4
		
	}
	for (char i = 0; i < 16; i++) {
		data[i] = temp[i];
	}

	free(temp);
}

void mix_cols(unsigned char *data) {
	char i;

	for (i = 0; i < 4; i++) {
		char r1 = data[4 * i];
		char r2 = data[4 * i + 1];
		char r3 = data[4 * i + 2];
		char r4 = data[4 * i + 3];
		
		data[4 * i] = (2 * r1) ^ (3 * r2) ^ r3 ^ r4;
		data[4 * i + 1] = r1 ^ (2 * r2) ^ (3 * r3) ^ r4;
		data[4 * i + 2] = r1 ^ r2 ^ (2 * r3) ^ (3 * r4);
		data[4 * i + 3] = (3 * r4) ^ r2 ^ r3 ^ (2 * r4);
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
