#ifndef _BeNetWindow_h
#include "BeNetWindow.h"
#endif

#ifndef Preferences_h
#include "Preferences.h"
#endif

#ifndef _EMAILIP_
#include "EMailIP.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#ifndef _Network
#include "network.h"
#endif

#ifndef _CLIENT_H
#include "client.h"
#endif

#ifndef _Output_h
#include "Output.h"
#endif

#include <interface/Alert.h>
//#include <storage/Entry.h> // OliverESP: not needed thanks IRA
#include <app/Roster.h>

#include <locale/Locale.h>

BeNetWindow::BeNetWindow() :
	BWindow(BRect(0,0,230,209),_T("Whisper"), B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE),
	m_pBookmarksWindow(NULL),
	m_pPreferencesWindow(NULL),
	m_bShowStatusBar(true),
	m_bShowToolBar(true),
	m_isConnected(false)
{
	// For connection to server
	TClient::Instance();

	// For connection to user
	CNetwork::Instance()->SetWindow(this);

	
	// Load the preferences
	Preferences::Instance()->Load();

	
	BRect b = Bounds();

	AddChild(m_pMenu = new BeNetMenu(BRect(b.left, b.top, b.right, b.top + 12 )));
	b = m_pMenu->Frame();

	AddChild(m_pToolBar = new ToolBarView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 40)));
	b = m_pToolBar->Frame();

	AddChild(m_pConnectView = new ConnectView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 230), !Preferences::Instance()->ShowConnection()));
	b = m_pConnectView->Frame();

	// OliverESP: b.bottom + 97 <- ok
	AddChild(m_pScopeView = new ScopeView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 97), !Preferences::Instance()->ShowOscilloscope()));
	b = m_pScopeView->Frame();

	AddChild(m_pSoundControlsView = new SoundControlsView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 51), !Preferences::Instance()->ShowSoundControls()));
	b = m_pSoundControlsView->Frame();
	m_pSoundControlsView->Volume(Preferences::Instance()->RecordVolume());

	AddChild(m_pStatusBar = new StatusBar(BRect(b.left, b.bottom + 1, b.right, b.bottom + 19)));
	b = m_pStatusBar->Frame();

	ResizeTo(b.right, b.bottom - 1);
	MoveTo(	Preferences::Instance()->MainX(),
			Preferences::Instance()->MainY());

	// Initialize the recorder
	m_pRecorder = new Recorder(m_pScopeView);

	// Start recording	
//	m_pRecorder->Record(); // Whisper Old sound engine
	m_pRecorder->StartRecording();

	find_directory(B_USER_SETTINGS_DIRECTORY, &m_dirPath);
   	m_dirPath.Append(PREFERENCES_FOLDER);
   	create_directory(m_dirPath.Path(), 0777);
   	
   	m_prefsPath.Append(m_dirPath.Path());
   	m_bookmarksPath.Append(m_dirPath.Path());


	m_pRemoteWindow = new RemoteWindow(this);
	m_pRemoteWindow->Run();

	m_pBookmarksWindow = new BookmarksWindow(m_pConnectView);
	m_pBookmarksWindow->Run();

	m_pPreferencesWindow = new PreferencesWindow;
	m_pPreferencesWindow->Run();

	m_pAboutWindow = new AboutWindow;
	m_pAboutWindow->Run();

	Output::Instance()->Run();

	if (Preferences::Instance()->OnlineOnStartup())
		GoOnline();

	if (Preferences::Instance()->ShowBookmarks())
		ShowWindow(m_pBookmarksWindow);

	if (Preferences::Instance()->ShowOutput())
		ShowWindow(Output::Instance());
}


BeNetWindow::~BeNetWindow()
{
	m_pRemoteWindow->Lock();
	m_pRemoteWindow->Quit();

	m_pBookmarksWindow->Lock();
	m_pBookmarksWindow->Quit();

	m_pPreferencesWindow->Lock();
	m_pPreferencesWindow->Quit();

	m_pAboutWindow->Lock();
	m_pAboutWindow->Quit();

	Output::Instance()->Lock();
	Output::Instance()->Quit();

	delete Preferences::Instance();
	delete CNetwork::Instance();
}


