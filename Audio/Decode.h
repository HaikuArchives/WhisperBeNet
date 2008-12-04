/*
	This class is a front-end of the codec g721
	Is to permit to decode a buffer encoded with the g.721-linear
*/

#ifndef _Decode_h
#define _Decode_h

#ifndef _G72X_H
#include "g72x.h"
#endif

#include <stdio.h>

class Decode {
public:

	Decode();
	void decodeBuffer(char*, int, void*, int);
	void* createBuffer(int, size_t&, int);
	
private:
	struct g72x_state state;
	short sample_short;
	unsigned char code;
	int	dec_bits;	

	//unpack function
	int unpack_input(char *, int, unsigned char *, int);
	void reset_unpack();

	//unpack variable (must be preserve between each call of unpak)
	unsigned int in_buffer;
	int in_bits;
	int in_buffer_pos;

	short last_data;
};

#endif
