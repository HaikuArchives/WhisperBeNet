#ifndef _EMAILIP_
#include "EMailIP.h"
#endif

#include <File.h>
#include <Entry.h>
#include <Directory.h>
#include <Path.h>

#include <stdio.h>
#include <string.h>

EMailIP::EMailIP()
{
	m_pFirstID = NULL;
}

bool EMailIP::SendIP(char * pzMailTo, char * pzMyNickName, char * pzMyIPAddress, char * pzImageFile, char * pzMessage ,int iDelay)
{
	BMailMessage * email;
	char pzContent[1024];
	BFile file;
	off_t size;
	
	//Convertir le delay en seconde depuis le 1 janvier 1970.
	iDelay = iDelay * 60 + real_time_clock();
	sprintf(pzContent, "<IP>%s\n\r<NickName>%s\n\r<Delay>%i\n\r<Message>%s<EndMessage>", pzMyIPAddress, pzMyNickName, iDelay, pzMessage);
	
	email = new BMailMessage();
	email->AddHeaderField(B_MAIL_TO, pzMailTo);
	email->AddHeaderField(B_MAIL_SUBJECT, BENET_EMAIL_SUBJECT_ID);
	
	email->AddContent(pzContent, strlen(pzContent));
	

	if(file.SetTo(pzImageFile, B_READ_ONLY) == B_OK)
	{
		file.GetSize(&size);
		if(size <= 32000)
		{
			email->AddEnclosure(pzImageFile);
		}
	}
	
	if(email->Send() != B_OK)
	{
		return true;
	}
	send_queued_mail();
	return false;
}

PEOPLE_ID_NODE * EMailIP::CheckMail(bool bCheckAll)
{
	BEntry entry;
	BDirectory directory("/boot/home/mail/in/");
	BFile file;
	BPath path;
	PEOPLE_ID_NODE* pNode = NULL;
	int32 iResult = 0;
	m_pFirstID = NULL;

	check_for_mail(&iResult);
	if(!bCheckAll && iResult == 0)
	{
		return NULL;	
	}

	while( directory.GetNextEntry(&entry) == B_OK)
	{
		entry.GetPath(&path);
		file.SetTo(path.Path(), B_READ_ONLY);
		if(CheckIfTheMailIsValid(&file))
		{
			pNode = GetInfo(&file);
			file.Unset();
			if(pNode)
			{
				strcpy(pNode->pzFile, path.Path()); 
				AddNode(pNode);
			}
			else
			{
				entry.Remove();
			}
		}
		else
		{
			file.Unset();
		}
			
	}	
	return m_pFirstID;
}

void EMailIP::AddNode(PEOPLE_ID_NODE * pNode)
{
	PEOPLE_ID_NODE * pTemp = m_pFirstID;
	pNode->pNext = NULL;
	pNode->pPrev = NULL;
	if(pTemp == NULL)
	{
		m_pFirstID = pNode;
	}
	else
	{
		while(pTemp->pNext != NULL) pTemp = pTemp->pNext;
		pTemp->pNext = pNode;
		pNode->pPrev = pTemp;
	}
}

bool EMailIP::CheckIfTheMailIsValid(BFile * pFile)
{
	char pzBuffer[256];
	while( pFile->Read(pzBuffer, sizeof(char)) > 0)
	{
		if(pzBuffer[0] == 'S')
		{
			if(pFile->Read(pzBuffer, sizeof(char) * 8) == sizeof(char) * 8)
			{
				pzBuffer[8] = '\0';
				if(strcmp(pzBuffer, "ubject: ") == 0)
				{
					if(pFile->Read(pzBuffer, sizeof(char) * 16) == sizeof(char) * 16)
					{
						pzBuffer[16] = '\0';
						if(strcmp(pzBuffer, "BeNet IP Address") == 0)
						{
							return true;	
						}
					}
				}
			}
		}
	}
	return false;		
}

PEOPLE_ID_NODE* EMailIP::GetInfo(BFile * pFile)
{
	char pzBuffer[64000];
	char pzBufferOut[32000];
	BFile filePicture;
	bool bFind = false;
	ssize_t size;
	
	PEOPLE_ID_NODE * pNode = new PEOPLE_ID_NODE;
	
	
	//Trouver la date
	
	if(FindString(pFile, "Date: "))
	{
		GetLine(pFile, pzBuffer);
		strcpy(pNode->pzDate, pzBuffer);
	}

	
	//Trouver le email.
	if(FindString(pFile, "From: "))
	{
		GetLine(pFile, pzBuffer);
		strcpy(pNode->pzEMail, pzBuffer);
	}

	bFind = false;
	//Trouver le IP.
	if(FindString(pFile, "<IP>"))
	{
		GetLine(pFile, pzBuffer);
		strcpy(pNode->pzIP, pzBuffer);
	}
	
	
	if(FindString(pFile, "<NickName>"))
	{
		GetLine(pFile, pzBuffer);
		strcpy(pNode->pzName, pzBuffer);
	}
	
	if(FindString(pFile, "<Delay>"))
	{
		GetLine(pFile, pzBuffer);
					
		uint32 iExpiredTime = atoi(pzBuffer);
					
		if(real_time_clock() <= iExpiredTime)
		{
		}
		else
		{
			delete pNode;
			return NULL;
		}
	}
	
	if(FindString(pFile, "<Message>"))
	{
		off_t begin = pFile->Position();
		off_t end;
		FindString(pFile, "<EndMessage>");
		end = pFile->Position() - 12;
		pFile->Seek(begin, SEEK_SET);
		pFile->Read(pzBuffer, end - begin);
		pzBuffer[end - begin] = '\0';
		strcpy(pNode->pzMessage, pzBuffer);
		
	}
	
	
	//Trouver le nom image
	if(FindString(pFile, "Encoding: base64"))
	{
		pFile->Read(pzBuffer, 2);	//Lire les deux endline.
		
		strcpy(pNode->pzPicture, "/boot/home/");
		strcat(pNode->pzPicture, pNode->pzName);
		strcat(pNode->pzPicture, ".jpg");
		filePicture.SetTo(pNode->pzPicture, B_WRITE_ONLY | B_CREATE_FILE);
		
		size = pFile->Read(pzBuffer, 64000);
		
		size = decode_base64(pzBufferOut, pzBuffer, size, false);
		filePicture.Write(pzBufferOut, size);

		filePicture.Unset();		
	}
	return pNode;
}

bool EMailIP::GetLine(BFile * pFile, char * pzBuffer)
{
	int i = 0;
	char c = 0;
	bool bEndLine = false;
	
	while(pFile->Read(&c, sizeof(char)) > 0 && i < 256 && !bEndLine)
	{
		if(c == '\n' || c == '\r')
		{
			bEndLine =  true;
		}
		else
		{
			pzBuffer[i++] = c;
		}
	}
	pzBuffer[i] = '\0';

	if(i == 0) return false;
	
	return true;
}

bool EMailIP::FindString(BFile * pFile, char * pzString)
{
	char pzBuffer[256];
	bool bFind = false;
	int i = 0;
	char c;
	off_t offset = 0;
	
	pFile->Seek(0, SEEK_SET);
	
	while( pFile->Read(&c, sizeof(char)) > 0 && !bFind)
	{
		if( c == pzString[i])
		{
			pzBuffer[i++] = c;
			pzBuffer[i] = '\0';
			if(strcmp(pzString, pzBuffer) == 0)
			{
				offset = pFile->Position();
				bFind = true;
			}
		}
		else
		{
			i = 0;
		}
	}
	pFile->Seek(offset, SEEK_SET);
	
	return bFind;
}
