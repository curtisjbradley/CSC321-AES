#include <stdint.h>

#ifndef AES_H
#define AES_H
unsigned char* encrypt(unsigned char* data, unsigned char* key);
void sub_bytes(unsigned char *data);
void shift_rows(unsigned char *data);
void mix_cols(unsigned char *data);
uint32_t rot_word(uint32_t word);
uint32_t *key_expansion(unsigned char* key);
void add_round_key(unsigned char *data, uint32_t *key_schedule);
void print_data(unsigned char *data, char *message);
unsigned char gf_mul(unsigned char a, unsigned char b);
unsigned char* generate_key();
#endif
