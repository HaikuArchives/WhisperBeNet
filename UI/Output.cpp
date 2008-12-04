#ifndef _Output_h
#include "Output.h"
#endif

#ifndef _Preferences_h
#include "Preferences.h"
#endif



/****************************************************************/
/*	OutputView													*/
/****************************************************************/

OutputView::OutputView(BRect frame) :
	BView(frame, "OutputView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	SetViewColor(216,216,216);
	rgb_color color = {0,216,0};

	BRect b = Bounds();
	AddChild(m_pTextView = new BTextView(BRect(b.left+5,b.top+5,b.right-B_V_SCROLL_BAR_WIDTH-6,b.bottom-5), "Output", BRect(b.left+5,b.top+5,b.right-B_V_SCROLL_BAR_WIDTH-6,b.bottom-5), NULL, &color, B_FOLLOW_ALL_SIDES, B_WILL_DRAW));
	m_pTextView->SetViewColor(0,0,0);
	m_pTextView->MakeEditable(false);

	AddChild(m_pScrollBar = new BScrollBar(BRect(b.right-B_V_SCROLL_BAR_WIDTH-5,b.top+5,b.right-5, b.bottom-5), "outputScroll", m_pTextView, 0, 0, B_VERTICAL));
}


void
OutputView::FrameResized(float width, float height)
{
	BView::FrameResized(width, height);
	m_pTextView->SetTextRect(BRect(0,0,width,height));
}


/****************************************************************/
/*	Output														*/
/****************************************************************/

// Singleton implementation
Output* Output::m_instance = 0;

Output::Output() :
	BWindow(BRect(200,200,600,600), "Output", B_TITLED_WINDOW, B_NOT_RESIZABLE)
{
	BRect b = Bounds();

	BView* resetView = new BView(BRect(b.left,b.bottom-25,b.right,b.bottom), "resetView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	resetView->SetViewColor(216,216,216);
	resetView->AddChild(m_pReset = new BButton(BRect(1,1,61,20), "reset all", "Clear", new BMessage(MSG_OUTPUT_RESET), B_FOLLOW_BOTTOM));
	resetView->AddChild(m_pResetAll = new BButton(BRect(70,1,130,20), "reset", "Clear all", new BMessage(MSG_OUTPUT_RESET_ALL), B_FOLLOW_BOTTOM));
	AddChild(resetView);

	BTabView* tabView = new BTabView(BRect(b.left,b.top,b.right,b.bottom-25), "tab_view", B_WIDTH_FROM_WIDEST/*, B_FOLLOW_ALL_SIDES, B_FULL_UPDATE_ON_RESIZE*/); 
	tabView->SetViewColor(216,216,216); 

	b = tabView->Bounds(); 
	b.bottom -= tabView->TabHeight(); 

	tabView->AddTab(m_pAll = new OutputView(b), m_pAllTab = new BTab()); 
	m_pAllTab->SetLabel("All");

	tabView->AddTab(m_pAudio = new OutputView(b), m_pAudioTab = new BTab()); 
	m_pAudioTab->SetLabel("Audio");

	tabView->AddTab(m_pNetwork = new OutputView(b), m_pNetworkTab = new BTab()); 
	m_pNetworkTab->SetLabel("Network");

	tabView->AddTab(m_pUI = new OutputView(b), m_pUITab = new BTab()); 
	m_pUITab->SetLabel("UI");

	AddChild(tabView);

	MoveTo(	Preferences::Instance()->OutputX(),
			Preferences::Instance()->OutputY());
}

Output::~Output()
{

}


Output*
Output::Instance()
{
	if (!m_instance)
		m_instance = new Output;
	return m_instance;
}


bool
Output::QuitRequested()
{
	Hide();
	return false;
}


void
Output::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
	case MSG_OUTPUT_RESET:
		if (m_pAllTab->IsSelected())
			m_pAll->Clear();
		else if (m_pAudioTab->IsSelected())
			m_pAudio->Clear();
		else if (m_pNetworkTab->IsSelected())
			m_pNetwork->Clear();
		else
			m_pUI->Clear();
		break;
	case MSG_OUTPUT_RESET_ALL:
		m_pAll->Clear();
		m_pAudio->Clear();
		m_pNetwork->Clear();
		m_pUI->Clear();
		break;
	default:
		BWindow::MessageReceived(msg);
		break;
	}
}


void
Output::FrameMoved(BPoint point)
{
	Preferences::Instance()->OutputX(point.x);
	Preferences::Instance()->OutputY(point.y);
}


void
Output::Add(char* text, OutputView* view)
{
	Lock();
	view->Add(text);
	m_pAll->Add(text);
	Unlock();
}

