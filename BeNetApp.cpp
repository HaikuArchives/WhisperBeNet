#ifndef _BeNetApp_h
#include "BeNetApp.h"
#endif

#ifndef _BeNetWindow_h
#include "BeNetWindow.h"
#endif

#ifndef _Network
#include "network.h"
#endif


BeNetApp::BeNetApp() 
	: BApplication("application/x-vnd.yT.Whisper")
{
	BeNetWindow* theWindow = new BeNetWindow();
	theWindow->Show();
}

void
BeNetApp::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
	default:
		BApplication::MessageReceived(msg);
		break;
	}
}

bool
BeNetApp::QuitRequested()
{
//	CNetwork::Instance()->Quit();	
	
	return BApplication::QuitRequested();
}

