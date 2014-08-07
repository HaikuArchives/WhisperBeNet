#ifndef _ConnectView_h
#define _ConnectView_h

#ifndef __SHRINK_VIEW__
#include "ShrinkView.h"
#endif

#ifndef _BookmarksShortcut_h_
 #include "BookmarksShortcut.h"
#endif

#include <stdio.h>
#include <TextControl.h>
#include <Button.h>
#include <PopUpMenu.h>
#include <Menu.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <Box.h>
#include <RadioButton.h>


class ConnectView: public ShrinkView
{
public:
	ConnectView(BRect, bool bShrink = false);
	~ConnectView();
		
	virtual void	AttachedToWindow();
	virtual void	MessageReceived(BMessage *);
	void 			BuildShortcutMenu();
	void 			BuildProtocolMenu();  // OliverESP
	status_t		Connect();
	char*			GetIP()							{return (char*)m_pIP->Text();}
	char*			GetEmail()						{return (char*)m_pEmail->Text();}
	char*			GetFirstName()					{return (char*)m_pFirstName->Text();}
	char*			GetLastName()					{return (char*)m_pLastName->Text();}
	BookmarkItem*	GetConnectionInfo() 			{return m_pBookmarkItem;}
	bool			IsServerNeeded()				{return (bool)m_pRadioPersonal->Value();}
	void			Online();
	void			Offline();

//  OliverESP togle and untoolge the view
	void 		    SetEnabled(bool);
	void 			Field_error(const char* msg);
private:
	BButton*			m_pConnect;
	BTextControl*		m_pLastName;
	BTextControl*		m_pFirstName;
	BTextControl*		m_pEmail;
	BTextControl*		m_pIP;
	BookmarksShortcut*	m_pBookmarksShortcut;
	BookmarkItem*		m_pBookmarkItem;
	BMenuField*			m_pShortcutField;
	BBox*				m_pBoxPersonal;
	BBox*				m_pBoxIP;
	BRadioButton*		m_pRadioIP;
	BRadioButton*		m_pRadioPersonal;
	
	BPopUpMenu*			m_pProtocols;
	bool				online; //oliverESP
};

#endif // _ConnectView_h
