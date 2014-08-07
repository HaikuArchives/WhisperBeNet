/* types.h */
#ifndef _TYPES
#define _TYPES

#include <time.h>
#include <MediaFiles.h>
#include "definitions.h"



#define V 2
#define P 0
#define X 0
#define CC 0
#define M 0
#define SSRC 0

struct TRtpData
{
	size_t size;
	int timestamp;
	char data[RTPPLLEN_MAX]; //OliverESP: change the order for spliting
};

struct TRtpHeader
{
	unsigned int v:2;
	unsigned int p:1;
	unsigned int x:1;
	unsigned int cc:4;
	unsigned int m:1;
	unsigned int pt:7;
	unsigned short seq;
	unsigned int ts;
	unsigned int ssrc;
	unsigned int csrc[CC];
};

struct TRtcpHeader
{
	unsigned int v:2;
	unsigned int ht:6;
};


#endif
