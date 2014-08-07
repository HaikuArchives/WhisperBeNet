#ifndef __EMAIL_IP_MANAGER__
#define __EMAIL_IP_MANAGER__

#include "EMailIP.h"
#include "BeNetMailIPWindow.h"



class EMailIPManager
{
public:
	~EMailIPManager();
	static EMailIPManager * Instance();
	bool CreateThread(int);
	bool KillThread();
	bool CheckNow(bool bCheck);
	bool SendIP(char * pzMailTo, char * pzMyNickName, char * pzMyIPAddress, char * pzImageFile, char * pzMessage ,int iDelay)
		{ return m_email.SendIP(pzMailTo, pzMyNickName, pzMyIPAddress, pzImageFile, pzMessage ,iDelay);}
	
private: //Function
	EMailIPManager();
	static int32 EMailCheckThread(void* p);
	int64 Intervale() { return m_iIntervale;}
		
private: //Data
	static	EMailIPManager * m_pInstance;
	int m_iThreadID;
	int64 m_iIntervale;
	EMailIP m_email; 
};

#endif // __EMAIL_IP_MANAGER__
