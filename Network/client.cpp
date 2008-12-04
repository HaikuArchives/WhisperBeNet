#ifndef _CLIENT_H
#include "client.h"
#endif

#ifndef _Preferences_h
#include "Preferences.h"
#endif

#ifndef _WaitWindow_h
#include "WaitWindow.h"
#endif

#include <arpa/inet.h>
#include <unistd.h>

#include <Alert.h>
#include <locale/Locale.h>

TClient* TClient::m_instance = 0;

TClient::TClient() :
	m_bIsOnline(false)
{
	m_remoteAddress_size = sizeof (m_remoteAddress);

	my_thread = -1;
	
   	cmd_lock = create_sem(1, "CMD lock");
	result_lock = create_sem(1, "RECV lock");
}

TClient::~TClient ()
{
	delete_sem(cmd_lock);
	delete_sem(result_lock);
}

TClient* TClient::Instance()
{
	if (!m_instance)
		m_instance = new TClient;
	return m_instance;
}



char* TClient::GetIPByEmail(char *email){
	char cmd[1024];
	strcpy(cmd, "find ");
	strcat(cmd, email);
	strcat(cmd, ";");
	SendCmd(cmd);
	return GetResult();
}

char* TClient::GetIPByName(char *last_name, char* first_name){
	char cmd[1024];
	strcpy(cmd, "find ");
	strcat(cmd, last_name);
	strcat(cmd, "|");
	strcat(cmd, first_name);
	strcat(cmd, ";");
	SendCmd(cmd);
	return GetResult();
}

char* TClient::GetFirstNameByIP(const char *ip){
	char cmd[1024];
	strcpy(cmd, "getFirstName ");
	strcat(cmd, ip);
	strcat(cmd, ";");
	SendCmd(cmd);
	return GetResult();
}

char* TClient::GetLastNameByIP(const char *ip){
	char cmd[1024];
	strcpy(cmd, "getLastName ");
	strcat(cmd, ip);
	strcat(cmd, ";");
	SendCmd(cmd);
	return GetResult();
}

char* TClient::GetEmailByIP(const char *ip){
	char cmd[1024];
	strcpy(cmd, "getEmail ");
	strcat(cmd, ip);
	strcat(cmd, ";");
	SendCmd(cmd);
	return GetResult();
}

bool TClient::Register(char *last_name, char *first_name, char *email){
	if(Go())
	{
		char cmd[1024];
		strcpy(cmd, "info ");
		strcat(cmd, last_name);
		strcat(cmd, "|");
		strcat(cmd, first_name);
		strcat(cmd, "|");
		strcat(cmd, email);
		strcat(cmd, ";");
		SendCmd(cmd);
		
		m_bIsOnline = true;

		return true;
	}
	return false;
}

void TClient::Deregister()
{
	SendCmd("del;");
	m_bIsOnline = false;
}


int TClient::ConnectTCP ()
{
	//printf("ConnectTCP\n");
	m_sStream = socket (AF_INET, SOCK_STREAM, 0);  // orienter connection

	// etablie la connection dans le socket orienter connection
	if (connect (m_sStream, (const struct sockaddr*)&m_remoteAddress, m_remoteAddress_size) != 0)
	{
		return -1;
	}

	//printf("Connected with %s\n", inet_ntoa (m_remoteAddress.sin_addr));

	return 0;
}

void TClient::Cmd()
{
	int redo = 1;	
	do
	{	
		char *sendbuffer = new char[strlen(CMD)];
		strcpy(sendbuffer, CMD);
		if(send(m_sStream, sendbuffer, strlen(sendbuffer), 0 ) < 0)
		{
			printf("error send\n");
			break;
		}
		delete sendbuffer;

		if(strcmp(CMD, "del;"))
		{
			if(strstr(CMD, "alive;") == NULL && strstr(CMD, "info") == NULL)
			{
				int length = 0;
				char aChar = ' ';
				if(recv(m_sStream, &length, sizeof(char), 0 ) < 0)
				{
					printf("error read length");
					break;
				}
				else 
				{
					RESULT = new char[length + 1];
					int i = 0;
					while(i < length)
					{				
						if(recv(m_sStream, &aChar, sizeof(char), 0 ) < 0)
						{
							printf("error read");
							redo = 0;
							break;
						}
						else
						{
							RESULT[i] = aChar;
							i++;
						}
					}
					RESULT[i] = '\0';
					printf("result: %s\n", RESULT);
				}
			}
				
			redo = 1;
			strcpy(CMD, "alive;");
			release_sem(cmd_lock);
			release_sem(result_lock);
			
			snooze(50 * 1000 * 1000);
		}
		else
		{
			redo = 0;
		}
	}
	while (redo);

	close(m_sStream);
	my_thread = -1;	
}

void TClient::SendCmd(char *cmd)
{
	if (my_thread != -1)
	{
		acquire_sem(cmd_lock);
		acquire_sem(result_lock);
		strcpy(CMD, cmd);
		suspend_thread(my_thread);
		resume_thread(my_thread);
	}
}

char* TClient::GetResult()
{
	acquire_sem(result_lock);
	char *result = new char[strlen(RESULT)];
	strcpy(result, RESULT);
	RESULT = NULL;
	release_sem(result_lock);
	return result;
}

static int32 Cmd_helper(void* data)
{
	((TClient*)data)->Cmd();
	
	// todo: Why does this method return a value? What should it be?
	return 0;
}

bool TClient::Go()
{
//OliESP
		WaitWindow* waitWindow = new WaitWindow;
		waitWindow->Show();
//		waitWindow->SetText("server");
		waitWindow->SetText(Preferences::Instance()->ServerIP());
		
	struct hostent* hostRemote;
	hostRemote = gethostbyname(Preferences::Instance()->ServerIP());
	if (hostRemote == NULL) {
		BAlert *myAlert = new BAlert("Error",
								 _T("Unable to get server information\nPlease verify in Network preferences the server's address"),
								 "Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT);
		myAlert->Go();	

		waitWindow->Lock();			 // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! not here!
		waitWindow->Quit();          //OliverESP: if we dont Quit() it, it will acumulate with anothers
		
		return false;
	}
	
	waitWindow->SetText(_T(" Server found"));
	
//OliESP

	if(my_thread != -1)
		return true;

	m_remoteAddress.sin_family = AF_INET;
	m_remoteAddress.sin_port = htons(Preferences::Instance()->ServerPort());
	m_remoteAddress.sin_addr.s_addr = inet_addr(Preferences::Instance()->ServerIP());

	CMD = new char[1024];
	strcpy(CMD, "alive;");	

	if (ConnectTCP() == 0)
	{   
		waitWindow->SetText(_T(" TCP connection established"));
		my_thread = spawn_thread(Cmd_helper, "Cmd Thread", 10, this); 
		resume_thread(my_thread);
		
		waitWindow->SetText(_T(" Ressuming thread"));
		waitWindow->Lock();			 // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! not here!
		waitWindow->Quit();          //OliverESP: if we dont Quit() it, it will acumulate with anothers

		return true;
	}
	
	waitWindow->SetText(_T(" Impossible to connect"));	
	waitWindow->Lock();			 // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! not here!
	waitWindow->Quit();          //OliverESP: if we dont Quit() it, it will acumulate with anothers

	BAlert *myAlert = new BAlert("Error",
							 _T("Impossible to establish the TCP connection"),
							 "Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT);
	myAlert->Go();	
	
	
	return false;
	
}
