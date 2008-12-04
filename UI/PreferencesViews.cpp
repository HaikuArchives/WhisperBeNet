//#define SHOW_AUDIO_INPUTS

#ifndef _PreferencesViews_h
#include "PreferencesViews.h"
#endif

#ifndef _Preferences_h
#include "Preferences.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#ifndef _Output_h
#include "Output.h"
#endif

#include <stdlib.h>
#include <locale/Locale.h>

/************************************/
/* General Preferences              */
/************************************/
GeneralPreferencesView::GeneralPreferencesView(BRect frame)
	: BView(frame, "General" , B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(216,216,216);

	AddChild(m_shrinkOscilloscope = new BCheckBox(BRect(10,15,150,30), "Oscilloscope",_T("Oscilloscope"), new BMessage(MSG_GENERAL_SHOW_OSCILLOSCOPE)));
	AddChild(m_shrinkSoundControls = new BCheckBox(BRect(10,35,150,50), "Sound Controls", _T("Sound Controls"), new BMessage(MSG_GENERAL_SHOW_SOUNDCONTROLS)));
	AddChild(m_shrinkConnection = new BCheckBox(BRect(10,55,150,70), "Connection", _T("Connection"), new BMessage(MSG_GENERAL_SHOW_CONNECTION)));
	AddChild(m_bookmarksWindow = new BCheckBox(BRect(10,75,150,90), "Bookmarks", _T("Bookmarks"), new BMessage(MSG_GENERAL_SHOW_BOOKMARKS)));
	AddChild(m_outputWindow = new BCheckBox(BRect(10,95,150,110), "Output", "Output", new BMessage(MSG_GENERAL_SHOW_OUTPUT)));

	// Set the controls' values
	m_shrinkOscilloscope->SetValue((int32)Preferences::Instance()->ShowOscilloscope());
	m_shrinkSoundControls->SetValue((int32)Preferences::Instance()->ShowSoundControls());
	m_shrinkConnection->SetValue((int32)Preferences::Instance()->ShowConnection());
	m_bookmarksWindow->SetValue((int32)Preferences::Instance()->ShowBookmarks());
	m_outputWindow->SetValue((int32)Preferences::Instance()->ShowOutput());
}

void
GeneralPreferencesView::AllAttached()
{
	m_shrinkOscilloscope->SetTarget(this);
	m_shrinkSoundControls->SetTarget(this);
	m_shrinkConnection->SetTarget(this);
	m_bookmarksWindow->SetTarget(this);
	m_outputWindow->SetTarget(this);
}

void
GeneralPreferencesView::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
	case MSG_GENERAL_SHOW_OSCILLOSCOPE:
		Output::Instance()->UI("Preferences General: MSG_GENERAL_SHOW_OSCILLOSCOPE\n");
		Preferences::Instance()->ShowOscilloscope(m_shrinkOscilloscope->Value());
		break;
	case MSG_GENERAL_SHOW_SOUNDCONTROLS:
		Output::Instance()->UI("Preferences General: MSG_GENERAL_SHOW_SOUNDCONTROLS\n");
		Preferences::Instance()->ShowSoundControls(m_shrinkSoundControls->Value());
		break;
	case MSG_GENERAL_SHOW_CONNECTION:
		Output::Instance()->UI("Preferences General: MSG_GENERAL_SHOW_CONNECTION\n");
		Preferences::Instance()->ShowConnection(m_shrinkConnection->Value());
		break;
	case MSG_GENERAL_SHOW_BOOKMARKS:
		Output::Instance()->UI("Preferences General: MSG_GENERAL_SHOW_BOOKMARKS\n");
		Preferences::Instance()->ShowBookmarks(m_bookmarksWindow->Value());
		break;
	case MSG_GENERAL_SHOW_OUTPUT:
		Output::Instance()->UI("Preferences General: MSG_GENERAL_SHOW_OUTPUT\n");
		Preferences::Instance()->ShowOutput(m_outputWindow->Value());
		break;
	default:
		BView::MessageReceived(msg);
		break;
	}
}


