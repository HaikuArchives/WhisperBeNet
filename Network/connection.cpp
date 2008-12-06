/* connection.cc */
#ifndef _Output_h
#include "Output.h"
#endif

#ifndef _CONNECTION_H_
#include "connection.h"
#endif

#include <sys/socket.h>

#include <unistd.h>
#include <netdb.h>

//****************************************************************************************
//	Procedure:	CConnection::CConnection
//	Fonction:	Constructeur
//****************************************************************************************
CConnection::CConnection()
{
	m_bConnectionReady = false;	

	socketSendRtpUdp = -1;	// pseudo client
	socketRecvRtpUdp = -1;  // pseudo server
	socketSendRtcpUdp = -1; // pseudo client
	socketRecvRtcpUdp = -1; // pseudo server

	//Address: Local RTP (port 2000)
	addressLocalRtpUdp.sin_family = AF_INET;
	addressLocalRtpUdp.sin_port = htons(PORT_RTP_UDP);
//	memset(addressLocalRtpUdp.sin_zero, 0, sizeof(addressLocalRtpUdp.sin_zero));    // warum? -> out!

//	addressLocalRtpUdp.sin_addr.s_addr = GetLocalAddr();   // Changes   ???no more needed???
    addressLocalRtpUdp.sin_addr.s_addr = htonl(INADDR_ANY);


	//Address: Local RTCP (port 2001)
	addressLocalRtcpUdp.sin_family = AF_INET;
	addressLocalRtcpUdp.sin_port = htons(PORT_RTCP_UDP);
//	memset(addressLocalRtcpUdp.sin_zero, 0, sizeof(addressLocalRtcpUdp.sin_zero));  // warum? -> out!

//	addressLocalRtcpUdp.sin_addr.s_addr = GetLocalAddr();	// Changes   ???no more needed???
    addressLocalRtcpUdp.sin_addr.s_addr = htonl(INADDR_ANY);



}

//****************************************************************************************
//	Procedure:	CConnection::~CConnection
//	Fonction:	Destructeur
//****************************************************************************************	
CConnection::~CConnection()
{
	close(socketSendRtpUdp);
	close(socketRecvRtpUdp);
	close(socketSendRtcpUdp);
	close(socketRecvRtcpUdp);
}

//****************************************************************************************
status_t CConnection::SetRemoteAddress(char* host)
{
	struct hostent* hostRemote;

// OliverESP: MoveOver the UDP fixing changes there are anothers even in headers:

// There is a problem with gethostbyaddr. Not always can retreive the information of a host
// and then returns a NULL. so we cannot depend on gethostbyaddr and not also of " ulong RemoteAddress "
// The most reliable information we have about the remote host that is connecting (with us)is the sockaddr_in returned
// by recvfrom. So we will use this for setting the remote adress that someone is connecting to us. -> SetRemoteConnectingAddress

// And we will use just this method just for setting the remote host when WE are starting the connection.
// Also we will use a string and here we will manage the possible errors.


// hostRemote = gethostbyaddr((const char *)&RemoteAddress, sizeof (RemoteAddress), AF_INET);
	
// OliverESP: explained before we don´t use anymore gethostbyaddr not safe, so gethostbyname
	hostRemote = gethostbyname(host);
	if (hostRemote == NULL) {
		cout << "gethostbyname: Couldn´t get host information: " << h_errno << endl;
		return B_ERROR;
	}
	
	//!!!Sweet bug!!! Declarer les remotes AVANT les locals //

	//Address: Remote RTP (port 2000)
	addressRemoteRtpUdp.sin_family = AF_INET;
	addressRemoteRtpUdp.sin_port = htons(PORT_RTP_UDP);
//	memset(addressRemoteRtpUdp.sin_zero, 0, sizeof(addressRemoteRtpUdp.sin_zero));

//	addressRemoteRtpUdp.sin_addr.s_addr = *(long *)hostRemote->h_addr;
    memcpy((char *) &addressRemoteRtpUdp.sin_addr.s_addr, hostRemote->h_addr_list[0], hostRemote->h_length);

	
	//Address: Remote RTCP (port 2001)
	addressRemoteRtcpUdp.sin_family = AF_INET;
	addressRemoteRtcpUdp.sin_port = htons(PORT_RTCP_UDP);	
//	memset(addressRemoteRtcpUdp.sin_zero, 0, sizeof(addressRemoteRtcpUdp.sin_zero)); //out

//	addressRemoteRtcpUdp.sin_addr.s_addr = *(long *)hostRemote->h_addr;	
    memcpy((char *) &addressRemoteRtcpUdp.sin_addr.s_addr, hostRemote->h_addr_list[0], hostRemote->h_length);

	m_bConnectionReady = true;

	return B_OK;
}