void
BeNetWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case MSG_SOUNDCONTROLS_VOLUME:
			Preferences::Instance()->RecordVolume(m_pSoundControlsView->Volume());
			m_pRecorder->SetRecVolume(((float)Preferences::Instance()->RecordVolume()/(float)100));
			break;
		case MSG_SOUNDCONTROLS_MUTE:
			if(m_pSoundControlsView->IsMute())
				m_pRecorder->StopRecording();
			else
//				m_pRecorder->Record();
				m_pRecorder->StartRecording();
			break;
		case MSG_MENU_EXIT:				// QuitRequested(); OliverESP: This will make be called twice in Zeta hmmmmmmm
										be_app_messenger.SendMessage(B_QUIT_REQUESTED);
										break;
		case MSG_MENU_TOOLBAR:			ToggleToolBar(); break; 
		case MSG_MENU_STATUSBAR:		ToggleStatusBar(); break; 
		case MSG_MENU_ABOUT:			ShowWindow(m_pAboutWindow); break;
		case MSG_MENU_OUTPUTWINDOW:		ShowWindow(Output::Instance()); break; 
		case MSG_MENU_PREFERENCES:		ShowWindow(m_pPreferencesWindow); break; 
		case MSG_MENU_BOOKMARKS:		ShowWindow(m_pBookmarksWindow); break; 
		case MSG_MENU_MAILIP:			ShowMailIPWindow(); break; 
		case MSG_MENU_HELP:				LaunchHelp(); break; 
		case MENU_BOOKMARKS:			m_pConnectView->MessageReceived(msg); break;
		case MSG_MENU_CONNECT:			Connect(); break;
		case MSG_MENU_ACCEPT:			Accept(); break;
		case MSG_MENU_DISCONNECT:		Disconnect(); break; 
		case MSG_MENU_TERMINATE:		Terminate(); break; 
		case MSG_MENU_ONLINE:			GoOnline(); break; 
		case MSG_MENU_OFFLINE:			GoOffline(); break;
		
		case MSG_NOT_CONNECTED:			ToggleConection(false); // we receive this from the connectionView and this from the network 
										EnableActions(true);
										break; //OliverESP
		
		default:						BWindow::MessageReceived(msg); break;	 
	}
}


bool
BeNetWindow::QuitRequested(void)
{

	// Save the preferences
	Preferences::Instance()->Save();

	// Go offline
	if (TClient::Instance()->IsOnline())
		GoOffline();

	if (IsConnected())
		Disconnect();

	// Delete the recorder (pretend that it's recording)
	if (m_pRecorder != NULL) {        // OliverESP: we have to look out the pointer!
		m_pRecorder->StopRecording();
		delete m_pRecorder;
		m_pRecorder = NULL;
	}
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return true;
}


void
BeNetWindow::FrameMoved(BPoint point)
{
	Preferences::Instance()->MainX(point.x);
	Preferences::Instance()->MainY(point.y);
}


void
BeNetWindow::ShowWindow(BWindow* pWindow)
{
	pWindow->Lock();
	if (pWindow->IsHidden())
		pWindow->Show();
	else
		pWindow->Activate();
	pWindow->Unlock();
}


void
BeNetWindow::LaunchHelp()
{
	Output::Instance()->UI("BeNetWindow::LaunchHelp\n");
//	entry_ref ref;
	
// OliverESP: I though do something like this
// I saw code from SampleStudio and they make the same
// it doesn´t look pretty. All this just to know your current path

		BPath path;
		
		app_info ai;
		be_app->GetAppInfo(&ai);	
		BEntry entry(&ai.ref);
		
		entry.GetPath(&path);
		path.GetParent(&path);
		path.Append("help/gettingstarted.htm");
		
		char *help = new char[strlen(path.Path())+1];
		sprintf(help, path.Path());
		be_roster->Launch("text/html",1, &help);
		delete help;	

//	if (get_ref_for_path("help/help.htm", &ref) == B_OK)
//		be_roster->Launch(&ref);

}


void
BeNetWindow::ShowMailIPWindow()
{
	Output::Instance()->UI("BeNetWindow::ShowMailIPWindow\n");

	PEOPLE_ID_NODE* node = new PEOPLE_ID_NODE;
	
	strcpy(node->pzIP, "192.168.0.2");
	strcpy(node->pzName, "Mathieu Brault");
	strcpy(node->pzEMail, "bram01@gel.usherb.ca");
	strcpy(node->pzDate, "99-03-15");
	strcpy(node->pzPicture, "./UI/Bitmaps/Connect");
	strcpy(node->pzFile, "./Bitmaps/Connect");
	
	m_pMailIPWindow = new BeNetMailIPWindow(node);
	m_pMailIPWindow->Show();
}


