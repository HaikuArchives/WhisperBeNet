#ifndef _Preferences_h
#include "Preferences.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#include <Alert.h>

#include <stdio.h>


// singleton instanciation
Preferences* Preferences::m_instance = 0;

Preferences::Preferences() :
	m_showOscilloscope(true),
	m_showSoundControls(true),
	m_showConnection(true),
	m_showBookmarks(false),
	m_showOutput(false),
	m_audioUseModemQuality(false),
	m_audioUseCodec(false),
	m_recordVolume(80),
	m_playVolume(80),
	m_compression(4), // OliverESP
	m_OnlineOnStartup(false),
	m_connectionTimeout(1),
	m_serverPort(3030),
	m_mainX(100),
	m_mainY(100),
	m_bookmarksX(300),
	m_bookmarksY(100),
	m_bookmarksW(460),
	m_bookmarksH(200),
	m_preferencesX(300),
	m_preferencesY(300),
	m_mailX(100),
	m_mailY(100),
	m_remoteUserX(400),
	m_remoteUserY(100),
	m_outputX(100),
	m_outputY(100)
{
	strcpy(m_serverIP, "server");
	strcpy(m_firstName, "name");
	strcpy(m_lastName, "surname");
	strcpy(m_email, "mail");

	find_directory(B_USER_SETTINGS_DIRECTORY, &m_path);
   	m_path.Append(PREFERENCES_FOLDER);
   	m_directory.SetTo(m_path.Path());
}

Preferences::~Preferences()
{

}

Preferences* Preferences::Instance()
{
	if( m_instance == 0 )
		m_instance = new Preferences;

	return m_instance;
}


