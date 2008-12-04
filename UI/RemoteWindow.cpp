#ifndef __RemoteWindow__
#include "RemoteWindow.h"
#endif
	
#ifndef _Prefernences_h
#include "Preferences.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#ifndef _BeNetWindow_h
#include "BeNetWindow.h"
#endif

#ifndef _Network
#include "network.h"
#endif

#include <locale/Locale.h>

RemoteWindow::RemoteWindow(BeNetWindow* window) :
	BWindow(BRect(0,0,0,0), "RemoteUser", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE | B_NOT_ZOOMABLE, B_CURRENT_WORKSPACE),
	m_pBeNetWindow(window)
{
	BRect AFrame;
	
	AddChild(m_pInformationView = new InformationView(BRect(0, 0, 200, 90), "InformationView"));
	AFrame = m_pInformationView->Frame();

	AddChild(m_pScopeView = new ScopeView(BRect(0,AFrame.bottom,200,AFrame.bottom + 96)));
	AFrame = m_pScopeView->Frame();

	AddChild(m_pSoundControlsView = new SoundControlsView(BRect(AFrame.left, AFrame.bottom + 1, AFrame.right, AFrame.bottom + 51), !Preferences::Instance()->ShowSoundControls()));
	AFrame = m_pSoundControlsView->Frame();
	m_pSoundControlsView->Volume(Preferences::Instance()->PlayVolume());

	// Add the player
	m_pPlayer = new Player(m_pScopeView);

	AddChild(m_pPictureView = new PictureView(BRect(0, AFrame.bottom + 1, 200, AFrame.bottom + 130)));
	AFrame = m_pPictureView->Frame();
	
	ResizeTo(AFrame.right, AFrame.bottom - 1);
	MoveTo(	Preferences::Instance()->RemoteUserX(),
			Preferences::Instance()->RemoteUserY());
}	

RemoteWindow::~RemoteWindow()
{

}

bool RemoteWindow::QuitRequested()
{
	//OliverESP: give a chance if the user closed by mistake
	BAlert *myAlert = new BAlert(_T("End of conversation?"), _T("Are you sure that you want to end the conversation?"),_T("Yes"), _T("No"), NULL, 
												B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT); 
	if (!myAlert->Go()) { // yes we finish
	
		BMessage msg;
		msg.what = MSG_MENU_DISCONNECT;
		BMessenger(m_pBeNetWindow).SendMessage(&msg);
	//	Hide();
		return false;
	}
	else {				  // Go on talking
		return false;
	}
	

}

void
RemoteWindow::FrameMoved(BPoint point)
{
	Preferences::Instance()->RemoteUserX(point.x);
	Preferences::Instance()->RemoteUserY(point.y);
}

void
RemoteWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case MSG_SOUNDCONTROLS_VOLUME:
		{
			Preferences::Instance()->PlayVolume(m_pSoundControlsView->Volume());
			m_pPlayer->SetPlayVolume(((float)Preferences::Instance()->PlayVolume()/(float)100));
			break;
		}
		case MSG_SOUNDCONTROLS_MUTE:
		{
			m_pPlayer->SetPlayVolume((bool)(m_pSoundControlsView->IsMute() == B_CONTROL_ON) ? 0.0 : ((float)m_pSoundControlsView->Volume()/(float)100));
			break;
		}
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}

void
RemoteWindow::Show()
{
	BWindow::Show();

	if (CNetwork::Instance()->IsConnectionReady())
	{
		SetTitle(CNetwork::Instance()->GetRemoteIP());
		m_pInformationView->Update();
	}
}
