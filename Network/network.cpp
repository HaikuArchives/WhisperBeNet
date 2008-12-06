//#define DEBUG_ALIVE

#ifndef _Network
#include "network.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#ifndef _Output_h
#include "Output.h"
#endif

#include <locale/Locale.h>
#include <support/Beep.h>


#ifndef timersub 
#define timersub(a, b, result) \
	do { \
		(result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
		(result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
		if ((result)->tv_usec < 0) { \
			--(result)->tv_sec; \
			(result)->tv_usec += 1000000; \
		} \
	} while (0)
#endif

CNetwork* CNetwork::m_instance = 0;

//****************************************************************************************
//	Procedure:	CNetwork::CNetwork
//	Fonction:	Constructeur
//****************************************************************************************
CNetwork::CNetwork() :
	m_alertAccept(NULL),
	m_pPlayer(NULL),
	m_pBeNetWindow(NULL),
	m_connectionReady(false),
	m_connectedRTP(false),
	m_connectedRTCP(false)
{
	Output::Instance()->Network("Network created\n");
	protocol= new CProtocol;

	// Initialize();
	
	// OliverESP debugging
	if (Initialize() == B_OK )
		Output::Instance()->Network("Net Thread Spawned...\n");	
}


//****************************************************************************************
//	Procedure:	CNetwork::~CNetwork
//	Fonction:	Destructeur
//****************************************************************************************
CNetwork::~CNetwork()
{
	if(kill_thread(find_thread(NET_THREAD_RECV_RTCP)) != B_NO_ERROR)
		cout << "ERROR: Unable to kill NET_THREAD_RECV_RTCP: CNetwork::~CNetwork" << endl;

	delete protocol;
}

//****************************************************************************************
CNetwork* CNetwork::Instance()
{
	if (!m_instance)
		m_instance = new CNetwork;
	return m_instance;
}
		
//****************************************************************************************
status_t CNetwork::Initialize()
{
 	thread_id idRecvRTCP;
	if ((idRecvRTCP = spawn_thread(RecvModeRTCPGopher, NET_THREAD_RECV_RTCP, B_NORMAL_PRIORITY, this)) < B_OK) 
	{	
		cout << "Error spawning NET_THREAD_RECV_RTCP" << endl;
		return idRecvRTCP;
	}
	resume_thread(idRecvRTCP);
	return B_OK;
}


//****************************************************************************************
status_t CNetwork::Quit()
{
	if(kill_thread(find_thread(NET_THREAD_RECV_RTCP)) != B_NO_ERROR)
	{
		cout << "ERROR: Unable to kill NET_THREAD_RECV_RTCP" << endl;
		return B_ERROR;
	}

	return B_OK;
}


//****************************************************************************************
status_t CNetwork::ConnectFromUI(char* host)
{
	cout << "ConnectFromUI to " << host << endl;
	
	//protocol->connection->SetRemoteAddress(host); OliverESP: adds more control
	if (protocol->connection->SetRemoteAddress(host) == B_ERROR) 
		return EHOSTUNREACH;
	
	gettimeofday(&startTime, NULL);

	m_connectedRTCP = true;
	
	if(find_thread(NET_THREAD_SEND_RTCP) == B_NAME_NOT_FOUND)
	{
		thread_id idSendRTCP;
		if ( (idSendRTCP = spawn_thread(SendModeRTCPGopher, NET_THREAD_SEND_RTCP, B_NORMAL_PRIORITY, this )) < B_OK)
		{
			cout << "Error spawning NET_THREAD_SEND_RTCP" << endl;
			return idSendRTCP;
		}
		resume_thread(idSendRTCP);
	}
	snooze(100000);

	system_beep("Calling");  //OliverESP EVENT!!

	if(send_data(find_thread(NET_THREAD_SEND_RTCP), NET_ID_CONNECT, NULL, 0) != B_NO_ERROR)
	{
		cout << "ERROR: Unable to send NET_ID_CONNECT to NET_THREAD_SEND_RTCP: CNetwork::ConnectFromUI)" << endl;
		return B_ERROR;
	}
	
	if(find_thread(NET_THREAD_WAIT_CONNECTION) == B_NAME_NOT_FOUND)
	{
		thread_id idWaitConnection;
		if ((idWaitConnection = spawn_thread(WaitConnectionGopher, NET_THREAD_WAIT_CONNECTION, B_NORMAL_PRIORITY, this)) < B_OK)
		{
			cout << "Error spawning NET_THREAD_SEND_RTCP" << endl;
			return idWaitConnection;
		}
		resume_thread(idWaitConnection);
	}	
	snooze(100000);
	
	return B_OK;
}


//****************************************************************************************
status_t
CNetwork::InitializeDiscussion()
{
	m_connectedRTP = true;

	if(find_thread(NET_THREAD_RECV_RTP) == B_NAME_NOT_FOUND)
	{
 		thread_id idRecvRTP;
		if ((idRecvRTP = spawn_thread(RecvModeRTPGopher, NET_THREAD_RECV_RTP, B_NORMAL_PRIORITY, this)) < B_OK) 
		{
			cout << "Error spawning NET_THREAD_RECV_RTP" << endl;
			return idRecvRTP;
		}		
		resume_thread(idRecvRTP);
	}

	if(find_thread(NET_THREAD_SEND_RTP) == B_NAME_NOT_FOUND)
	{
		thread_id idSendRTP;
		if ((idSendRTP = spawn_thread(SendModeRTPGopher, NET_THREAD_SEND_RTP, B_NORMAL_PRIORITY, this)) < B_OK)
		{
			cout << "Error spawning NET_THREAD_SEND_RTP" << endl;
			return idSendRTP;
		}
		resume_thread(idSendRTP);	
	}
 
	if(find_thread(NET_THREAD_PIT) == B_NAME_NOT_FOUND)
	{
		thread_id idPit;
		if ((idPit = spawn_thread(PitGopher, NET_THREAD_PIT, B_NORMAL_PRIORITY, this)) < B_OK)
		{
			cout << "Error spawning NET_THREAD_PIT" << endl;
			return idPit;
		}
		resume_thread(idPit);
	}

	snooze(100000);

	return B_OK;
}


//****************************************************************************************
//	Procedure:	CNetwork::Disconnect
//	Fonction:	
//****************************************************************************************
status_t CNetwork::Disconnect()
{
	cout << "Disconnecting..." << endl;

	if (!m_connectionReady)
		return B_OK;

	if(send_data(find_thread(NET_THREAD_SEND_RTCP), NET_ID_DISCONNECT, NULL, 0) != B_NO_ERROR)
	{
		cout << "ERROR: Unable to send NET_ID_DISCONNECT to NET_THREAD_SEND_RTCP: CNetwork::Disconnect)" << endl;
		return B_ERROR;;
	}

	Terminate();

	return B_OK;
}

//****************************************************************************************
//	Procedure:	CNetwork::SendModeRTPGopher
//	Fonction:	Interface statique entre StartNetwork et SendModeRTP
//****************************************************************************************
status_t CNetwork::SendModeRTPGopher(void* arg)
{
	CNetwork* networkThis = (CNetwork *)arg; 
	return (networkThis->SendModeRTP()); 
}


//****************************************************************************************
//	Procedure:	CNetwork::SendModeRTP
//	Fonction:	Thread gerant l'envoie de trames RTP
//****************************************************************************************
status_t CNetwork::SendModeRTP(void)
{
	cout << "------------SendRTP------------" << endl;
	TRtpData rtpDataBuffer;

	protocol->connection->RtpUdpMode(SEND);

	while(m_connectedRTP)
	{
	//	if (has_data(find_thread(NET_THREAD_SEND_RTP))) //OliverESP: improvement
		if (has_data(find_thread(NULL))) 
		{
			switch(receive_data(NULL, &rtpDataBuffer, sizeof(TRtpData)))  // OliverESP: recives ALL!! and may just need 1Kb of the data...
			{
			case NET_ID_RAW:
				//cout << "SENDRTP: Sending RTP packet: " << rtpDataBuffer.data << endl;
				//if(protocol->SendRTPPacket(rtpDataBuffer.data, rtpDataBuffer.timestamp, NET_ID_RAW) != B_OK)   //OliverESP: 2nd rev
				if(protocol->SendRTPPacket(rtpDataBuffer, NET_ID_RAW) != B_OK)
				{
					cout << "SENDRTP: ERROR: Unable to send RTP packet: CNetwork::SendModeRTP" << endl;
					return B_ERROR;	
				}
				break;
			case NET_ID_CODEC:
				//cout << "SENDRTP: Sending RTP packet: " << rtpDataBuffer.data << endl;
				// if(protocol->SendRTPPacket(rtpDataBuffer.data, rtpDataBuffer.timestamp, NET_ID_CODEC) != B_OK)
				if(protocol->SendRTPPacket(rtpDataBuffer, NET_ID_CODEC) != B_OK)
				{
					cout << "SENDRTP: ERROR: Unable to send RTP packet: CNetwork::SendModeRTP" << endl;
					return B_ERROR;	
				}
				break;
			case NET_ID_MODEM:
				//cout << "SENDRTP: Sending RTP packet: " << rtpDataBuffer.data << endl;
				//if(protocol->SendRTPPacket(rtpDataBuffer.data, rtpDataBuffer.timestamp, NET_ID_MODEM) != B_OK)
				if(protocol->SendRTPPacket(rtpDataBuffer, NET_ID_MODEM) != B_OK)
				{
					cout << "SENDRTP: ERROR: Unable to send RTP packet: CNetwork::SendModeRTP" << endl;
					return B_ERROR;	
				}
				break;
			default:
				cout << "SENDRTP: ERROR: Unknown message trying to be sent: CNetwork::SendModeRTP" << endl;
				return B_ERROR;
				break;
			}
		}
		else
			snooze(5000);
	}
	return B_OK;
}


//****************************************************************************************
//	Procedure:	CNetwork::RecvModeRTPGopher
//	Fonction:	Interface statique entre StartNetwork et RecvModeRTP
//****************************************************************************************
status_t CNetwork::RecvModeRTPGopher(void* arg)
{
	CNetwork* networkThis = (CNetwork *)arg; 
	return (networkThis->RecvModeRTP()); 
}


//****************************************************************************************
//	Procedure:	CNetwork::RecvModeRTP
//	Fonction:	Thread gerant la reception de trames RTP
//****************************************************************************************
status_t CNetwork::RecvModeRTP(void)
{
	ssize_t size;
	cout << "------------RecvRTP------------" << endl;
	char payload[RTPPLLEN_MAX];

	protocol->connection->RtpUdpMode(RECV);

	while(m_connectedRTP)
	{
		switch(protocol->RecvRTPPacket(payload,&size))
		{
			case NET_ID_EMPTY:
				snooze(5000);
				break;
				
			case NET_ID_RAW:
				if (m_pPlayer)
				//	m_pPlayer->PlayOld(payload, RTPPLLEN_RAW); OliverESP: 2nd rev
					m_pPlayer->PlayOld(payload, size,NET_ID_RAW);   
				
				//cout << "RECVRTP: Receiving AUDIO RTP packet: " << payload << endl;
				break;
			case NET_ID_CODEC:
				if (m_pPlayer)
					//m_pPlayer->PlayOld(payload, RTPPLLEN_CODEC); OliverESP: 2nd rev
					m_pPlayer->PlayOld(payload, size, NET_ID_CODEC);
				//cout << "RECVRTP: Receiving CODEC RTP packet: " << payload << endl;
				break;
			case NET_ID_MODEM:
				if (m_pPlayer)
					//m_pPlayer->PlayOld(payload, RTPPLLEN_MODEM); OliverESP: 2nd rev
					m_pPlayer->PlayOld(payload, size, NET_ID_MODEM);
				//cout << "RECVRTP: Receiving MODEM RTP packet: " << payload << endl;
				break;
								
			default:
				cout << "RECVRTP: ERROR: Unknown message trying to be received: CNetwork::RecvModeRTP" << endl;
				break;
		};				
	}
	return B_OK;
}


//****************************************************************************************
//	Procedure:	CNetwork::SendModeRTCPGopher
//	Fonction:	Interface statique entre StartNetwork et SendModeRTCP
//****************************************************************************************
status_t CNetwork::SendModeRTCPGopher(void* arg)
{
      CNetwork* networkThis = (CNetwork *)arg; 
      return (networkThis->SendModeRTCP()); 
}


//****************************************************************************************
//	Procedure:	CNetwork::SendModeRTCP
//	Fonction:	Thread gerant l'envoie de trames RTCP
//****************************************************************************************
status_t CNetwork::SendModeRTCP(void)
{
	cout << "------------SendRTCP------------" << endl;
	protocol->connection->RtcpUdpMode(SEND);

	while(true)
	{
		//if (has_data(find_thread(NET_THREAD_SEND_RTCP))) //OliverESP: improvement
		if (has_data(find_thread(NULL)))
		{
			switch(receive_data(NULL, NULL, 0))
			{
				case(NET_ID_CONNECT):
					cout << "SENDRTCP: Sending CONNECT message" << endl;	
					protocol->SendRTCPPacket(NET_ID_CONNECT);
					break;
				case(NET_ID_DISCONNECT):
					cout << "SENDRTCP: Sending DISCONNECT message" << endl;
					protocol->SendRTCPPacket(NET_ID_DISCONNECT);
					snooze(100000);
					return B_OK;
					break;
				case(NET_ID_ALIVE):
#ifdef DEBUG_ALIVE
					cout << "SENDRTCP: Sending ALIVE message" << endl;
#endif	
					protocol->SendRTCPPacket(NET_ID_ALIVE);
					break;
				case(NET_ID_ACCEPT):
					cout << "SENDRTCP: Sending ACCEPT message" << endl;
					protocol->SendRTCPPacket(NET_ID_ACCEPT);
					break;
				case(NET_ID_REFUSE):
					cout << "SENDRTCP: Sending REFUSE message" << endl;
					protocol->SendRTCPPacket(NET_ID_REFUSE);
					break;
				case(NET_ID_BUSY):
					cout << "SENDRTCP: Sending BUSY message" << endl;
					protocol->SendRTCPPacket(NET_ID_BUSY);
					break;
				case(NET_ID_ABORT):
					cout << "SENDRTCP: Sending ABORT message" << endl;
					protocol->SendRTCPPacket(NET_ID_ABORT);
					break;
				default:
					cout << "SENDRTCP: ERROR: Unknown message trying to be sent: CNetwork::SendModeRTCP" << endl;
					return B_ERROR;
					break;
			}
		}
		else
		{
			if (!m_connectedRTCP)
				break;
			snooze(5000);
		}
	}
	return B_OK;
}


//****************************************************************************************
//	Procedure:	CNetwork::RecvModeRTCPGopher
//	Fonction:	Interface statique entre StartNetwork et RecvModeRTCP
//****************************************************************************************
status_t CNetwork::RecvModeRTCPGopher(void* arg)
{
	CNetwork* networkThis = (CNetwork *)arg; 
	return (networkThis->RecvModeRTCP()); 
}


//****************************************************************************************
//	Procedure:	CNetwork::RecvModeRTCP
//	Fonction:	Thread gerant l'envoie de trames RTCP
//****************************************************************************************
status_t CNetwork::RecvModeRTCP(void)
{
	cout << "------------RecvRTCP------------" << endl;
	time_t lastTimeRecv = real_time_clock();
	protocol->connection->RtcpUdpMode(RECV); // set the sockets
	
	//ulong lRemoteAddress;     //OliverESP
	sockaddr_in lRemoteAddress; //OliverESP

	bool bFirstConnection = false;

	while(true)
	{
		// cout << "Awaiting some connection" << endl; 
		switch(protocol->RecvRTCPPacket(lRemoteAddress))
		{
			case NET_ID_EMPTY:
				// cout << "NET_ID_EMPTY" << endl;
				break;
					
			case NET_ID_CONNECT:
			{
				cout << "RECVRTCP: Receiving Restez-vous chez vos parents" << endl;

				lastTimeRecv = real_time_clock();
				gettimeofday(&startTime, NULL);
				// protocol->connection->SetRemoteAddress(lRemoteAddress);       //OliverESP
				protocol->connection->SetRemoteConnectingAddress(lRemoteAddress);//OliverESP : explained in connection.cc

				m_connectedRTCP = true;
				if(find_thread(NET_THREAD_SEND_RTCP) == B_NAME_NOT_FOUND)
				{
					thread_id idSendRTCP;
					if ( (idSendRTCP = spawn_thread(SendModeRTCPGopher, NET_THREAD_SEND_RTCP, B_NORMAL_PRIORITY, this )) < B_OK )
					{
						return idSendRTCP;
					}
					resume_thread(idSendRTCP);
				}
				snooze(100000);

				////////////////////////////// BUSY
				if(m_connectedRTP)
				{					
					if(send_data(find_thread(NET_THREAD_WAIT_CONNECTION), NET_ID_BUSY, NULL, 0) != B_NO_ERROR)
					{
						cout << "ERROR: Unable to send NET_ID_REFUSE to NET_THREAD_WAIT_CONNECTION: CNetwork::RecvModeRTCP)" << endl;
					}
					break;
				}
				
				UpdateConnection(AcceptConnection());
				break;
			}				
			case NET_ID_DISCONNECT:
			{
				cout << "RECVRTCP: Receiving Disconnect" << endl;

				bFirstConnection = false;

				BMessage msg;
				msg.what = MSG_MENU_TERMINATE;
				BMessenger(m_pBeNetWindow).SendMessage(&msg);

				Terminate();
				break;
			}				
			case NET_ID_ACCEPT:
			{
				cout << "Receive ACCEPT from remote" << endl;
				if(send_data(find_thread(NET_THREAD_WAIT_CONNECTION), NET_ID_ACCEPT, NULL, 0) != B_NO_ERROR)
				{
					cout << "ERROR: Unable to send NET_ID_ACCEPT to NET_THREAD_WAIT_CONNECTION: CNetwork::RecvModeRTCP)" << endl;
				}
				break;
			}	
			case NET_ID_REFUSE:
			{
				cout << "Receive REFUSE from remote" << endl;
				if(send_data(find_thread(NET_THREAD_WAIT_CONNECTION), NET_ID_REFUSE, NULL, 0) != B_NO_ERROR)
				{
					cout << "ERROR: Unable to send NET_ID_REFUSE to NET_THREAD_WAIT_CONNECTION: CNetwork::RecvModeRTCP)" << endl;
				}
				break;
			}
			case NET_ID_BUSY:
			{
				cout << "Receive BUSY from remote" << endl;
				if(send_data(find_thread(NET_THREAD_WAIT_CONNECTION), NET_ID_BUSY, NULL, 0) != B_NO_ERROR)
				{
					cout << "ERROR: Unable to send NET_ID_BUSY to NET_THREAD_WAIT_CONNECTION: CNetwork::RecvModeRTCP)" << endl;
				}
				break;
			}
			case NET_ID_ABORT:
			{
				cout << "Receive ABORT from remote" << endl;
				if (m_alertAccept != NULL)
				{
					BMessage msg;
					msg.what = B_QUIT_REQUESTED;
					BMessenger(m_alertAccept).SendMessage(&msg);
				}
				break;
			}
			case NET_ID_ALIVE:
			{
#ifdef DEBUG_ALIVE
				cout << "RECVRTCP: Receiving Keep Alive" << endl;
#endif
				lastTimeRecv = real_time_clock();
				break;
			}						
			default:
			{
				cout << "RECVRTCP: ERROR: Unknown message trying to be received: CNetwork::RecvModeRTCP" << endl;
				break;
			}
		};

		if (m_connectionReady && !bFirstConnection)
		{
			lastTimeRecv = real_time_clock();
			bFirstConnection = true;
		}

		if( m_connectionReady && ((real_time_clock() - lastTimeRecv) > NET_TIMEOUT_TIME) )
		{
			cout << "RECVRTCP: Timeout" << endl;

			BMessage msg;
			msg.what = MSG_MENU_DISCONNECT;
			BMessenger(m_pBeNetWindow).SendMessage(&msg);

			bFirstConnection = false;
			Terminate();
		}

		snooze(NET_RTCP_POLLING);
	}
	return B_OK;
}


//****************************************************************************************
//	Procedure:	CNetwork::PitGopher
//	Fonction:	Interface statique entre StartNetwork et Pit
//****************************************************************************************
status_t CNetwork::PitGopher(void* arg)
{
	CNetwork* networkThis = (CNetwork *)arg; 
	return (networkThis->Pit()); 
}


//****************************************************************************************
//	Procedure:	CNetwork::Pit
//	Fonction:	Thread generant periodiquement un message NET_ID_ALIVE
//****************************************************************************************
status_t CNetwork::Pit(void)
{
	// !!!SWEET BUG!!! S'assurer que le pit envoie pas trop vite son premier KEEP ALIVE a
	// NET_THREAD_SEND_RTCP, car NET_THREAD_SEND_RTCP est encore instable (en train d'initialiser
	// ses sockets dans CConnection) pis s'il recoit un message... ca fuck.
	snooze(2000000);

	while(m_connectedRTP)
	{
		if(send_data(find_thread(NET_THREAD_SEND_RTCP), NET_ID_ALIVE, NULL, 0) != B_NO_ERROR)
		{
			cout << "ERROR: Unable to send NET_ID_ALIVE to NET_THREAD_SEND_RTCP: CNetwork::Pit" << endl;
			return B_ERROR;
		}
		
		snooze(NET_KEEPALIVE_TIME);
	}
	return B_OK;
}


//****************************************************************************************
//	Procedure:	CNetwork::Terminate
//	Fonction:	
//				Appartient au thread NET_THREAD_RECV_RTCP
//****************************************************************************************
void CNetwork::Terminate()
{
	Output::Instance()->Network("Terminating...\n");

	// OliverESP send message to toggle: are all cases here covered?
	BMessenger(m_connectview).SendMessage(new BMessage(MSG_CONNECTION_FINISH));  

	m_connectionReady = false;
	m_connectedRTP = false;
	m_connectedRTCP = false;
}


//****************************************************************************************
//	Procedure:	CNetwork::GetTimeStamp
//	Fonction:	Calculer le temps ecoule depuis "startTime"
//	Retourne:	timeStamp (entier sur 32 bits)
//****************************************************************************************
int CNetwork::GetTimeStamp() 
{
	int timestamp;
	timeval actualTime;
	timeval elapsedTime;

	gettimeofday(&actualTime, NULL);
	timersub(&actualTime, &startTime, &elapsedTime);
	timestamp = (elapsedTime.tv_sec * 1000) + (elapsedTime.tv_usec % 1000 > 500 ? elapsedTime.tv_usec/1000 + 1 : elapsedTime.tv_usec/1000);

	return timestamp;
}


//****************************************************************************************
bool
CNetwork::AcceptConnection()
{
	Output::Instance()->Network("Incoming connection... accept?\n");
	system_beep("Phone Call");  //OliverESP EVENT!!
	m_alertAccept = new BAlert(	_T("Incoming connection"), _T("Accept call?"),
								_T("No"), _T("Yes"), NULL, 
		B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_INFO_ALERT); 
	m_alertAccept->SetShortcut(1, B_ENTER); 
	int32 button_index = m_alertAccept->Go();
	m_alertAccept = NULL;
	
	if (button_index != 1)
		return false;
	else
		return true;
}

//***************************************************************************************
void
CNetwork::UpdateConnection(bool result)
{
	m_alertAccept = NULL;	
	if(result)	//  Ask user to accept the call.
	{
		cout << "Connection accepted" << endl;

		if(send_data(find_thread(NET_THREAD_SEND_RTCP), NET_ID_ACCEPT, NULL, 0) != B_NO_ERROR)
		cout << "ERROR: Unable to send NET_ID_ACCEPT to NET_THREAD_SEND_RTCP: CNetwork::RecvModeRTCP)" << endl;

		if (InitializeDiscussion() == B_OK)
		{
			snooze(10000); // ?????????????????????????????????????
	
			BMessage msg;
			msg.what = MSG_MENU_ACCEPT;
			BMessenger(m_pBeNetWindow).SendMessage(&msg);
	
			m_connectionReady = true;
		}
		else
		{
			cout << "InitializeDiscussion failed" << endl;
			m_connectedRTCP = false;
		}
	}
	else
	{
		cout << "Connection refused" << endl;

		if(send_data(find_thread(NET_THREAD_SEND_RTCP), NET_ID_REFUSE, NULL, 0) != B_NO_ERROR)
			cout << "ERROR: Unable to send NET_ID_REFUSE to NET_THREAD_SEND_RTCP: CNetwork::RecvModeRTCP)" << endl;

		m_connectedRTCP = false;
	}
}

//****************************************************************************************
status_t
CNetwork::WaitConnectionGopher(void* arg)
{
	CNetwork* networkThis = (CNetwork *)arg; 
	return (networkThis->WaitConnection()); 
}


//****************************************************************************************
status_t
CNetwork::WaitConnection(void)
{
	Output::Instance()->Network("Wait for response from remote user\n");

	TRtpData rtpDataBuffer;
	time_t startTimeout = real_time_clock();
	while ((real_time_clock() - startTimeout) < NET_TIMEOUT_TIME)
	{
		//if (has_data(find_thread(NET_THREAD_WAIT_CONNECTION))) //OliverESP:Improvement
		if (has_data(find_thread(NULL))) 
		{
			switch(receive_data(NULL, &rtpDataBuffer, sizeof(TRtpData)))
			{
			case NET_ID_ACCEPT:
			{
				if (InitializeDiscussion() != B_OK)
					return B_ERROR;

				m_connectionReady = true;

				BMessage msg;
				msg.what = MSG_MENU_ACCEPT;
				BMessenger(m_pBeNetWindow).SendMessage(&msg);
				return B_OK;   // succesfull
			}
			case NET_ID_REFUSE:
			{
				m_connectedRTCP = false;
	
				BAlert *myAlert = new BAlert( "Connection refused", _T("Connection refused."), "Ok");
				myAlert->Go();
				BMessenger(m_connectview).SendMessage(new BMessage(MSG_CONNECTION_FINISH));  // OliverESP send message to toggle
				return B_OK;  // failed
			}
			case NET_ID_BUSY:
			{
				m_connectedRTCP = false;
			
				system_beep("Busy");  //OliverESP EVENT!!
	
				BAlert *myAlert = new BAlert( "Busy", _T("The remote user is busy."), "Ok"); 
				myAlert->Go();
				BMessenger(m_connectview).SendMessage(new BMessage(MSG_CONNECTION_FINISH));  // OliverESP send message to toggle
				return B_OK;  // failed
			}
			}
		}
		else
			snooze(5000);
	}

	if(send_data(find_thread(NET_THREAD_SEND_RTCP), NET_ID_ABORT, NULL, 0) != B_NO_ERROR)
	{
		cout << "ERROR: Unable to send NET_ID_ABORT to NET_THREAD_SEND_RTCP" << endl;
		return B_ERROR;
	}

	m_connectedRTCP = false;
	BAlert *myAlert = new BAlert( "Connection timeout", _T("Connection timeout"), "Ok"); 
	myAlert->Go();
	BMessenger(m_connectview).SendMessage(new BMessage(MSG_CONNECTION_FINISH));  // OliverESP send message to toggle
	return B_OK;
}
void
CNetwork::SetView(ConnectView* view){
	
	m_connectview = view;

}
