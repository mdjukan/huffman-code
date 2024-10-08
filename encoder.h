#ifndef __ENCODER_INCLUDED__
#define __ENCODER_INCLUDED__

#include <stdio.h>
#include "huffman_algo.h"
#include "consts.h"

void
clear_buffer(char buffer[BUFFER_SIZE]) {
	for (int i=0; i<BUFFER_SIZE; i++) {
		buffer[i] = 0;
	}
}

void
write_buffer(FILE *out_file, char buffer[BUFFER_SIZE], int numchar) {
	char bytes[BIT_BUFFER_SIZE];
	int k = 0;
	for (int i=0; i<BIT_BUFFER_SIZE; i++) {
		bytes[i] = 0;
		for (int j=CHAR_BIT-1; j>=0; j--) {
			bytes[i] |= buffer[k++]<<j;
		}
	}

	fwrite(&numchar, sizeof(int), 1, out_file);
	fwrite(bytes, sizeof(char), BIT_BUFFER_SIZE, out_file);
}

void
encode(char *in_file_name, char *out_file_name) {
	FILE *in_file = fopen(in_file_name, "rb");
	FILE *out_file = fopen(out_file_name, "wb");

	int freqs[ASCII_LEN];
	count_freqs(in_file_name, freqs);
	fwrite(freqs, sizeof(int), ASCII_LEN, out_file);

	Code *codes[ASCII_LEN];
	generate_codes(freqs, codes);

	int num_buffer = 0;

	char buffer[BUFFER_SIZE];
	clear_buffer(buffer);

	int numchar = 0;
	int k = 0;
	char ch;


	while (1) {
		size_t num_read = fread(&ch, sizeof(char), 1, in_file);
		if (num_read==0) {
			write_buffer(out_file, buffer, numchar);
			break;
		}

		Code *code = codes[(int)ch];

		if (BUFFER_SIZE-k<code->len) {
			num_buffer += 1;
			write_buffer(out_file, buffer, numchar);
			clear_buffer(buffer);
			k = 0;
			numchar = 0;
		}

		for (int i=0; i<code->len; i++) {
			buffer[k++] = code->code[i];
		}
		numchar += 1;
	}

	int zero = 0;
	fwrite(&zero, sizeof(int), 1, out_file);

	fclose(in_file);
	fclose(out_file);
}
#endif // __ENCODER_INCLUDED__
