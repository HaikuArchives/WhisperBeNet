#ifndef _PROTOCOL
#define _PROTOCOL

#ifndef __CONNECTION_H_
#include "connection.h"
#endif

#include <iostream>
#include <string.h>
//#include <types.h>


class CProtocol
{
friend class CNetwork; //Network a besoin de connection pour rejoindre CConnection::xxxxMode

public:
	CProtocol();
	~CProtocol();

//	status_t	SendRTPPacket(char*, int, int); OliverESP: 2nd rev
	status_t	SendRTPPacket(TRtpData&, int); 
//	int			RecvRTPPacket(char*);
	int			RecvRTPPacket(char*,ssize_t*);
		
	status_t	SendRTCPPacket(int);
//	int			RecvRTCPPacket(ulong* RemoteAddress);     OliverESP:  explained in connection.cc
	int         RecvRTCPPacket(sockaddr_in& RemoteAddress); 


	const char*		GetRemoteIP()						{return connection->GetRemoteIP();}

private: // functions
	void		Display8(unsigned value);
	void		Display16(unsigned value);
	void		Display32(unsigned value);

private: // data
	CConnection* connection;
};

#endif

