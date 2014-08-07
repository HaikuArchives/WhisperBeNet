#ifndef __RemoteWindow__
#define __RemoteWindow__

#ifndef _ScopeView_h
#include "ScopeView.h"
#endif

#ifndef _SoundControlsView_h
#include "SoundControlsView.h"
#endif

#ifndef _PictureView_h
#include "PictureView.h"
#endif

#ifndef _Player_h
#include "Player.h"
#endif

#ifndef __INFORMATION_VIEW__
#include "InformationView.h"
#endif

#include <Window.h>


class BeNetWindow;


class RemoteWindow : public BWindow
{
public:
	RemoteWindow(BeNetWindow* window);
	~RemoteWindow();
	virtual bool QuitRequested();
	virtual void FrameMoved(BPoint point);
	virtual void MessageReceived(BMessage* msg);
	virtual void Show();

private:
	InformationView*	m_pInformationView;
	ScopeView*			m_pScopeView;
	SoundControlsView*	m_pSoundControlsView;
	PictureView*		m_pPictureView;
	Player*				m_pPlayer;
	BeNetWindow*		m_pBeNetWindow;
};

#endif // __RemoteWindow__