/************************************/
/* Audio Preferences                */
/************************************/
AudioPreferencesView::AudioPreferencesView(BRect frame)
	: BView(frame, "Audio", B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(216,216,216);

//	AddChild(m_useModemQuality = new BCheckBox(BRect(10,15,100,30), "Modem Quality", "Modem Quality", new BMessage(MSG_AUDIO_USE_MODEL_QUALITY)));
//	AddChild(m_useCodec = new BCheckBox(BRect(10,31,100,46), "Use codec", "Use codec", new BMessage(MSG_AUDIO_USE_CODEC)));
	AddChild(m_useModemQuality = new BRadioButton(BRect(10,15,160,30), "Modem Quality", _T("Modem Quality"), new BMessage(MSG_AUDIO_USE_MODEL_QUALITY)));
	AddChild(m_useCodec = new BRadioButton(BRect(10,35,160,50), "Use codec", _T("Use codec"), new BMessage(MSG_AUDIO_USE_CODEC)));
	AddChild(m_custom = new BRadioButton(BRect(10,55,160,70), "Custom compression", _T("Custom compression"), new BMessage(MSG_AUDIO_CUSTOM)));
			 m_custom->ResizeToPreferred();
	AddChild(m_slider_compression = new BSlider(BRect(170,70,270,85), "Compression Slider", _T("NBR rate: "), new BMessage(MSG_AUDIO_COMP_CHANGED),1,32));
	m_slider_compression->SetLimitLabels("1","32");
	
	// Set the controls' values
//	m_useCodec->SetValue((int32)Preferences::Instance()->UseCodec());
//	m_useModemQuality->SetValue((int32)Preferences::Instance()->UseModemQuality());
//	m_useModemQuality->SetEnabled((bool)(m_useCodec->Value() == B_CONTROL_ON));	
	int32 comp = (int32)Preferences::Instance()->Compression();
	switch(comp) {
		case 4:
			m_useCodec		  ->SetValue(B_CONTROL_ON);
			m_slider_compression->SetEnabled(false);
			break;
		case 6:
			m_useModemQuality ->SetValue(B_CONTROL_ON);
			m_slider_compression->SetEnabled(false);
			break;
		default:
			m_custom		  ->SetValue(B_CONTROL_ON);
			m_slider_compression->SetEnabled(true);
			break;		
	}	
	m_slider_compression->SetValue(comp);

//Set the label for feedback
BString label;
label << _T("NBR rate: ") << Preferences::Instance()->Compression(); 
m_slider_compression->SetLabel(label.String());


#ifdef SHOW_AUDIO_INPUTS	
// Get possible entries 
	CreateInputs();
// ... and set by default the first if it actually has!
	if (mInputs->ItemAt(0)!=NULL)
		(mInputs->ItemAt(0))->SetMarked(true);	
#endif	
}

