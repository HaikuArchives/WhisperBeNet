#ifndef _BeNetMenu_h
#include "BeNetMenu.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#ifdef ZETA
#include <interface/IconMenu.h>
#endif

#include <locale/Locale.h>

BeNetMenu::BeNetMenu(BRect frame)
	:	BMenuBar(frame, "Menu")
{
	//"File"
	m_pFile = new BMenu(_T("File"));
	m_pFile->SetTargetForItems(this);
	m_pFile->AddItem(m_pAbout = new BMenuItem(_T("About Whisper"), new BMessage(MSG_MENU_ABOUT), 'A'));
	m_pFile->AddSeparatorItem();
	m_pFile->AddItem(m_pHelp = new BMenuItem(_T("Help"), new BMessage(MSG_MENU_HELP), 'H'));
	m_pFile->AddItem(m_pPreferences = new BMenuItem(_T("Preferences"), new BMessage(MSG_MENU_PREFERENCES), 'P'));
	//OliverESP: I think here preferences is better
	m_pFile->AddSeparatorItem();
	m_pFile->AddItem(m_pExit = new BMenuItem(_T("Exit"), new BMessage(MSG_MENU_EXIT), 'X'));
		
#ifdef ZETA
	BIconMenu* app_menu = new BIconMenu(m_pFile);		// Iconified menu app
	AddItem(app_menu);
#else
	AddItem(m_pFile);									// Standard  menu app
#endif


	
	//"Edit"
	m_pEdit = new BMenu(_T("Edit"));
	m_pEdit->SetTargetForItems(this);
	m_pEdit->AddItem(m_pBookmarks = new BMenuItem(_T("Contacts"), new BMessage(MSG_MENU_BOOKMARKS), 'B'));
	AddItem(m_pEdit);

	//"View"
	m_pView = new BMenu(_T("View"));
	m_pView->SetTargetForItems(this);
	m_pView->AddItem(m_pToolbar = new BMenuItem(_T("Toolbar"), new BMessage(MSG_MENU_TOOLBAR), 'T'));
	m_pView->AddItem(m_pToolbar = new BMenuItem(_T("Status bar"), new BMessage(MSG_MENU_STATUSBAR), 'S'));
	m_pView->AddItem(m_pOutputWindow = new BMenuItem(_T("Output window"), new BMessage(MSG_MENU_OUTPUTWINDOW), 'O'));
	AddItem(m_pView);

	//"Tools"
	m_pTools = new BMenu(_T("Actions"));
	m_pTools->SetTargetForItems(this);
	m_pTools->AddItem(m_pConnect = new BMenuItem(_T("Connect"), new BMessage(MSG_MENU_CONNECT), 'C'));
	m_pTools->AddItem(m_pDisconnect = new BMenuItem(_T("Disconnect"), new BMessage(MSG_MENU_DISCONNECT), 'D'));
	m_pTools->AddItem(m_pCheckMail = new BMenuItem(_T("Check for connection"), new BMessage(MSG_MENU_MAILIP), 'K'));
	m_pCheckMail->SetEnabled(false); // OliverESP: sorry not revised 
	m_pTools->AddItem(m_pGoOnline = new BMenuItem(_T("Go online"), new BMessage(MSG_MENU_ONLINE), 'L'));
	m_pTools->AddItem(m_pGoOffline = new BMenuItem(_T("Go offline"), new BMessage(MSG_MENU_OFFLINE), 'F'));
	AddItem(m_pTools);

}
