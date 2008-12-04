#ifndef _ConnectView_h
#include "ConnectView.h"
#endif

#ifndef _Output_h     // OliverESP
#include "Output.h"
#endif

#ifndef _BeNetMenu_h
#include "BeNetMenu.h"
#endif

#ifndef _BookmarkItem_h_
#include "BookmarkItem.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#ifndef _Network
#include "network.h"
#endif

#ifndef _WaitWindow_h
#include "WaitWindow.h"
#endif

#ifndef _CLIENT_H
#include "client.h"
#endif

#ifndef _BitmapCatalog_h
#include "BitmapCatalog.h"
#endif

#include <Message.h>
#include <Alert.h>
#include <Window.h>

#include <locale/Locale.h>
#include <string.h>
#include <OS.h>


ConnectView::ConnectView(BRect frame, bool bShrink) :
	ShrinkView(frame, (char *)_T("Connection"), bShrink/*, BitmapCatalog::Instance()->FetchBitmap("Bitmaps/ShrinkView7.tif", true)*/)
{
	SetViewColor(192,192,192);
}

ConnectView::~ConnectView()
{

}

void
ConnectView::AttachedToWindow()
{
	float w, h;
	m_pRadioPersonal = new BRadioButton(BRect(5, 20, 50, 40), "Personal_info",
	 												_T("Using Personal Information"),
	 												 new BMessage(MSG_CONNECTION_MODE),
	 												 B_FOLLOW_NONE, B_WILL_DRAW);
	m_pRadioPersonal->SetTarget(this);
	 												 
	m_pRadioIP = new BRadioButton(BRect(5, 180, 50, 200), "Ip_adress",
											  _T("Using IP Address"),
											  new BMessage(MSG_CONNECTION_MODE),
											  B_FOLLOW_NONE, B_WILL_DRAW);
	m_pRadioIP->SetTarget(this);
	
	m_pBoxPersonal = new BBox(BRect(5, 20, 225, 135), "BoxPersonal", B_FOLLOW_NONE, B_WILL_DRAW, B_FANCY_BORDER);
	m_pBoxIP = new BBox(BRect(5, 140, 225, 190), "BoxIP", B_FOLLOW_NONE, B_WILL_DRAW, B_FANCY_BORDER);

	m_pBoxPersonal->SetLabel(m_pRadioPersonal);
	m_pBoxIP->SetLabel(m_pRadioIP);

	m_pConnect = new BButton(BRect(110,30,225,215), "Connect", _T("Connect"), new BMessage(MSG_MENU_CONNECT));
	m_pConnect->GetPreferredSize( &w , &h);
	m_pConnect->ResizeToPreferred();
	m_pConnect->MoveTo(BPoint(Bounds().right - w - 10, Bounds().bottom - h - 10) );
	m_pConnect->MakeDefault(true); // OliverESP: then we can press intro after typing the IP
	
	
	m_pBoxPersonal->AddChild(m_pFirstName = new BTextControl(BRect(5, 20, 210, 35), "ConnectName", _T("Surname"), NULL, 0, B_FOLLOW_NONE, B_WILL_DRAW));
	m_pFirstName->SetDivider(60);

	m_pBoxPersonal->AddChild(m_pLastName = new BTextControl(BRect(5,40,210,55), "ConnectName", _T("Name"), NULL, 0, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE));
	m_pLastName->SetDivider(60);
	
	m_pBoxPersonal->AddChild(m_pEmail = new BTextControl(BRect(5, 60, 210, 75), "ConnectName", _T("Email"), NULL, 0, B_FOLLOW_NONE, B_WILL_DRAW));
	m_pEmail->SetDivider(60);
	
	m_pBoxIP->AddChild(m_pIP = new BTextControl(BRect(5,20,210,40), "ConnectIP", _T("IP / Host :"), NULL, 0, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE));
	m_pIP->SetDivider(60);

	AddChild(m_pBoxPersonal);
	AddChild(m_pBoxIP);
	AddChild(m_pConnect);

	BuildShortcutMenu();
//	BuildProtocolMenu();  //OliverESP

	m_pRadioPersonal->SetValue(0);
	m_pRadioIP->SetValue(1);
	
	m_pRadioPersonal->SetEnabled(false); // Yes! disabled till we dont have a connectionOliverESP
	online = false;
	
	m_pLastName->SetEnabled(false);
	m_pFirstName->SetEnabled(false);
	m_pEmail->SetEnabled(false);
	
	m_pIP->SetEnabled(true);
}


