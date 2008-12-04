#ifndef _StatusBar_h
#define _StatusBar_h

#ifndef __BENET_BUTTON__
#include "BeNetButton.h"
#endif

#include <View.h>
#include <Bitmap.h>
#include <StringView.h>


class StatusBar : public BView
{
public:
	StatusBar(BRect frame);
	~StatusBar();

	void			Online();
	void			Offline();
	void			Connected();
	void			Disconnected();
	
	void           SetEnable(bool enabled); //OliverESP

private:
	BeNetButton*	m_pServerButton;
	BeNetButton*	m_pRemoteButton;
	BStringView*	m_pServerStatus;
	BStringView*	m_pRemoteStatus;
};

#endif // _StatusBar_h