void CConnection::SetRemoteConnectingAddress(sockaddr_in& RemoteAddress)    // OliverESP: new method provided
{
	cout << "SetRemoteConnectingAddress: Updating the remote host (v6)" << endl;

    memcpy( &addressRemoteRtcpUdp , &RemoteAddress , sizeof(RemoteAddress));
	addressRemoteRtcpUdp.sin_family = AF_INET;
	addressRemoteRtcpUdp.sin_port = htons(PORT_RTCP_UDP);	

    memcpy( &addressRemoteRtpUdp , &RemoteAddress , sizeof(RemoteAddress));
	addressRemoteRtpUdp.sin_family = AF_INET;
	addressRemoteRtpUdp.sin_port = htons(PORT_RTP_UDP);

}

//****************************************************************************************
//	Procedure:	CConnection::RtpUdpMode
//	Fonction:	
//****************************************************************************************	
void CConnection::RtpUdpMode(MODE mode)
{
int n = 1;  // OliverESP fix
int error;  // OliverESP debugging

	switch (mode)
	{
	case SEND:
		if (socketSendRtpUdp < 0)
		{
			if((socketSendRtpUdp = socket(AF_INET, SOCK_DGRAM, 0)) < B_OK)
				cout << "ERROR: Unable to create RTP UDP socket - Cconnection::RtpUdpMode (SEND)" << endl;

//			if ( setsockopt(socketSendRtpUdp, SOL_SOCKET, SO_NONBLOCK, "1", 1) != B_OK )
			if ( (error = setsockopt(socketSendRtpUdp, SOL_SOCKET, SO_NONBLOCK, &n, sizeof(n))) != B_OK )
				cout  << "ERROR: setsockopt - Cconnection::RtpUdpMode (SEND)" << endl;
		}
		break;

	case RECV:
		if (socketRecvRtpUdp < 0)
		{
			if((socketRecvRtpUdp = socket(AF_INET, SOCK_DGRAM, 0)) < B_OK)
				cout << "ERROR: Unable to create RTP UDP socket - Cconnection::RtpUdpMode(RECV)" << endl;

//			if ( setsockopt(socketRecvRtpUdp, SOL_SOCKET, SO_NONBLOCK, "1", 1) != B_OK )	
			if ( (error = setsockopt(socketRecvRtpUdp, SOL_SOCKET, SO_NONBLOCK, &n, sizeof(n))) != B_OK ){
				cout  << "ERROR: setsockopt - Cconnection::RtpUdpMode (RECV)" << endl;
				perror("setsockopt");
				}


//			if ((bind(socketRecvRtpUdp, (sockaddr *)&addressLocalRtpUdp, sizeof(sockaddr_in))) < 0)
			if ((bind(socketRecvRtpUdp, (struct sockaddr *)&addressLocalRtpUdp, sizeof(addressLocalRtpUdp))) < 0)
				cout << "ERROR: Unable to bind RTP Udp socket recv - CConnection::RtpUdpMode (RECV)" << endl;	
		}
		break;
	};
}


