#ifndef _Preferences_h
#define _Preferences_h

// for the preferences file
#include <FindDirectory.h>
#include <Directory.h>
#include <Path.h>
#include <File.h>		

#include <string.h>


class Preferences
{
public:
	static Preferences*	Instance();
	~Preferences();

	void				Load();
	void				Save();

	// UI
	void				ShowOscilloscope(bool state)		{m_showOscilloscope = state;}
	bool				ShowOscilloscope()					{return m_showOscilloscope;}
	void				ShowSoundControls(bool state)		{m_showSoundControls = state;}
	bool				ShowSoundControls()					{return m_showSoundControls;}
	void				ShowConnection(bool state)			{m_showConnection = state;}
	bool				ShowConnection()					{return m_showConnection;}
	void				ShowBookmarks(bool state)			{m_showBookmarks = state;}
	bool				ShowBookmarks()						{return m_showBookmarks;}
	void				ShowOutput(bool state)				{m_showOutput = state;}
	bool				ShowOutput()						{return m_showOutput;}

	// Audio
	void				RecordVolume(int32 volume)			{m_recordVolume = volume;}
	int32				RecordVolume()						{return m_recordVolume;}
	void				PlayVolume(int32 volume)			{m_playVolume = volume;}
	int32				PlayVolume()						{return m_playVolume;}
	void				UseModemQuality(bool state)			{m_audioUseModemQuality = state;}
	bool				UseModemQuality()					{return m_audioUseModemQuality;}
	void				UseCodec(bool state)				{m_audioUseCodec = state;}
	bool				UseCodec()							{return m_audioUseCodec;}

	void				Compression(int32 comp)				{m_compression = comp;}
	int32				Compression()						{return m_compression;}

	// Network
	void				OnlineOnStartup(bool online)		{m_OnlineOnStartup = online;}
	bool				OnlineOnStartup()					{return m_OnlineOnStartup;}
	void				ConnectionTimeout(int timeout)		{m_connectionTimeout = timeout;}
	bool				ConnectionTimeout()					{return m_connectionTimeout;}
	void				ServerIP(char* ip)					{strcpy(m_serverIP, ip); strcat(m_serverIP, "\0");}
	char*				ServerIP()							{return m_serverIP;}
	void				ServerPort(int port)				{m_serverPort = port;}
	int					ServerPort()						{return m_serverPort;}

	// Windows coordinates
	void				MainX(float x)						{m_mainX = x;}
	float				MainX()								{return m_mainX;}
	void				MainY(float y)						{m_mainY = y;}
	float				MainY()								{return m_mainY;}
	void				BookmarksX(float x)					{m_bookmarksX = x;}
	float				BookmarksX()						{return m_bookmarksX;}
	void				BookmarksY(float y)					{m_bookmarksY = y;}
	float				BookmarksY()						{return m_bookmarksY;}
	void				BookmarksW(float w)					{m_bookmarksW = w;}
	float				BookmarksW()						{return m_bookmarksW;}
	void				BookmarksH(float h)					{m_bookmarksH = h;}
	float				BookmarksH()						{return m_bookmarksH;}
	void				PreferencesX(float x)				{m_preferencesX = x;}
	float				PreferencesX()						{return m_preferencesX;}
	void				PreferencesY(float y)				{m_preferencesY = y;}
	float				PreferencesY()						{return m_preferencesY;}
	void				MailX(float x)						{m_mailX = x;}
	float				MailX()								{return m_mailX;}
	void				MailY(float y)						{m_mailY = y;}
	float				MailY()								{return m_mailY;}
	void				RemoteUserX(float x)				{m_remoteUserX = x;}
	float				RemoteUserX()						{return m_remoteUserX;}
	void				RemoteUserY(float y)				{m_remoteUserY = y;}
	float				RemoteUserY()						{return m_remoteUserY;}
	void				OutputX(float x)					{m_outputX = x;}
	float				OutputX()							{return m_outputX;}
	void				OutputY(float y)					{m_outputY = y;}
	float				OutputY()							{return m_outputY;}

	// UserInfo
	void				FirstName(char* name)				{strcpy(m_firstName, name); strcat(m_firstName, "\0");}
	char*				FirstName()							{return m_firstName;}
	void				LastName(char* name)				{strcpy(m_lastName, name); strcat(m_lastName, "\0");}
	char*				LastName()							{return m_lastName;}
	void				Email(char* email)					{strcpy(m_email, email); strcat(m_email, "\0");}
	char*				Email()								{return m_email;}


private: // functions
	Preferences(); // singleton implementation
	
private: // data
	static Preferences* m_instance;

	BFile				m_preferencesFile;
	BDirectory			m_directory;
	BPath				m_path;

	// UI
	bool				m_showOscilloscope;
	bool				m_showSoundControls;
	bool				m_showConnection;
	bool				m_showBookmarks;
	bool				m_showOutput;

	// Audio
	bool				m_audioUseModemQuality;
	bool				m_audioUseCodec;
	int32				m_recordVolume;
	int32				m_playVolume;

	int32				m_compression; // OliverESP: let´s play


	// Network
	bool				m_OnlineOnStartup;
	int					m_connectionTimeout;
	char				m_serverIP[16];
	int					m_serverPort;

	// Windows coordinates
	float				m_mainX;
	float				m_mainY;
	float				m_bookmarksX;
	float				m_bookmarksY;
	float				m_bookmarksW;
	float				m_bookmarksH;
	float				m_preferencesX;
	float				m_preferencesY;
	float				m_mailX;
	float				m_mailY;
	float				m_remoteUserX;
	float				m_remoteUserY;
	float				m_outputX;
	float				m_outputY;

	// UserInfo
	char				m_firstName[64];
	char				m_lastName[64];
	char				m_email[128];
};

#endif // _Preferences_h
