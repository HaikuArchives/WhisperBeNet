//#define DEBUG

#ifndef _PROTOCOL
#include "protocol.h"
#endif

#ifndef _Output_h
#include "Output.h"
#endif

//****************************************************************************************
//	Procedure:	CProtocol::CProtocol
//	Fonction:	Constructeur
//***************************************************************************************
CProtocol::CProtocol()
{
	Output::Instance()->Network("Protocol created\n");
	connection = new CConnection;
}


//****************************************************************************************
//	Procedure:	CProtocol::~CProtocol
//	Fonction:	Destructeur
//***************************************************************************************
CProtocol::~CProtocol()
{
	delete connection;
}

//****************************************************************************************
//	Procedure:	CProtocol::SendRTPPacket
//	Fonction:	Generer les packets RTP
//****************************************************************************************
//status_t CProtocol::SendRTPPacket(char* payload, int timestamp, int packetType) //OliverESP: 2nd rev
status_t CProtocol::SendRTPPacket(TRtpData& payload, int packetType) 
{
	TRtpHeader rtpHeader;
	char datagramme[RTPDATAGRAMMELEN];
	static int seqNb;
	
	rtpHeader.v = V;
	rtpHeader.p = P;
	rtpHeader.x = X;
	rtpHeader.cc = CC;
	rtpHeader.m = M;
	rtpHeader.pt = packetType;
	rtpHeader.seq = seqNb ;
	rtpHeader.ts = payload.timestamp; //OliverESP: modif
	rtpHeader.ssrc = SSRC;

	seqNb = (seqNb + 1)%65536 ;
	memset(datagramme, '\0', sizeof(datagramme));
	memcpy(datagramme, &rtpHeader, RTPHEADERLEN);

	switch(packetType)
	{
		case NET_ID_RAW: 
			//TODO: if != good cout bla bla bla
			//memcpy(datagramme + RTPHEADERLEN, payload.data, RTPPLLEN_RAW);
			memcpy(datagramme + RTPHEADERLEN, payload.data, payload.size);
			connection->SendRtpUdp(datagramme, NET_ID_RAW, payload.size);
		break;
	
		case NET_ID_CODEC: 
			//TODO: if != good cout bla bla bla
			//memcpy(datagramme + RTPHEADERLEN, payload.data, RTPPLLEN_CODEC);
			memcpy(datagramme + RTPHEADERLEN, payload.data, payload.size);
			connection->SendRtpUdp(datagramme, NET_ID_CODEC, payload.size);
		break;
		
		case NET_ID_MODEM:
			//TODO: if != good cout bla bla bla
			//memcpy(datagramme + RTPHEADERLEN, payload.data, RTPPLLEN_MODEM);
			memcpy(datagramme + RTPHEADERLEN, payload.data, payload.size);
			connection->SendRtpUdp(datagramme, NET_ID_MODEM, payload.size);
		break;

		default:
			cout << "SENDRTP: ERROR: Unknown message trying to be sent: CNetwork::SendModeRTP" << endl;
			return B_ERROR;
	};


	
	return B_OK;
}

//****************************************************************************************
//	Procedure:	CProtocol::RecvRTPPacket
//	Fonction:	Recevoir les packets RTP
//	Retourne:	Type de packet RTP recu et payload
//****************************************************************************************
int CProtocol::RecvRTPPacket(char* payload,ssize_t* sizerec)
{
	int32 size; // OliverESP: to check size
	
	TRtpHeader rtpHeader;
	char datagramme[RTPDATAGRAMMELEN];
	memset(datagramme, '\0', sizeof(datagramme));	
	
	if ((size = connection->RecvRtpUdp(datagramme, RTPDATAGRAMMELEN)) <= 0)
	{
		return NET_ID_EMPTY;
	}

#ifdef DEBUG
	cout << "Protocol: Received wH " << size - RTPHEADERLEN << " bytes" << endl;
#endif
	memcpy(&rtpHeader, datagramme, sizeof(rtpHeader));
	
	switch(rtpHeader.pt)
	{
		case(NET_ID_RAW):
			// memcpy(payload, datagramme + RTPHEADERLEN, RTPPLLEN_RAW); OliverESP
			memcpy(payload, datagramme + RTPHEADERLEN, size - RTPHEADERLEN);
			
			break;
		case(NET_ID_CODEC):
			// memcpy(payload, datagramme + RTPHEADERLEN, RTPPLLEN_CODEC);
			memcpy(payload, datagramme + RTPHEADERLEN, size - RTPHEADERLEN);
			break;
		case(NET_ID_MODEM):
			// memcpy(payload, datagramme + RTPHEADERLEN, RTPPLLEN_MODEM);
			memcpy(payload, datagramme + RTPHEADERLEN, size - RTPHEADERLEN);
			break;
	}
	*sizerec = size - RTPHEADERLEN;
	//TODO: verifier la version
	return rtpHeader.pt;
}
		

