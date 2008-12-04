#ifndef _Definitions
#define _Definitions

#define PORT_RTP_UDP 2000
#define PORT_RTCP_UDP 2001


#define NET_ID_UNKNOWN		-1
#define NET_ID_EMPTY		000
#define NET_ID_RAW			001
#define NET_ID_CODEC		002
#define NET_ID_MODEM		003

#define NET_ID_ALIVE		010
#define NET_ID_CONNECT		011
#define NET_ID_DISCONNECT	012
#define NET_ID_ACCEPT		013
#define NET_ID_REFUSE		014
#define NET_ID_TIMEOUT		015
#define NET_ID_BUSY			016
#define NET_ID_ABORT		017

#define NET_KEEPALIVE_TIME	2500000	// micro-secondes
#define NET_RTCP_POLLING	NET_KEEPALIVE_TIME / 4
#define NET_TIMEOUT_TIME	20		// secondes

#define RTPHEADERLEN 		sizeof(TRtpHeader)
//#define RTPPLLEN_RAW 		2048		// FIXME!
#define RTPPLLEN_RAW 		32768		// I do and....

//#define RTPPLLEN_CODEC 		64
//#define RTPPLLEN_CODEC 		1024
//#define RTPPLLEN_MODEM 		42
//#define RTPPLLEN_MODEM 		682

#define RTPPLLEN_MAX		RTPPLLEN_RAW 
#define RTPDATAGRAMMELEN 	RTPHEADERLEN + RTPPLLEN_MAX

#define RTCPHEADERLEN		sizeof(TRtcpHeader)
#define RTCPDATAGRAMMELEN	RTCPHEADERLEN

#define NET_THREAD_SEND_RTP			"SendRTP"
#define NET_THREAD_RECV_RTP			"RecvRTP"
#define NET_THREAD_SEND_RTCP		"SendRTCP"
#define NET_THREAD_RECV_RTCP		"RecvRTCP"
#define NET_THREAD_PIT				"Pit"
#define NET_THREAD_WAIT_CONNECTION	"WaitConnection"

enum MODE {SEND, RECV}; 


#endif
