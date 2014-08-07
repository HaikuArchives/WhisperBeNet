/***********************************************************************************************\
*	BeNetMailIPWindow.h																			*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 19-03-99																*
*************************************************************************************************
*	Cette classe permet d'affichier une window modal qui affiche les candidat qui veulent se 	*
*	connecter.																					*
\***********************************************************************************************/

#ifndef __BENET_MAIL_IP_WINDOWS__
#define __BENET_MAIL_IP_WINDOWS__

#include "BeNetMailIPInfoView.h"

#ifndef __BENET_MAIL_IP_CONTROL_VIEW__
#include "BeNetMailIPControlView.h"
#endif

#include "BeNetMailIPPictureView.h"

#include "BeNetMailIPMessageView.h"


#include <Window.h>

class BeNetMailIPWindow :public BWindow
{
public:
								BeNetMailIPWindow(PEOPLE_ID_NODE* pNode);		
								~BeNetMailIPWindow();
	void 						MessageReceived(BMessage *message);

private:	//Function
	void						RemoveCurrentNode();
	void 						SetInfo();

private:	//Data
	BeNetMailIPInfoView * 		pInfoView;
	BeNetMailIPControlView * 	pControlView;
	BeNetMailIPPictureView * 	pPictureView;
	BeNetMailIPMessageView * 	pMessageView;

	PEOPLE_ID_NODE *			m_pNode;	
};

#endif //__BENET_MAIL_IP_WINDOWS__