//****************************************************************************************
//	Procedure:	CProtocol::SendRTCPPacket
//	Fonction:	Generer les packets RTP
//****************************************************************************************
status_t CProtocol::SendRTCPPacket(int packetType)
{
	TRtcpHeader rtcpHeader;
	char datagramme[RTCPDATAGRAMMELEN];

	switch(packetType)
	{
		case NET_ID_DISCONNECT:
		case NET_ID_CONNECT:
		case NET_ID_ALIVE:
		case NET_ID_ACCEPT:
		case NET_ID_REFUSE:
		case NET_ID_BUSY:
		case NET_ID_ABORT:
			rtcpHeader.v = V;
			rtcpHeader.ht = packetType;
			break;
		default:
			cout << "SENDRTCP: ERROR: Unknown message trying to be sent: CNetwork::SendModeRTCP" << endl;
			return B_ERROR;
			break;
	};

	memset(datagramme, '\0', sizeof(datagramme));
	memcpy(datagramme, &rtcpHeader, RTCPHEADERLEN);
	
	//TODO: if != good cout bla bla bla
	connection->SendRtcpUdp(datagramme);

	return B_OK;
}


//****************************************************************************************
//	Procedure:	CProtocol::RecvRTPPacket
//	Fonction:	Recevoir les packets RTP
//	Retourne:	Type de packet RTCP recu
//****************************************************************************************
//int CProtocol::RecvRTCPPacket(ulong* RemoteAddress)
int CProtocol::RecvRTCPPacket(sockaddr_in& RemoteAddress)

{
	TRtcpHeader rtcpHeader;
	char datagramme[RTCPDATAGRAMMELEN];

	memset(datagramme, '\0', sizeof(datagramme));
	
	if (connection->RecvRtcpUdp(datagramme, RTCPDATAGRAMMELEN, RemoteAddress) <= 0)
	{
		return NET_ID_EMPTY;
	}

	memcpy(&rtcpHeader, datagramme, sizeof(rtcpHeader));
	
	//TODO: verifier la version
	return rtcpHeader.ht;

}




void CProtocol::Display8(unsigned value)
{
	unsigned mask = 1 << 7;
	unsigned c;
	
	for(c = 1; c <= 8; c++)
	{
		cout << (value & mask ? '1' : '0');
		value <<= 1;
	}

	cout << endl;
}

//**************************************** temp!
void CProtocol::Display16(unsigned value)
{
	unsigned mask = 1 << 15;
	unsigned c;
	
	for(c = 1; c <= 16; c++)
	{
		cout << (value & mask ? '1' : '0');
		value <<= 1;

		if(c % 8 == 0)
		{	
			cout << ' ';
		}

	}
	
	cout << endl;
}

//**************************************** temp!
void CProtocol::Display32(unsigned value)
{
	unsigned mask = 1 << 31;
	unsigned c;
	
	for(c = 1; c <= 32; c++)
	{
		cout << (value & mask ? '1' : '0');
		value <<= 1;

		if(c % 8 == 0)
		{	
			cout << ' ';
		}

	}

	
	cout << endl;
}






//	cout << " 	PROTOCOL SEND RTP: Sending..." << endl;
//	cout << "	RtpHeader.pt   = " << rtpHeader.pt << endl;
//	cout << "	RtpHeader.seq  = " << rtpHeader.seq << endl;
//	cout << "	RtpHeader.ts   = " << rtpHeader.ts << endl;
//	cout << "	Payload : " <<	payload << endl;


//	cout << " 	PROTOCOL SEND RTP: Receiving..." << endl;
//	cout << "	RtpHeader.pt   = " << rtpHeader.pt << endl;
//	cout << "	RtpHeader.seq  = " << rtpHeader.seq << endl;
//	cout << "	RtpHeader.ts   = " << rtpHeader.ts << endl;
//	cout << "	Payload : " <<	payload << endl;
