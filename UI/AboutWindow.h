#ifndef _About_h
#define _About_h

#include <Window.h>

class AboutWindow : public BWindow
{
public:
	AboutWindow();
	virtual bool	QuitRequested();
}

#endif // _About_h
