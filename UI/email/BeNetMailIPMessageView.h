/***********************************************************************************************\
*	BeNetMailIPMessageView.h																	*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 19-03-99																*
*************************************************************************************************
*	C'est une ShrinkView qui affiche le message d'un PEOPLE_ID_NODE.							*
\***********************************************************************************************/
#ifndef __BENET_MAIL_IP_MESSAGE_VIEW__
#define __BENET_MAIL_IP_MESSAGE_VIEW__

#ifndef __SHRINK_VIEW__
#include "ShrinkView.h"
#endif

#ifndef _EMAILIP_
#include "EMailIP.h"
#endif

#include <TextView.h>

class BeNetMailIPMessageView : public ShrinkView
{
public:
				BeNetMailIPMessageView(BRect frame);
	void 		AttachedToWindow();
	void		Draw(BRect updateRect);
	void		SetInfo(PEOPLE_ID_NODE* pNode);
private:
	BTextView* 	pMessage;
};


#endif // __BENET_MAIL_IP_MESSAGE_VIEW__