//****************************************************************************************
//	Procedure:	CConnection::RtcpUdpMode
//	Fonction:	
//****************************************************************************************	
void CConnection::RtcpUdpMode(MODE mode)
{
int n = 1;  // OliverESP fix
int error;  // OliverESP debugging

	switch (mode)
	{
	case SEND:
		if (socketSendRtcpUdp < 0)
		{
			if((socketSendRtcpUdp = socket(AF_INET, SOCK_DGRAM, 0)) < B_OK)
				cout << "ERROR: Unable to create RTCP UDP socket - CConnection::RtcpUdpMode (SEND)" << endl;
		
//			if ( setsockopt(socketSendRtcpUdp, SOL_SOCKET, SO_NONBLOCK, "1", 1) != B_OK )
			if ( (error = setsockopt(socketSendRtcpUdp, SOL_SOCKET, SO_NONBLOCK, &n, sizeof(n))) != B_OK )
				cout  << "ERROR: setsockopt - CConnection::RtcpUdpMode (SEND)" << endl;
		}
		break;
		
	case RECV:
		if (socketRecvRtcpUdp < 0)
		{
			if((socketRecvRtcpUdp = socket(AF_INET, SOCK_DGRAM, 0)) < B_OK)
				cout << "ERROR: Unable to create RTCP UDP socket - CConnection::RtcpUdpMode (RECV)" << endl;
		
//			if ( setsockopt(socketRecvRtcpUdp, SOL_SOCKET, SO_NONBLOCK, "1", 1) != B_OK ) // OliverESP fix
			if ( (error = setsockopt(socketRecvRtcpUdp, SOL_SOCKET, SO_NONBLOCK, &n, sizeof(n))) != B_OK ){
				cout  << "ERROR: setsockopt - CConnection::RtcpUdpMode (RECV) " << error << endl;
				perror("setsockopt");
				}
	
//			if ((bind(socketRecvRtcpUdp, (sockaddr *)&addressLocalRtcpUdp, sizeof(sockaddr_in))) < 0) // OliverESP: following a sure way
			if ((bind(socketRecvRtcpUdp, (struct sockaddr *)&addressLocalRtcpUdp, sizeof(addressLocalRtcpUdp))) < 0)
				cout << "ERROR: Unable to bind RTCP Udp socket recv - CConnection::RtcpUdpMode (RECV)" << endl;	
		}
		break;
	};
}


//****************************************************************************************
//	Procedure:	CConnection::SendRtpUdp
//	Fonction:	Envoyer une RTP 
//****************************************************************************************	
void CConnection::SendRtpUdp(char *data, int packetType,int size)
{
	switch(packetType)
	{
		case(NET_ID_RAW):

//			if (sendto(socketSendRtpUdp, data, RTPHEADERLEN + RTPPLLEN_RAW, 0, (sockaddr *)&addressRemoteRtpUdp, sizeof(sockaddr_in)) < 0)
//	    	if (sendto(socketSendRtpUdp, data, RTPHEADERLEN + RTPPLLEN_RAW, 0, (struct sockaddr *)&addressRemoteRtpUdp, sizeof(addressRemoteRtpUdp)) < 0)
	    	if (sendto(socketSendRtpUdp, data, RTPHEADERLEN + size, 0, (struct sockaddr *)&addressRemoteRtpUdp, sizeof(addressRemoteRtpUdp)) < 0)
			{
				cout << "ERROR: Unable to send by Audio UDP some data - Cconnection::SendUdp()" << endl;
			}
			break;	
		case(NET_ID_CODEC):
//			if (sendto(socketSendRtpUdp, data, RTPHEADERLEN + RTPPLLEN_CODEC, 0, (sockaddr *)&addressRemoteRtpUdp, sizeof(sockaddr_in)) < 0)
//			if (sendto(socketSendRtpUdp, data, RTPHEADERLEN + RTPPLLEN_CODEC, 0, (struct sockaddr *)&addressRemoteRtpUdp, sizeof(addressRemoteRtpUdp)) < 0)
			if (sendto(socketSendRtpUdp, data, RTPHEADERLEN + size, 0, (struct sockaddr *)&addressRemoteRtpUdp, sizeof(addressRemoteRtpUdp)) < 0)			
			{
				cout << "ERROR: Unable to send by Codec UDP some data - Cconnection::SendUdp()" << endl;
			}
			break;
		case(NET_ID_MODEM):
//			if (sendto(socketSendRtpUdp, data, RTPHEADERLEN + RTPPLLEN_MODEM, 0, (sockaddr *)&addressRemoteRtpUdp, sizeof(sockaddr_in)) < 0)
//	    	if (sendto(socketSendRtpUdp, data, RTPHEADERLEN + RTPPLLEN_MODEM, 0, (struct sockaddr *)&addressRemoteRtpUdp, sizeof(addressRemoteRtpUdp)) < 0)
	    	if (sendto(socketSendRtpUdp, data, RTPHEADERLEN + size, 0, (struct sockaddr *)&addressRemoteRtpUdp, sizeof(addressRemoteRtpUdp)) < 0)
			{
				cout << "ERROR: Unable to send by Modem UDP some data - Cconnection::SendUdp()" << endl;
			}
			break;
	}
}