void
ConnectView::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
   		case MENU_BOOKMARKS:
   		{
			char *name = new char[256];
			char *surname = new char[256];
			
 			strcpy(name, (char*)message->FindString("Bookmark_name"));
			strcpy(surname, (char*)message->FindString("Bookmark_surname"));						
			
			
//			BMenuItem *item; 
//	        message->FindPointer("source",(void **)&item); 	
//			strcpy(v_pString, item->Label());						
			
			
			m_pBookmarkItem = Bookmarks::Instance()->FindBookmarkItem(name,surname);
			
			m_pLastName->SetText(m_pBookmarkItem->GetColumnContentText(0));
			m_pFirstName->SetText(m_pBookmarkItem->GetColumnContentText(1));
			m_pEmail->SetText(m_pBookmarkItem->GetColumnContentText(2));
   			
 			delete name;
 			delete surname;
   			break;
   		}
   		case MSG_MENU_SAVE:
   		{
   			// OliverESP: when a new bookmark is added we remake the shortcut-> BUG
   			m_pBoxPersonal->RemoveChild(m_pShortcutField); // this fixs it
   			
   			delete m_pBookmarksShortcut;
   			RemoveChild(m_pShortcutField);
   			BuildShortcutMenu();
   			break;
   		}
   		case MSG_CONNECTION_MODE:
			m_pFirstName->SetEnabled((bool)m_pRadioPersonal->Value());
			m_pLastName->SetEnabled((bool)m_pRadioPersonal->Value());
			m_pEmail->SetEnabled((bool)m_pRadioPersonal->Value());
			m_pShortcutField->SetEnabled((bool)m_pRadioPersonal->Value());
			m_pIP->SetEnabled((bool)m_pRadioIP->Value());
   			break;
		case MSG_CONNECTION_FINISH: 
			// OliverESP to toggle the connection view elements
			// also uptade the toolbar status bar and enable the controls
			// we receive this message from the Network
			// will inform the window to update the controls
		    BMessenger(this->Window()).SendMessage(new BMessage(MSG_NOT_CONNECTED)); 
			SetEnabled(true);		// OliverESP
		break;
   		default:
   			BView::MessageReceived(message);
			break;
	}
}


void
ConnectView::BuildShortcutMenu()
{
	m_pBookmarksShortcut = new BookmarksShortcut();
	m_pBookmarksShortcut->SetRadioMode(true);
	
	m_pBoxPersonal->AddChild(m_pShortcutField = new BMenuField(BRect(5, 85, 200, 90), "shortcutField", NULL, m_pBookmarksShortcut));
	m_pShortcutField->SetEnabled((bool)m_pRadioPersonal->Value());
}

void
ConnectView::BuildProtocolMenu()
{
	m_pProtocols = new BPopUpMenu(_T("Protocol"));
	m_pProtocols->AddItem(new BMenuItem("H323", NULL));
	m_pProtocols->AddItem(new BMenuItem("BeNet/RTP-RCTP", NULL));
	
	m_pProtocols->SetRadioMode(true);

	AddChild(new BMenuField(BRect(5, 195, 200, 215), "ProtocolsField", NULL, m_pProtocols));
}


