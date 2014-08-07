#ifndef _About_h
#define _About_h

#include <Window.h>
#include <View.h>

class AboutWindow : public BWindow
{
public:
	AboutWindow();
	~AboutWindow();
	virtual bool	QuitRequested();
//	void			Animate();
//	void			Scroll();

private:
	BView*			m_pAboutView;
	BView* 			m_pLogoView;
	int				m_height;
	bool			m_animate;
};

#endif // _About_h