void
Preferences::Load()
{
	if (m_preferencesFile.SetTo(&m_directory, PREFERENCES_FILE, B_READ_WRITE | B_FAIL_IF_EXISTS) == B_OK)
	{
		int i = 0; // for string length

		// reset the file pointer
		m_preferencesFile.Seek(0, SEEK_SET);

		// show module on startup ---------------------------------------------
		if (m_preferencesFile.Read(&m_showOscilloscope, sizeof(bool)) < 0)
			printf("error read m_shrinkOscilloscope\n");
		if (m_preferencesFile.Read(&m_showSoundControls, sizeof(bool)) < 0)
			printf("error read m_shrinkSoundControls\n");
		if (m_preferencesFile.Read(&m_showConnection, sizeof(bool)) < 0)
			printf("error read m_shrinkConnection\n");
		if (m_preferencesFile.Read(&m_showBookmarks, sizeof(bool)) < 0)
			printf("error read m_bookmarksWindow\n");
		if (m_preferencesFile.Read(&m_showOutput, sizeof(bool)) < 0)
			printf("error read m_showOutput\n");

		// Audio --------------------------------------------------------------
		if (m_preferencesFile.Read(&m_recordVolume, sizeof(int32)) < 0)
			printf("error read m_recordVolume\n");
		if (m_preferencesFile.Read(&m_playVolume, sizeof(int32)) < 0)
			printf("error read m_playVolume\n");
		if (m_preferencesFile.Read(&m_audioUseModemQuality, sizeof(bool)) < 0)
			printf("error read m_useModemQuality\n");
		if (m_preferencesFile.Read(&m_audioUseCodec, sizeof(bool)) < 0)
			printf("error read m_useCodec\n");

		if (m_preferencesFile.Read(&m_compression, sizeof(int32)) < 0)
			printf("error read m_compression\n");


		// Network ------------------------------------------------------------
		if (m_preferencesFile.Read(&m_OnlineOnStartup, sizeof(bool)) < 0)
			printf("error read m_OnlineOnStartup\n");
		if (m_preferencesFile.Read(&m_connectionTimeout, sizeof(int)) < 0)
			printf("error read m_connectionTimeout\n");
		if (m_preferencesFile.Read(&i, sizeof(int)) < 0)
			printf("error read m_serverIP length\n");
		if (m_preferencesFile.Read(m_serverIP, i + 1) < 0)
			printf("error read m_serverIP\n");
		if (m_preferencesFile.Read(&m_serverPort, sizeof(int)) < 0)
			printf("error read m_serverPort\n");

		// Windows coordinates ------------------------------------------------
		if (m_preferencesFile.Read(&m_mainX, sizeof(float)) < 0)
			printf("error read m_mainX\n");
		if (m_preferencesFile.Read(&m_mainY, sizeof(float)) < 0)
			printf("error read m_mainY\n");
		if (m_preferencesFile.Read(&m_bookmarksX, sizeof(float)) < 0)
			printf("error read m_bookmarksX\n");
		if (m_preferencesFile.Read(&m_bookmarksY, sizeof(float)) < 0)
			printf("error read m_bookmarksY\n");
		if (m_preferencesFile.Read(&m_bookmarksW, sizeof(float)) < 0)
			printf("error read m_bookmarksW\n");
		if (m_preferencesFile.Read(&m_bookmarksH, sizeof(float)) < 0)
			printf("error read m_bookmarksH\n");
		if (m_preferencesFile.Read(&m_preferencesX, sizeof(float)) < 0)
			printf("error read m_preferencesX\n");
		if (m_preferencesFile.Read(&m_preferencesY, sizeof(float)) < 0)
			printf("error read m_preferencesY\n");
		if (m_preferencesFile.Read(&m_mailX, sizeof(float)) < 0)
			printf("error read m_mailX\n");
		if (m_preferencesFile.Read(&m_mailY, sizeof(float)) < 0)
			printf("error read m_mailY\n");
		if (m_preferencesFile.Read(&m_remoteUserX, sizeof(float)) < 0)
			printf("error read m_remoteUserX\n");
		if (m_preferencesFile.Read(&m_remoteUserY, sizeof(float)) < 0)
			printf("error read m_remoteUserY\n");
		if (m_preferencesFile.Read(&m_outputX, sizeof(float)) < 0)
			printf("error read m_outputX\n");
		if (m_preferencesFile.Read(&m_outputY, sizeof(float)) < 0)
			printf("error read m_outputY\n");
			
		// User info ----------------------------------------------------------
		if (m_preferencesFile.Read(&i, sizeof(int)) < 0)
			printf("error read m_firstName length\n");
		if (m_preferencesFile.Read(m_firstName, i + 1) < 0)
			printf("error read m_firstName\n");
		if (m_preferencesFile.Read(&i, sizeof(int)) < 0)
			printf("error read m_lastName length\n");
		if (m_preferencesFile.Read(m_lastName, i + 1) < 0)
			printf("error read m_lastName\n");
		if (m_preferencesFile.Read(&i, sizeof(int)) < 0)
			printf("error read m_email length\n");
		if (m_preferencesFile.Read(m_email, i + 1) < 0)
			printf("error read m_email\n");
	}
}


