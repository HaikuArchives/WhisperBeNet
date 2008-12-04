/*
	This class is a front-end of the codec g721
	Is to permit to encode a buffer with the g.721-linear
*/

#ifndef _Encode_h
#define _Encode_h

#ifndef _G72X_H
#include "g72x.h"
#endif

#include <stdio.h>

class Encode {
public:

	Encode();
	void encodeBuffer(void *, size_t, char*, int);
	bool decisionToEncode(void *, size_t, int, float);
	char* createBuffer(size_t, int&, int);

private:
	struct g72x_state state;
	short sample_short;
	unsigned char code;
	int	resid;
	int	in_coding;
	int	in_size;
	int	enc_bits;
	
	//pack_function
	void reset_pack();
	int pack_output(unsigned, int, char*);

	//pack variables (must be preserve between each call of pack)
	unsigned int out_buffer;
	int out_bits;
	int out_buffer_pos;
};

#endif
