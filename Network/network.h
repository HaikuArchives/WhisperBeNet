#ifndef _Network
#define _Network

#ifndef _Player_h
#include "Player.h"
#endif

#ifndef __CONNECTION_H_
#include "connection.h"
#endif

#ifndef _PROTOCOL
#include "protocol.h"
#endif

#ifndef _Definitions
#include "definitions.h"
#endif

#ifndef _TYPES
#include "types.h"
#endif

#ifndef _ConnectView_h    // OliverESP
#include "ConnectView.h"
#endif

#include <OS.h>
#include <Window.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h> 

#include <iostream>
#include <Alert.h>



class CNetwork
{
public:
	~CNetwork();
	static CNetwork* Instance();
		
	//UI Interface
	status_t Initialize();
	status_t Quit();
//	status_t ConnectFromUI(ulong lRemoteAddress);
	status_t ConnectFromUI(char* host);

	status_t Disconnect();
	status_t InitializeDiscussion();

	//Codec Interface
	int GetTimeStamp();

	void	SetWindow(BWindow* window)		{m_pBeNetWindow = window;}
	void	SetPlayer(Player* player)		{m_pPlayer = player;}
	bool	IsConnectionReady()				{return m_connectionReady;}
	const char*	GetRemoteIP()					{return protocol->GetRemoteIP();}
	bool	AcceptConnection();

	void SetView(ConnectView* view); //OliverESP inform the network who to
									 //send the end of the connection(to toggle);

private: // functions
	CNetwork();

	void	UpdateConnection(bool result);

	static status_t SendModeRTPGopher(void *arg); 
	status_t SendModeRTP(void);

	static status_t RecvModeRTPGopher(void *arg); 
	status_t RecvModeRTP(void);

	static status_t SendModeRTCPGopher(void *arg); 
	status_t SendModeRTCP(void);

	static status_t RecvModeRTCPGopher(void *arg); 
	status_t RecvModeRTCP(void);
	
	static status_t PitGopher(void *arg); 
	status_t Pit(void);

	static status_t WaitConnectionGopher(void* arg); 
	status_t WaitConnection(void);

	void Terminate();

private: // data
	static CNetwork* m_instance;

	CProtocol*		protocol;
	BAlert*			m_alertAccept;
	timeval			startTime;
	Player*			m_pPlayer;
	BWindow*		m_pBeNetWindow;
	bool			m_connectionReady;
	bool			m_connectedRTP;
	bool			m_connectedRTCP;
	
	ConnectView*	m_connectview;  //OliverESP keep information about the view
};

#endif