//****************************************************************************************
//	Procedure:	CConnection::RecvRtpUdp
//	Fonction:	Recevoir une trame RTP
//****************************************************************************************	
int CConnection::RecvRtpUdp(char* pchBuffer, int nMaxLen)
{
//	return recvfrom(socketRecvRtpUdp, pchBuffer, nMaxLen, 0, NULL, NULL);
//	return recvfrom(socketRecvRtpUdp, pchBuffer, nMaxLen, 0, (struct sockaddr *)&lRemoteAddress, &lenAddr);  // Oli: Im not sure
	return recvfrom(socketRecvRtpUdp, pchBuffer, nMaxLen, 0, NULL, NULL);

}


//****************************************************************************************
//	Procedure:	CConnection::SendRtcpUdp
//	Fonction:	Envoyer une trame RTCP 
//****************************************************************************************	
void CConnection::SendRtcpUdp(char *data)
{
	if (sendto(socketSendRtcpUdp, data, RTCPDATAGRAMMELEN, 0, (struct sockaddr *)&addressRemoteRtcpUdp, sizeof(addressRemoteRtcpUdp)) < 0)
//	if (sendto(socketSendRtcpUdp, data, RTCPDATAGRAMMELEN, 0, (sockaddr *)&addressRemoteRtcpUdp, sizeof(sockaddr_in)) < 0)  // OliverESP: following a sure way
	{
		cout << "ERROR: Unable to send by UDP some data - Cconnection::SendUdp()" << endl;
	}
}


//****************************************************************************************
//	Procedure:	CConnection::RecvRtcpUdp
//	Fonction:	Recevoir une trame RTCP
//****************************************************************************************	

//int CConnection::RecvRtcpUdp(char* pchBuffer, int nMaxLen, ulong* lRemoteAddress)   //OliverESP
int CConnection::RecvRtcpUdp(char* pchBuffer, int nMaxLen, sockaddr_in& lRemoteAddress)   
{
//	sockaddr_in sinRemote;   // OliverESP: yes can be deleted....

#ifdef ZETA
	int lenAddr;
#else	
	socklen_t lenAddr;
#endif


	lenAddr = sizeof(lRemoteAddress);  // OliverESP
//	
	int rv = recvfrom(socketRecvRtcpUdp, pchBuffer, nMaxLen, 0, (struct sockaddr *)&lRemoteAddress, &lenAddr);
//	int rv = recvfrom(socketRecvRtcpUdp, pchBuffer, nMaxLen, 0, (sockaddr*)&sinRemote, &lenAddr);  // OliverESP: following a sure way

//	*lRemoteAddress = (ulong)sinRemote.sin_addr.s_addr;  // it updates? don´t need to be done?


	// Oliver ESP: I did it for ensure.. improving and making a self method -> SetRemoteConnectingAddress()
//	if (rv > 0) {
//	        cout << "Updating the remote host v4" << endl;
//			memcpy( &addressRemoteRtcpUdp , &sinRemote , sizeof(sinRemote));
//			addressRemoteRtcpUdp.sin_family = AF_INET;
//			addressRemoteRtcpUdp.sin_port = htons(PORT_RTCP_UDP);	
//	}
					
	return rv;	
}


//****************************************************************************************
//	Procedure:	CConnection::GetLocalAddr
//	Fonction:	
//****************************************************************************************	
long CConnection::GetLocalAddr()
{ 
	struct hostent* hostLocal;
	// OliverESP: Need to decide/fix something, just works if localhostname is localhost

	if (gethostname(localName, MAXHOSTNAMELEN) == B_ERROR)   // OliverESP fix	
//	if (gethostname(localName, MAXHOSTNAMELEN) == 0)
	{ 
		cout << "ERROR: Unable to get host name - Cconnection::GetLocalAddr() I got : " << localName << endl;
		cout << "OliverESP: try set your hostname to localhost in Network Preferences"  << endl;
		return -1; 
	} 
   
	if ((hostLocal = gethostbyname(localName)) == NULL)
	{ 
		cout << "ERROR: Unable to get host by name - Cconnection::GetLocalAddr()  I got : " << localName << endl;
		cout << "OliverESP: try set your hostname to localhost in Network Preferences"  << endl;
		return -1; 
	} 
   
	return *(long *)hostLocal->h_addr; 
}
