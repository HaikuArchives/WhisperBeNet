/*
	Decode.cpp
*/

#ifndef _Decode_h
#include "Decode.h"
#endif

Decode::Decode(){
	g72x_init_state(&state);

	dec_bits = 4;
	in_buffer = 0;
	in_bits = 0;

	last_data = 0;
}

int Decode::unpack_input(char *buffer, int size_in_buf, unsigned char* code, int bits)
{
	unsigned char in_byte;

	if (in_bits < bits) {
		if (in_buffer_pos >= size_in_buf) {
			*code = 0;
			return (-1);
		} else {
			in_byte = buffer[in_buffer_pos++]; 
		}		
		in_buffer |= (in_byte << in_bits);
		in_bits += 8;
	}
	*code = (unsigned char)in_buffer & ((1 << bits) - 1);
	in_buffer >>= bits;
	in_bits -= bits;
	return (in_bits > 0);
}

void Decode::reset_unpack(){
	in_buffer = 0;
	in_bits = 0;
	in_buffer_pos = 0;
}

void* Decode::createBuffer(int size_in_buf, size_t &size, int no_of_trame_to_skip){
	size = size_in_buf * 4 * (no_of_trame_to_skip*2);
	return (void*)(new short[size/2]);
}

void Decode::decodeBuffer(char *in_buffer, int size_in_buf, void *data, int no_of_trame_to_skip){
	reset_unpack();

	int data_pos = 0;	
	short *data_short = (short*)data;	
	while (unpack_input(in_buffer, size_in_buf,  &code, dec_bits) >= 0) {
		sample_short = g721_decoder(code, AUDIO_ENCODING_LINEAR, &state);
		//sample_short = g723_40_decoder(code, AUDIO_ENCODING_LINEAR, &state);
		short augmentation = (sample_short - last_data) / no_of_trame_to_skip;
		for(int j = 0; j < (no_of_trame_to_skip*2); j++){
			data_short[data_pos + j] = last_data + (augmentation * (j/2));		
		}
		data_pos += no_of_trame_to_skip*2;
		last_data = sample_short;
	}

	data = (void*)data_short;
}
