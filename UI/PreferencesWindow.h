
#ifndef _PreferencesWindow_h
#define _PreferencesWindow_h

#include <Window.h>
#include <Point.h>

class PreferencesWindow : public BWindow
{
public: 
	PreferencesWindow();
	virtual bool	QuitRequested(); 
	virtual void	FrameMoved(BPoint point);
private:

};

#endif // _PreferencesWindow_h
