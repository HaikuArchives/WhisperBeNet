#ifndef _NetworkInterface_h
#include "NetworkInterface.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#include <stdio.h>
#include <string.h>

// singleton initialization
NetworkInterface* NetworkInterface::m_instance = 0;

NetworkInterface::NetworkInterface() :
	m_pPlayer(NULL)
{

}

NetworkInterface::~NetworkInterface()
{

}

NetworkInterface*
NetworkInterface::Instance()
{
	if (!m_instance)
		m_instance = new NetworkInterface();
	return m_instance;
}

void
NetworkInterface::Send(void* data, size_t size)
{
	Receive(data, size);
}

void
NetworkInterface::Receive(void* data, size_t size)
{
	//OliverRD NON USED CLASS, code kept for references
	if (m_pPlayer)  
		m_pPlayer->PlayOld(data, size, 0);
}
