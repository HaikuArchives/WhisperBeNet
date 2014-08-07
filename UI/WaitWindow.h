
#ifndef _WaitWindow_h
#define _WaitWindow_h

#include <Window.h>
#include <StringView.h>

class WaitWindow : public BWindow
{
public:
	WaitWindow();
	~WaitWindow();
	virtual bool	QuitRequested();
	void			SetText(const char* text);


private:
	BStringView*	m_pStringView;
	BView* 			waitView;
};

#endif // _WaitWindow_h
