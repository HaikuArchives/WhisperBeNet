#ifndef _CLIENT_H
#define _CLIENT_H

#include <OS.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

class TClient
{
public:
	~TClient();
	static TClient*	Instance();

	char* 			GetIPByEmail(char *email);
	char* 			GetIPByName(char *last_name, char* first_name);
	char* 			GetFirstNameByIP(const char *ip);
	char* 			GetLastNameByIP(const char *ip);
	char* 			GetEmailByIP(const char *ip);
	bool			Register(char *last_name, char *first_name, char *email);
	void			Deregister();
	bool			IsOnline()			{return m_bIsOnline;}
	
	void		Cmd(); //IRA: moved it around
private: // functions
	TClient();

	int			ConnectTCP();
	bool		Go();
	void		SendCmd(char*);
	char*		GetResult();

private: // data
	static TClient*	m_instance;

	int				m_remoteAddress_size;
	sockaddr_in		m_remoteAddress;
	int				m_sStream;
	thread_id		my_thread;
	sem_id			cmd_lock;
	sem_id			result_lock;
	bool			result_ok;
	char*			CMD;
	char*			RESULT;
	bool			m_bIsOnline;
};

#endif
