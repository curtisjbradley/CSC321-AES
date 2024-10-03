#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "box.h"
#include "aes.h"

#define BLOCK_SIZE 16
#define ROUNDS 10

// Source: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf
// AES-128 does 10 rounds
char* encrypt(char* data, char* key){
	char* state = calloc(BLOCK_SIZE, 1);
	char i;
	//KeyExpansion(key)
	for (i = 0; i < BLOCK_SIZE; i++) {
		state[i] = data[i];
	}
	// state = addroundkey(state, keyexpansion[0...3]
	for (i = 0; i < ROUNDS - 2; i++) {
		sub_bytes(state);
		shift_rows(state);
		//MixColumns(state)
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
		temp[0] = data[i];
		temp[1] = data[(i + 1) % 4];
		temp[2] = data[(i + 2) % 4];
		temp[3] = data[(i + 3) % 4];
		char j;

		for (j = 0; j < 3; j++) {
			data[(i* 4) + j] = temp[j];
		}
	}
}