void 
AudioPreferencesView::CreateInputs() {
	status_t err = B_OK;
	mInputs = new BPopUpMenu(_T("Audio inputs"));
	mInputs->SetRadioMode(true);

///////////
//	Checking media roster existence
    BMediaRoster* m_roster = BMediaRoster::Roster(&err);

    if (!m_roster || (err != B_OK)) {
		BAlert *myAlert = new BAlert("Known", "There was an error setting the sound recording:\n\nmedia_server->Rip();" ,"Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT); 
		myAlert->Go();
		return;
	}

//	Discovering input devices	
	BMessage * msg = new BMessage(DEVICE);
	msg->AddInt32("node_id", -1);

	live_node_info lni[30];
	int32 ioCount = 30;
	media_format out;
	out.type = B_MEDIA_RAW_AUDIO;
	err = BMediaRoster::Roster()->GetLiveNodes(lni, &ioCount, 0, &out, 0, B_BUFFER_PRODUCER | B_PHYSICAL_INPUT);
	if (err < B_OK) {
		fprintf(stderr, "GetLiveNodes() failed: %s\n", strerror(err));
		(new BAlert("", strerror(err), "Stop"))->Go();
	}
	else for (int ix=0; ix<ioCount; ix++) {
		msg = new BMessage(DEVICE);
		msg->AddInt32("node_id", lni[ix].node.node);
		char s[12];
		sprintf(s, "%d", ix+1);
		
		mInputs->AddItem(new BMenuItem(lni[ix].name, msg));
	}

	dormant_node_info dni[30];
	ioCount = 30;
	out.type = B_MEDIA_RAW_AUDIO;
	err = BMediaRoster::Roster()->GetDormantNodes(dni, &ioCount, 0, &out, 0, B_BUFFER_PRODUCER | B_PHYSICAL_INPUT);
	if (err < B_OK) {
		fprintf(stderr, "GetDormantNodes() failed: %s\n", strerror(err));
		(new BAlert("", strerror(err), "Stop"))->Go();
	}
	else for (int ix=0; ix<ioCount; ix++) {
		media_node_id running = -1;
		if ((B_OK == BMediaRoster::CurrentRoster()->GetInstancesFor(
				dni[ix].addon, dni[ix].flavor_id, &running)) && (running > -1)) {
			fprintf(stderr, "%s: already running\n", dni[ix].name);
			continue;
		}
		msg = new BMessage(DEVICE);
		msg->AddData("dormant_info", B_RAW_TYPE, &dni[ix], sizeof(dni[ix]));
		char s[12];
		sprintf(s, "%d", ix+1);

		mInputs->AddItem(new BMenuItem(dni[ix].name, msg));
	}
	
	m_inputs = new BMenuField(BRect(10,110,160,120), "InputsField", NULL, mInputs);
	AddChild(m_inputs);
}


void
AudioPreferencesView::AllAttached()
{
	m_useModemQuality->SetTarget(this);	
	m_useCodec->SetTarget(this);
	m_custom->SetTarget(this);
	m_slider_compression->SetTarget(this);
}

void
AudioPreferencesView::MessageReceived(BMessage* msg)
{
BString label;

	switch(msg->what)
	{
	case MSG_AUDIO_USE_MODEL_QUALITY:
		Output::Instance()->UI("Preferences Audio: MSG_AUDIO_USE_MODEL_QUALITY\n");
		Preferences::Instance()->UseModemQuality(m_useModemQuality->Value()); // for compilance
		
		m_slider_compression->SetEnabled(false);
		m_slider_compression->SetValue(6);
		MessageReceived(new BMessage(MSG_AUDIO_COMP_CHANGED));
		
		break;
	case MSG_AUDIO_USE_CODEC:
		Output::Instance()->UI("Preferences Audio: MSG_AUDIO_USE_CODEC\n");
		Preferences::Instance()->UseCodec(m_useCodec->Value()); // for compilance
//		m_useModemQuality->SetEnabled((bool)(m_useCodec->Value() == B_CONTROL_ON));

		m_slider_compression->SetEnabled(false);
		m_slider_compression->SetValue(4);
		MessageReceived(new BMessage(MSG_AUDIO_COMP_CHANGED));		

		break;
	case MSG_AUDIO_CUSTOM:

		Output::Instance()->UI("Preferences Audio: MSG_AUDIO_CUSTOM\n");
		m_slider_compression->SetEnabled(true);
		
		break;		
	case MSG_AUDIO_COMP_CHANGED:  //OliverESP 		
		Output::Instance()->UI("Preferences Audio: MSG_AUDIO_COMP_CHANGED\n");		
		
		//OliverESP: Due to mathematical questions some rates are not possible
		if (m_slider_compression->Value() == 11 || m_slider_compression->Value() == 14 ||
		    m_slider_compression->Value() == 20 || m_slider_compression->Value() == 31)
			m_slider_compression->SetValue(m_slider_compression->Value()-1); //For example
			
		// update the label
		label << _T("NBR rate: ") << m_slider_compression->Value(); 
		m_slider_compression->SetLabel(label.String());

		Preferences::Instance()->Compression(m_slider_compression->Value());

		break;
	default:
		BView::MessageReceived(msg);
		break;
	}
}


/************************************/
/* Network Preferences              */
/************************************/
NetworkPreferencesView::NetworkPreferencesView(BRect frame)
	: BView(frame, "Network" , B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(216,216,216);
	BString str;

	AddChild(m_pGoOnlineOnStartup = new BCheckBox(BRect(10,15,140,30), "Online", _T("Go online on startup"), new BMessage(MSG_NETWORK_GOONLINE)));
	m_pGoOnlineOnStartup->ResizeToPreferred();
	
	str << (int)Preferences::Instance()->ConnectionTimeout();
	AddChild(m_connectionTimeout = new BTextControl(BRect(10,35,250,50), "timeout", _T("Connection timeout (min)"), str.String(), new BMessage(MSG_NETWORK_CONNECTION_TIMEOUT)));
	m_connectionTimeout->SetDivider(170);
	AddChild(m_pServerIP = new BTextControl(BRect(10,60,250,75), "serverIP", _T("Server IP address"), Preferences::Instance()->ServerIP() , new BMessage(MSG_NETWORK_SERVER_IP)));
	m_pServerIP->SetDivider(170);
	
	str.SetTo("");
	str << Preferences::Instance()->ServerPort();
	AddChild(m_pServerPort = new BTextControl(BRect(10,80,250,90), "serverPort", _T("Server Port"), str.String() , new BMessage(MSG_NETWORK_SERVER_PORT)));
	m_pServerPort->SetDivider(170);

	// Set the controls' values
	m_pGoOnlineOnStartup->SetValue((int32)Preferences::Instance()->OnlineOnStartup());
}

void
NetworkPreferencesView::AllAttached()
{
	m_connectionTimeout->SetTarget(this);
	m_pGoOnlineOnStartup->SetTarget(this);
	m_pServerIP->SetTarget(this);
	m_pServerPort->SetTarget(this);
}

void
NetworkPreferencesView::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
	case MSG_NETWORK_GOONLINE:
		Output::Instance()->UI("Preferences Network: MSG_NETWORK_GOONLINE\n");
		Preferences::Instance()->OnlineOnStartup(m_pGoOnlineOnStartup->Value());
		break;
	case MSG_NETWORK_CONNECTION_TIMEOUT:
		Output::Instance()->UI("Preferences Network: MSG_NETWORK_CONNECTION_TIMEOUT\n");
		Preferences::Instance()->ConnectionTimeout(atoi(m_connectionTimeout->Text()));
		break;
	case MSG_NETWORK_SERVER_IP:
		Output::Instance()->UI("Preferences Network: MSG_NETWORK_SERVER_IP\n");
		Preferences::Instance()->ServerIP((char*)m_pServerIP->Text());
		break;
	case MSG_NETWORK_SERVER_PORT:
		Output::Instance()->UI("Preferences Network: MSG_NETWORK_SERVER_PORT\n");
		Preferences::Instance()->ServerPort(atoi(m_pServerPort->Text()));
		break;
	default:
		BView::MessageReceived(msg);
		break;
	}
}