void
Preferences::Save()
{
	if (m_preferencesFile.SetTo(&m_directory, PREFERENCES_FILE, B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE) == B_OK)
	{
		int i = 0; // for string length
		
		// show module on startup ---------------------------------------------
		if (m_preferencesFile.Write(&m_showOscilloscope, sizeof(bool)) < 0)
			printf("error write m_showOscilloscope\n");
		if (m_preferencesFile.Write(&m_showSoundControls, sizeof(bool)) < 0)
			printf("error write m_showSoundControls\n");
		if (m_preferencesFile.Write(&m_showConnection, sizeof(bool)) < 0)
			printf("error write m_showSoundControls\n");
		if (m_preferencesFile.Write(&m_showBookmarks, sizeof(bool)) < 0)
			printf("error write m_showBookmarks\n");
		if (m_preferencesFile.Write(&m_showOutput, sizeof(bool)) < 0)
			printf("error write m_showOutput\n");

		// Audio --------------------------------------------------------------
		if (m_preferencesFile.Write(&m_recordVolume, sizeof(int32)) < 0)
			printf("error write m_recordVolume\n");
		if (m_preferencesFile.Write(&m_playVolume, sizeof(int32)) < 0)
			printf("error write m_playVolume\n");
		if (m_preferencesFile.Write(&m_audioUseModemQuality, sizeof(bool)) < 0)
			printf("error write m_audioUseModemQuality\n");
		if (m_preferencesFile.Write(&m_audioUseCodec, sizeof(bool)) < 0)
			printf("error write m_audioUseCodec\n");

		if (m_preferencesFile.Write(&m_compression, sizeof(int32)) < 0)
			printf("error write m_compression\n");


		// Network ------------------------------------------------------------
		if (m_preferencesFile.Write(&m_OnlineOnStartup, sizeof(bool)) < 0)
			printf("error write m_OnlineOnStartup\n");
		if (m_preferencesFile.Write(&m_connectionTimeout, sizeof(int)) < 0)
			printf("error write m_connectionTimeout\n");
		i = strlen(m_serverIP);
 		if (m_preferencesFile.Write(&i, sizeof(int)) < 0)
			printf("error write m_serverIP length\n");
		if (m_preferencesFile.Write(m_serverIP, i + 1) < 0)
			printf("error write m_serverIP\n");
		if (m_preferencesFile.Write(&m_serverPort, sizeof(int)) < 0)
			printf("error write m_serverPort\n");

		// Windows coordinates ------------------------------------------------
		if (m_preferencesFile.Write(&m_mainX, sizeof(float)) < 0)
			printf("error write m_mainX\n");
		if (m_preferencesFile.Write(&m_mainY, sizeof(float)) < 0)
			printf("error write m_mainY\n");
		if (m_preferencesFile.Write(&m_bookmarksX, sizeof(float)) < 0)
			printf("error write m_bookmarksX\n");
		if (m_preferencesFile.Write(&m_bookmarksY, sizeof(float)) < 0)
			printf("error write m_bookmarksY\n");
		if (m_preferencesFile.Write(&m_bookmarksW, sizeof(float)) < 0)
			printf("error write m_bookmarksW\n");
		if (m_preferencesFile.Write(&m_bookmarksH, sizeof(float)) < 0)
			printf("error write m_bookmarksH\n");
		if (m_preferencesFile.Write(&m_preferencesX, sizeof(float)) < 0)
			printf("error write m_preferencesX\n");
		if (m_preferencesFile.Write(&m_preferencesY, sizeof(float)) < 0)
			printf("error write m_preferencesY\n");
		if (m_preferencesFile.Write(&m_mailX, sizeof(float)) < 0)
			printf("error write m_mailX\n");
		if (m_preferencesFile.Write(&m_mailY, sizeof(float)) < 0)
			printf("error write m_mailY\n");
		if (m_preferencesFile.Write(&m_remoteUserX, sizeof(float)) < 0)
			printf("error write m_remoteUserX\n");
		if (m_preferencesFile.Write(&m_remoteUserY, sizeof(float)) < 0)
			printf("error write m_remoteUserY\n");
		if (m_preferencesFile.Write(&m_outputX, sizeof(float)) < 0)
			printf("error write m_outputX\n");
		if (m_preferencesFile.Write(&m_outputY, sizeof(float)) < 0)
			printf("error write m_outputY\n");

		// User info ----------------------------------------------------------
		i = strlen(m_firstName);
 		if (m_preferencesFile.Write(&i, sizeof(int)) < 0)
			printf("error write m_firstName length\n");
		if (m_preferencesFile.Write(m_firstName, i + 1) < 0)
			printf("error write m_firstName\n");
		i = strlen(m_lastName);
 		if (m_preferencesFile.Write(&i, sizeof(int)) < 0)
			printf("error write m_lastName length\n");
		if (m_preferencesFile.Write(m_lastName, i + 1) < 0)
			printf("error write m_lastName\n");
		i = strlen(m_email);
 		if (m_preferencesFile.Write(&i, sizeof(int)) < 0)
			printf("error write m_email length\n");
		if (m_preferencesFile.Write(m_email, i + 1) < 0)
			printf("error write m_email\n");
	}
}
