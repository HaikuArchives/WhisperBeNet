#ifndef _BeNetMenu_h
#define _BeNetMenu_h

#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>


class BeNetMenu : public BMenuBar
{
public:
	BeNetMenu(BRect frame);

private:
	BMenu*		m_pFile;
	BMenuItem*	m_pAbout;
	BMenuItem*	m_pHelp;
	BMenuItem*	m_pExit;

	BMenu*		m_pEdit;
	BMenuItem*	m_pBookmarks;
	BMenuItem*	m_pPreferences;

	BMenu*		m_pView;
	BMenuItem*	m_pToolbar;
	BMenuItem*	m_pStatusbar;
	BMenuItem*	m_pOutputWindow;

	BMenu*		m_pTools;
	BMenuItem*	m_pConnect;
	BMenuItem*	m_pDisconnect;
	BMenuItem*	m_pCheckMail;
	BMenuItem*	m_pGoOnline;
	BMenuItem*	m_pGoOffline;
};


#endif	// _BeNetMenu_h
