#ifndef _EMAILIP_
#define _EMAILIP_

#include <E-mail.h>
#include <stdlib.h>

class BFile;

#define BENET_EMAIL_SUBJECT_ID "BeNet IP Address"

struct PEOPLE_ID_NODE
{
	char pzIP[16];
	char pzName[32];
	char pzEMail[128];
	char pzDate[64];
	char pzPicture[256];		//Le path de l'image.
	char pzFile[256];			//Le path du fichier de mail.
	char pzMessage[1024];
	
	PEOPLE_ID_NODE* pNext;
	PEOPLE_ID_NODE* pPrev;
};


class EMailIP
{
public:
	EMailIP();
	PEOPLE_ID_NODE* CheckMail(bool bCheckAll = true);
	bool SendIP(char* pzMailTo, char* pzMyNickName, char* pzMyIPAddress, char* pzImageFile, char* pzMessage, int iDelay);
		
private: // functions
	bool GetLine(BFile* pFile, char* pzBuffer);
	bool CheckIfTheMailIsValid(BFile* pFile);
	PEOPLE_ID_NODE* GetInfo(BFile* pFile);
	void AddNode(PEOPLE_ID_NODE* pNode);
	bool FindString(BFile* pFile, char* pzString);

private: // data		
	PEOPLE_ID_NODE* m_pFirstID;
};


#endif // _EMAILIP_
