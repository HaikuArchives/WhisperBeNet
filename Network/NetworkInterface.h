#ifndef _NetworkInterface_h
#define _NetworkInterface_h

#ifndef _Player_h
#include "Player.h"
#endif

class NetworkInterface
{
public:
	~NetworkInterface();
	static NetworkInterface* Instance();

	void	Send(void* data, size_t size);
	void	Receive(void* data, size_t size);
	void	SetPlayer(Player* player)			{m_pPlayer = player;}

private: // functions	
	NetworkInterface();
	
private: // data
	static NetworkInterface*	m_instance;
	Player*						m_pPlayer;
};

#endif // _NetworkInterface_h