/************************************/
/* Identity Preferences             */
/************************************/
IdentityPreferencesView::IdentityPreferencesView(BRect frame)
	: BView(frame, "Identity" , B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(216,216,216);

	AddChild(m_pFirstName = new BTextControl(BRect(10,15,245,30), "firstname", _T("Name"), Preferences::Instance()->FirstName(), new BMessage(MSG_IDENTITY_FIRSTNAME)));
	m_pFirstName->SetDivider(70);
	AddChild(m_pLastName = new BTextControl(BRect(10,35,245,50), "lastname", _T("Surname"), Preferences::Instance()->LastName(), new BMessage(MSG_IDENTITY_LASTNAME)));
	m_pLastName->SetDivider(70);
	AddChild(m_pEmail = new BTextControl(BRect(10,55,245,70), "email", _T("E-mail"), Preferences::Instance()->Email(), new BMessage(MSG_IDENTITY_EMAIL)));
	m_pEmail->SetDivider(70);
}

void
IdentityPreferencesView::AllAttached()
{
	m_pFirstName->SetTarget(this);
	m_pLastName->SetTarget(this);
	m_pEmail->SetTarget(this);
}

void
IdentityPreferencesView::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
	case MSG_IDENTITY_FIRSTNAME:
		Output::Instance()->UI("Preferences Identity: MSG_IDENTITY_FIRSTNAME\n");
		Preferences::Instance()->FirstName((char*)m_pFirstName->Text());
		break;
	case MSG_IDENTITY_LASTNAME:
		Output::Instance()->UI("Preferences Identity: MSG_IDENTITY_LASTNAME\n");
		Preferences::Instance()->LastName((char*)m_pLastName->Text());
		break;
	case MSG_IDENTITY_EMAIL:
		Output::Instance()->UI("Preferences Identity: MSG_IDENTITY_EMAIL\n");
		Preferences::Instance()->Email((char*)m_pEmail->Text());
		break;
	default:
		BView::MessageReceived(msg);
		break;
	}
}
