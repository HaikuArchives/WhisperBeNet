#ifndef _ToolBarView_h
#define _ToolBarView_h

#ifndef __BENET_BUTTON__
#include "BeNetButton.h"
#endif

#include <View.h>


class ToolBarView : public BView
{
public:
	ToolBarView(BRect frame);
	virtual void	AttachedToWindow();
	void			Online();	
	void			Offline();	
	void			Connect();	
	void			Disconnect();	

	void           SetEnable(bool enabled); //OliverESP


private:
	BeNetButton*	m_pRemote;
	BeNetButton*	m_pServer;
	BeNetButton*	m_pBookmarks;
	BeNetButton*	m_pPreferences;
	BeNetButton*	m_pHelp;
};

#endif // _ToolBarView_h
