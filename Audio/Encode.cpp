/*
	Encode.cpp
*/

#ifndef _Encode_h
#include "Encode.h"
#endif

#include <stdio.h>

Encode::Encode(){
	g72x_init_state(&state);

	enc_bits = 4;
	in_size = sizeof (short);
	out_buffer = 0;
	out_bits = 0;
	code = 0;
}

//Pack output codes into bytes and write them to stdout.
//Returns 1 if there is residual output, else returns 0.

int Encode::pack_output(unsigned code, int bits, char* buffer)
{
	unsigned char out_byte;

	out_buffer |= (code << out_bits);
	out_bits += bits;
	if (out_bits >= 8) {
		out_byte = out_buffer & 0xff;
		out_bits -= 8;
		out_buffer >>= 8;

		buffer[out_buffer_pos++] = out_byte;
	}
	return (out_bits > 0);
}

void Encode::reset_pack(){
	out_buffer = 0;
	out_bits = 0;
	out_buffer_pos = 0;
}


char* Encode::createBuffer(size_t size, int &size_of_buf, int no_of_trame_to_skip)
{
	size_of_buf = size / 4 / (no_of_trame_to_skip*2);	
	return new char[size_of_buf];
}

bool Encode::decisionToEncode(void *data, size_t size, int no_of_trame_to_skip, float volume)
{
	short *data_short = (short*)data;
	long data_decision = 0;

	for (unsigned int i = 0; i < size / 2; i += no_of_trame_to_skip*2)
	{
		//to control the volume of mic
		data_short[i] = (short)((float)data_short[i] * volume);

		//to avoid some "affreux" peak
		if (data_short[i] > 30500 || data_short[i] < -30500)
			data_short[i] = (short)((float)data_short[i] / 1.2);

		data_decision += abs(data_short[i]);
	}

	data = (void*)data_short;

//	printf("%d\n",(data_decision / (size/(no_of_trame_to_skip * 2))));
	return ((data_decision / (size/(no_of_trame_to_skip * 2))) >= (20) && (size == 32768));
//	return ((data_decision / (size/(no_of_trame_to_skip * 2))) >= (120) /*&& (size == 32768) */);
}


void Encode::encodeBuffer(void *data, size_t size, char *buffer, int no_of_trame_to_skip)
{
	reset_pack();	
	
	short *data_short = (short*)data;

	/* Read input buffer and process */
	for(unsigned int i = 0; i < size / 2; i += no_of_trame_to_skip*2){
		sample_short = data_short [i];
		code = g721_encoder(sample_short, AUDIO_ENCODING_LINEAR, &state);
		//code = g723_40_encoder(sample_short, AUDIO_ENCODING_LINEAR, &state);
		
		resid = pack_output(code, enc_bits, buffer);
	}

	/* Fill with zero codes until all residual codes are written out */
	while (resid) {
		resid = pack_output(0, enc_bits, buffer);
	}
}

