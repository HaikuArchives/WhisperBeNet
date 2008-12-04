#ifndef __EMAIL_IP_MANAGER__
#include "EMailIPManager.h"
#endif

EMailIPManager* EMailIPManager::m_pInstance = NULL;

EMailIPManager::EMailIPManager()
{
	m_iThreadID = 0;
	m_iIntervale = 0;
}

EMailIPManager::~EMailIPManager()
{
	if(m_iThreadID)KillThread();
}

EMailIPManager * EMailIPManager::Instance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new EMailIPManager;
	}
	return m_pInstance;
}//fin de Instance.


bool EMailIPManager::CreateThread(int iIntervale)
{
	m_iIntervale = iIntervale;
	if(m_iThreadID)
	{
		return false;
	}
	m_iThreadID = spawn_thread(EMailCheckThread, "EMailCheck", B_LOW_PRIORITY, NULL);
	resume_thread(m_iThreadID);	
	return true;
}

bool EMailIPManager::KillThread()
{
	if(!m_iThreadID)
	{
		return false;
	}
	kill_thread(m_iThreadID);
	m_iThreadID = 0;
	return true;
}

bool EMailIPManager::CheckNow(bool bCheck)
{
	PEOPLE_ID_NODE * pNode;
	pNode = m_email.CheckMail(bCheck);
	if(pNode)
	{
		BeNetMailIPWindow * pWindow;
		pWindow = new BeNetMailIPWindow(pNode);
		pWindow->Show();
	}

	return true;
}

int32 EMailIPManager::EMailCheckThread(void* data)
{
	EMailIPManager::Instance()->CheckNow(true);
	while(1)
	{
		snooze(EMailIPManager::Instance()->Intervale() * 60000000);
		EMailIPManager::Instance()->CheckNow(false);
	}
}
