#ifndef _BeNetApp_h
#define _BeNetApp_h

#include <Application.h>


class BeNetApp : public BApplication
{
public:
	BeNetApp();
	virtual void	MessageReceived(BMessage* msg);
	virtual bool	QuitRequested();
private:

};

#endif	// _BeNetApp_h
