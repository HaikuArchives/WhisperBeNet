#ifndef _WaitWindow_h
#include "WaitWindow.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#include <View.h>
#include <Button.h>
#include <locale/Locale.h>

WaitWindow::WaitWindow() :
	BWindow(BRect(300,300,500,350), "Connecting...",B_MODAL_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_NOT_RESIZABLE | B_NOT_ZOOMABLE, B_CURRENT_WORKSPACE)
{
	BRect b = Bounds();
	waitView = new BView(BRect(b.left,b.top,b.right,b.bottom), "waitView", 0, B_WILL_DRAW);
	AddChild(waitView);
	waitView->SetViewColor(216,216,216);

//	OliverESP: At this point no safe way to abort the process. kill threads... etc etc	
//	waitView->AddChild(new BButton(BRect(((b.right-b.left)/2)-30,b.bottom-25,((b.right-b.left)/2)+30,b.bottom-5), "cancel", "Cancel", new BMessage(MSG_ABORT_CONNECTION)));

	waitView->AddChild(m_pStringView = new BStringView(BRect(b.left+5, b.top+10, b.right, b.top + 25), NULL, "Connecting to ... "));
	m_pStringView->SetFont(be_bold_font); //	OliverESP: nicer
}


WaitWindow::~WaitWindow()
{

}


bool
WaitWindow::QuitRequested()
{
	Hide();
	Unlock();
	return true;
}


void
WaitWindow::SetText(const char* text)
{
	BString label;
	label << _T("Connecting to ... ") << text;  //OliverESP to consider...
	Lock();
	m_pStringView->SetText(label.String());
	
	// Readapting window and View ...
	m_pStringView->ResizeToPreferred();
	BRect windb(m_pStringView->Bounds());
	windb.InsetBy(-16, -16);
	waitView->ResizeTo(windb.Width(), windb.Height());
	ResizeTo(windb.Width(), windb.Height());
	

    be_bold_font->StringWidth( label.String() ) ;
	
	
	Unlock();
}
