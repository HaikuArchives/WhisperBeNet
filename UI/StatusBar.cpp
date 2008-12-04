#ifndef _StatusBar_h
#include "StatusBar.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#ifndef _BubbleHelper_H
#include "BubbleHelper.h"
#endif

#ifndef _BitmapCatalog_h
#include "BitmapCatalog.h"
#endif

#include <stdio.h>
#include <locale/Locale.h>

StatusBar::StatusBar(BRect frame) :
	BView(frame, "StatusBar", B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(216,216,216);

	BRect b = Bounds();

	// Remote connection -----------------------------------------------------------------------------
	AddChild(m_pRemoteButton = new BeNetButton(	BRect(b.left+1,b.top+3,b.left+29,b.top+29),
												BitmapCatalog::Instance()->GetStatusBarRemote(),
												"RemoteConnection",
												MSG_MENU_CONNECT,
												BENET_DM_TOGGLE,
												MSG_MENU_DISCONNECT,
												true));
	BubbleHelper::Instance()->SetHelp(m_pRemoteButton, "Connect to remote user");
	AddChild(m_pRemoteStatus = new BStringView(BRect(b.left+30,b.top+1,b.left+120,b.top+16), "remoteState", _T("Disconnected")));


	// Server connection -----------------------------------------------------------------------------
	AddChild(m_pServerButton = new BeNetButton(	BRect(b.right-18,b.top+1,b.right-1,b.top+27),
												BitmapCatalog::Instance()->GetStatusBarServer(),
												"ServerConnection",
												MSG_MENU_ONLINE,
												BENET_DM_TOGGLE,
												MSG_MENU_OFFLINE,
												true));
	BubbleHelper::Instance()->SetHelp(m_pServerButton, "Go online");
	AddChild(m_pServerStatus = new BStringView(BRect(b.right-90,b.top+1,b.right-20,b.top+16), "serverState", _T("Offline")));
}


StatusBar::~StatusBar()
{

}

void
StatusBar::Online()
{
	m_pServerStatus->SetText(_T("Online"));
	m_pServerButton->SetToggle(true);
	m_pServerButton->Invalidate();
	BubbleHelper::Instance()->SetHelp(m_pServerButton, "Go offline");
}

void
StatusBar::Offline()
{
	m_pServerStatus->SetText(_T("Offline"));
	m_pServerButton->SetToggle(false);
	m_pServerButton->Invalidate();
	BubbleHelper::Instance()->SetHelp(m_pServerButton, "Go online");
}

void
StatusBar::Connected()
{
	m_pRemoteStatus->SetText(_T("Connected"));
	m_pRemoteButton->SetToggle(true);
	m_pRemoteButton->Invalidate();
	BubbleHelper::Instance()->SetHelp(m_pRemoteButton, "Disconnect current call");
}

void
StatusBar::Disconnected()
{
	m_pRemoteStatus->SetText(_T("Disconnected"));
	m_pRemoteButton->SetToggle(false);
	m_pRemoteButton->Invalidate();
	BubbleHelper::Instance()->SetHelp(m_pRemoteButton, "Connect remote user");
}
void StatusBar::SetEnable(bool enabled) //OliverESP
{
	m_pRemoteButton->SetEnabled(enabled);
	m_pServerButton->SetEnabled(enabled);
}
