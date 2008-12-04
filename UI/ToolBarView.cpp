#ifndef _ToolBarView_h
#include "ToolBarView.h"
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

#include <locale/Locale.h>

ToolBarView::ToolBarView(BRect frame) :
	BView(frame, "ToolBarView", 0, B_WILL_DRAW)
{
	SetViewColor(216,216,216);
}

void
ToolBarView::AttachedToWindow()
{
	BRect b = Bounds();
	
	// Remote connection -----------------------------------------------------------------------------
	AddChild(m_pRemote = new BeNetButton(	BRect(b.left+2,b.top+2, b.left+38, b.top+38),
											BitmapCatalog::Instance()->GetToolBarRemote(),
											NULL, MSG_MENU_CONNECT, BENET_DM_TOGGLE, MSG_MENU_DISCONNECT));
	BubbleHelper::Instance()->SetHelp(m_pRemote, (char *)_T("Connect"));
	b = m_pRemote->Frame();

	// Server connection -----------------------------------------------------------------------------
	AddChild(m_pServer = new BeNetButton(	BRect(b.right+4,b.top, b.right+40, b.bottom),
											BitmapCatalog::Instance()->GetToolBarServer(),
											NULL, MSG_MENU_ONLINE, BENET_DM_TOGGLE, MSG_MENU_OFFLINE));
	BubbleHelper::Instance()->SetHelp(m_pServer, (char *)_T("Go online"));
	b = m_pServer->Frame();

	// Bookmarks -------------------------------------------------------------------------------------
	AddChild(m_pBookmarks = new BeNetButton(BRect(b.right+4,b.top, b.right+40, b.bottom),
											BitmapCatalog::Instance()->GetToolBarBookmarks(),
											NULL, MSG_MENU_BOOKMARKS, BENET_DM_FULL_BMP));
	BubbleHelper::Instance()->SetHelp(m_pBookmarks, (char *)_T("Bookmarks"));
	b = m_pBookmarks->Frame();

	// Preferences -----------------------------------------------------------------------------------
	AddChild(m_pPreferences = new BeNetButton(	BRect(b.right+4,b.top, b.right+40, b.bottom),
												BitmapCatalog::Instance()->GetToolBarPreferences(),
												NULL, MSG_MENU_PREFERENCES, BENET_DM_FULL_BMP));
	BubbleHelper::Instance()->SetHelp(m_pPreferences, (char *)_T("Preferences"));
	b = m_pPreferences->Frame();

	// Help ------------------------------------------------------------------------------------------
	AddChild(m_pHelp = new BeNetButton(	BRect(b.right+4,b.top, b.right+40, b.bottom),
										BitmapCatalog::Instance()->GetToolBarHelp(),
										NULL, MSG_MENU_HELP, BENET_DM_FULL_BMP));
	BubbleHelper::Instance()->SetHelp(m_pHelp, (char *)_T("Help"));
}

void
ToolBarView::Online()
{
	m_pServer->SetToggle(true);
	m_pServer->Invalidate();
	BubbleHelper::Instance()->SetHelp(m_pServer, (char *)_T("Go offline"));
}

void
ToolBarView::Offline()
{
	m_pServer->SetToggle(false);
	m_pServer->Invalidate();
	BubbleHelper::Instance()->SetHelp(m_pServer, (char *)_T("Go online"));
}

void
ToolBarView::Connect()
{
	m_pRemote->SetToggle(true);
	m_pRemote->Invalidate();
	BubbleHelper::Instance()->SetHelp(m_pRemote, (char *)_T("Disconnect"));
}

void
ToolBarView::Disconnect()
{
	m_pRemote->SetToggle(false);
	m_pRemote->Invalidate();
	BubbleHelper::Instance()->SetHelp(m_pRemote, (char *)_T("Connect"));
}
void ToolBarView::SetEnable(bool enabled)//OliverESP
{
	m_pRemote->SetEnabled(enabled);
	m_pServer->SetEnabled(enabled);
}