void
BeNetWindow::ToggleToolBar()
{
	Output::Instance()->UI("BeNetWindow::ToggleToolBar\n");
	m_bShowToolBar = !m_bShowToolBar;

	Lock();
	m_bShowToolBar ? m_pToolBar->Show() : m_pToolBar->Hide();
	m_pConnectView->MoveBy(0,m_bShowToolBar ? 39 : -39);
	m_pScopeView->MoveBy(0,m_bShowToolBar ? 39 : -39);
	m_pSoundControlsView->MoveBy(0,m_bShowToolBar ? 39 : -39);
	m_pStatusBar->MoveBy(0,m_bShowToolBar ? 39 : -39);

	ResizeBy(0,m_bShowToolBar ? 39 : -39);
	Unlock();
}


void
BeNetWindow::ToggleStatusBar()
{
	Output::Instance()->UI("BeNetWindow::ToggleStatusBar\n");
	m_bShowStatusBar = !m_bShowStatusBar;

	Lock();
	m_bShowStatusBar ? m_pStatusBar->Show() : m_pStatusBar->Hide();
	ResizeBy(0,m_bShowStatusBar ? 18 : -18);
	Unlock();
}

void
BeNetWindow::GoOnline()
{
	Output::Instance()->UI("BeNetWindow::GoOnline\n");
	EnableActions(false);  //OliverESP


	if (TClient::Instance()->Register(	Preferences::Instance()->LastName(),
										Preferences::Instance()->FirstName(),
										Preferences::Instance()->Email()))
	{
		m_pStatusBar->Online();
		m_pToolBar->Online();
		m_pConnectView->Online();
	}
	else
	{
		Output::Instance()->Network("Error connecting to server");
		Output::Instance()->Network(Preferences::Instance()->ServerIP());
		Output::Instance()->Network("\n");
		m_pStatusBar->Offline();
		m_pToolBar->Offline();
		m_pConnectView->Offline();
	}
	EnableActions(true); //OliverESP
}


void
BeNetWindow::GoOffline()
{
	Output::Instance()->UI("BeNetWindow::GoOffline\n");
	TClient::Instance()->Deregister();
	m_pStatusBar->Offline();
	m_pToolBar->Offline();
	m_pConnectView->Offline();
}


void
BeNetWindow::Connect()
{
	Output::Instance()->UI("BeNetWindow::Connect\n");
	m_pConnectView->Connect();
	EnableActions(false); //OliverESP
}


void
BeNetWindow::Accept()
{
	Output::Instance()->UI("BeNetWindow::Accept\n");
	ShowWindow(m_pRemoteWindow);
	m_pStatusBar->Connected();
	m_pToolBar->Connect();
	m_isConnected = true;
}


void
BeNetWindow::Disconnect()
{
	Output::Instance()->UI("BeNetWindow::Disconnect\n");
	if (CNetwork::Instance()->Disconnect() == B_OK)
	{
		m_isConnected = false;
		m_pStatusBar->Disconnected();
		m_pToolBar->Disconnect();

		m_pRemoteWindow->Lock();
		m_pRemoteWindow->Hide();
		m_pRemoteWindow->Unlock();
	}
}


void
BeNetWindow::Terminate()
{
	Output::Instance()->UI("BeNetWindow::Terminate\n");
	m_isConnected = false;
	m_pStatusBar->Disconnected();
	m_pToolBar->Disconnect();

	m_pRemoteWindow->Lock();
	m_pRemoteWindow->Hide();
	m_pRemoteWindow->Unlock();
}
void //OliverESP this will set in the buttons
BeNetWindow::ToggleConection(bool tog)
{
	if (tog) {
		m_pStatusBar->Connected();
		m_pToolBar->Connect();	
	} else {
		m_pStatusBar->Disconnected();
		m_pToolBar->Disconnect();
	}
}
void //OliverESP this will dis/enable the buttons
BeNetWindow::EnableActions(bool tog)
{
		m_pStatusBar->SetEnable(tog);
		m_pToolBar->SetEnable(tog);	
}

