#ifndef AES_H
#define AES_H
char* encrypt(char* data, char* key);
void sub_bytes(char *data);
void shift_rows(char *data);
void mix_cols(char *data);
#endif