status_t
ConnectView::Connect()
{
	if (IsServerNeeded())
	{
		// Demandons au serveur l'adresse de l'usager choisi
		// On demande en premier avec l'email : GetEmail().
		// Si ca ne fonctionne pas, on demande avec le nom: GetFirstName() & GetLastName().
		
		char* ip = TClient::Instance()->GetIPByEmail(GetEmail());
		if (!strcmp(ip, "null"))
		{
			ip = TClient::Instance()->GetIPByName(GetLastName(), GetFirstName());
			if (!strcmp(ip, "null"))
			{
				BAlert *myAlert = new BAlert(	"Error", "User not found (maybe not online).\n  Please try again later.",
												"Ok", NULL, NULL, 
												B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT); 
				myAlert->Go();

				return false;
			}
		}

/*		WaitWindow* waitWindow = new WaitWindow;
		waitWindow->Show();
		waitWindow->SetText(ip);*/
		Output::Instance()->Network("Attempting servered Connection\n");  // OliverESP
		CNetwork::Instance()->SetView(this);							  // OliverESP
		SetEnabled(false);												  // OliverESP
		//return CNetwork::Instance()->ConnectFromUI(inet_addr(ip));
		return CNetwork::Instance()->ConnectFromUI(GetIP());
	}
	else
	{		
		if (!strlen(GetIP())) {
			Field_error(_T("Please type some host or IP address"));
			BMessenger(this->Window()).SendMessage(new BMessage(MSG_NOT_CONNECTED));
			SetEnabled(true);
			return B_ERROR;
		}	
		
		WaitWindow* waitWindow = new WaitWindow;
		waitWindow->Show();
		waitWindow->SetText(GetIP());

		// On demande directement au reseau de nous connecter avec l'adresse dans GetIP().
    	// ulong RemoteAddress; OliverESP :not needed
		status_t err;
		
		Output::Instance()->Network("Attempting direct Connection\n");  // OliverESP
		CNetwork::Instance()->SetView(this);							// OliverESP inform to enable later
		SetEnabled(false);     											// OliverESP diasabling

	 // return CNetwork::Instance()->ConnectFromUI(inet_addr(GetIP())); // OliverESP error control

	  	err = CNetwork::Instance()->ConnectFromUI(GetIP());
	  		 
		switch (err){
			case EHOSTUNREACH:
				Field_error(_T("It doesn´t look a valid host address"));
				BMessenger(this->Window()).SendMessage(new BMessage(MSG_NOT_CONNECTED));
				SetEnabled(true);
				break;
			case B_ERROR:
				Field_error("Unable to comunicate sRTCP thread");
				BMessenger(this->Window()).SendMessage(new BMessage(MSG_NOT_CONNECTED));
				SetEnabled(true);
				break;
			case B_OK:
				Output::Instance()->Network("Awaiting response from remote...\n");
				//Field_error("Trying connection..."); <-- this is blocking
				//return err;
				break;
			default:
				Field_error("Unable spawn threads!");
				BMessenger(this->Window()).SendMessage(new BMessage(MSG_NOT_CONNECTED));
				SetEnabled(true);
				break;			
		}

//		waitWindow->QuitRequested(); //OliverESP: this method is provided no way to use it
		waitWindow->Lock();
		waitWindow->Quit();          //OliverESP: if we dont Quit() it, it will acumulate with anothers
	    return err;		
	}
	
	return true; // temporaire
}


void
ConnectView::Online()
{
	m_pRadioPersonal->SetEnabled(true);
	online = true; //oliverESP, information about on/off lined
}


void
ConnectView::Offline()
{
	online = false;
	
	m_pRadioPersonal->SetEnabled(false);
	
	m_pRadioPersonal->SetValue(0);
	m_pRadioIP->SetValue(1);

	MessageReceived(new BMessage(MSG_CONNECTION_MODE));
}

// OliverESP better with this, not good being able to change the IP while connecting
void
ConnectView::SetEnabled(bool flag)
{
	m_pRadioPersonal->SetEnabled(online); //just if we are online
	m_pRadioIP->SetEnabled(flag);

	m_pRadioPersonal->SetValue(flag);
	m_pRadioIP->SetValue(flag);
	

		m_pFirstName->SetEnabled((bool)m_pRadioPersonal->Value());
		m_pLastName->SetEnabled((bool)m_pRadioPersonal->Value());
		m_pEmail->SetEnabled((bool)m_pRadioPersonal->Value());
		m_pShortcutField->SetEnabled((bool)m_pRadioPersonal->Value());
		m_pIP->SetEnabled((bool)m_pRadioIP->Value());
	
	m_pConnect->SetEnabled(flag);

}
void
ConnectView::Field_error(const char* msg) {
BAlert *myAlert = new BAlert(	"Error", msg,
												"Ok", NULL, NULL, 
												B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT); 
				myAlert->Go();
}
