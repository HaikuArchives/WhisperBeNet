#ifndef _BeNetWindow_h
#define _BeNetWindow_h

#include <Window.h>
#include <Application.h>
#include <string.h>
#include <FindDirectory.h>
#include <Directory.h>
#include <Path.h>

#ifndef _PreferencesWindow_h
#include "PreferencesWindow.h"
#endif

#ifndef _BookmarksWindow_h
#include "BookmarksWindow.h"
#endif

#ifndef _About_h
#include "About.h"
#endif

#ifndef __BENET_MAIL_IP_WINDOWS__
#include "BeNetMailIPWindow.h"
#endif

#ifndef __RemoteWindow__
#include "RemoteWindow.h"
#endif

#ifndef _StatusBar_h
#include "StatusBar.h"
#endif

#ifndef _BeNetMenu_h
#include "BeNetMenu.h"
#endif

#ifndef _ScopeView_h
#include "ScopeView.h"
#endif

#ifndef _SoundControlsView_h
#include "SoundControlsView.h"
#endif

#ifndef _ConnectView_h
#include "ConnectView.h"
#endif

#ifndef TOOLBAR_VIEW
#include "ToolBarView.h"
#endif

#ifndef _Recorder2_h
#include "Recorder2.h"
#endif

#include <Point.h>

class BeNetWindow : public BWindow
{
public: 
	BeNetWindow();
	~BeNetWindow();
	virtual bool	QuitRequested();
	virtual void 	MessageReceived(BMessage* msg);
	virtual void	FrameMoved(BPoint point);
	bool			IsConnected()				{return m_isConnected;}

//private: // Functions
	void 			ShowWindow(BWindow* pWindow);
	void			ShowMailIPWindow();
	void			ToggleToolBar();
	void			ToggleStatusBar();
	void			LaunchHelp();

	void			GoOnline();
	void			GoOffline();
	void			Connect();
	void			Accept();
	void			Disconnect();
	void			Terminate();

	void            ToggleConection(bool tog); //OliverESP
	void 			EnableActions(bool tog);//OliverESP

private: // Data
	RemoteWindow*		m_pRemoteWindow;
	BookmarksWindow*	m_pBookmarksWindow;
	PreferencesWindow*	m_pPreferencesWindow;
	AboutWindow*		m_pAboutWindow;
	BeNetMailIPWindow*	m_pMailIPWindow;

	BeNetMenu*			m_pMenu;
	ToolBarView*		m_pToolBar;
	ConnectView*		m_pConnectView;
	ScopeView*			m_pScopeView;
	SoundControlsView*	m_pSoundControlsView;
	StatusBar*			m_pStatusBar;

	Recorder*			m_pRecorder;
	BPath				m_dirPath;
	BPath				m_prefsPath;
	BPath				m_bookmarksPath;
	
	bool				m_bShowStatusBar;
	bool				m_bShowToolBar;
	bool				m_isConnected;
};

#endif	// _BeNetWindow_h
