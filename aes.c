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


char rcon[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

char* encrypt(char* data, char* key){
	char* state = calloc(BLOCK_SIZE, 1);
	char i;
	uint32_t *expansion = key_expansion(key);
	//KeyExpansion(key)
	for (i = 0; i < BLOCK_SIZE; i++) {
		state[i] = data[i];
	}
	// state = addroundkey(state, keyexpansion[0...3]
	for (i = 0; i < ROUNDS - 2; i++) {
		sub_bytes(state);
		shift_rows(state);
		mix_cols(state);
		// addrounkey(state, keyexpansion[4*i...(4*i) + 3]
	}
	sub_bytes(state);
	shift_rows(state);
	//AddRoundKey(state, Keyexpansion[4*ROUNDS, 4*Rounds + 1 ... 4* Rounds + 3]
	

	return state;
}

void sub_bytes(char* data) {
	char i;
	for (i = 0; i < BLOCK_SIZE; i++) {
		data[i] = s_box_lookup(data[i]);
	}

}

void shift_rows(char *data) {
	char temp[4] = {0,0,0,0};
	char i;
	for (i = 0; i < 4; i++) {
		temp[0] = data[(4 * i) + ( i % 4)];
		temp[1] = data[(4 * i) + ((1 - i) % 4)];
		temp[2] = data[(4 * i) + ((2 - i) % 4)];
		temp[3] = data[(4 * i) + ((3 - i) % 4)];
		char j;

		for (j = 0; j < 4; j++) {
			data[(i* 4) + j] = temp[j];
		}
	}
}

void mix_cols(char *data) {
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


uint32_t *key_expansion(char *key) {
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
	char first = (char) ((word & 0xff000000) >> 24 );
       	uint32_t out =	(word << 8) | (first);
	return out;

}

void add_roundkey() {

}
