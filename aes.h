#include <stdint.h>

#ifndef AES_H
#define AES_H
char* encrypt(char* data, char* key);
void sub_bytes(char *data);
void shift_rows(char *data);
void mix_cols(char *data);
uint32_t rot_word(uint32_t word);
uint32_t *key_expansion(char* key);
void add_round_key(char *data, uint32_t *key_schedule);
#endif
