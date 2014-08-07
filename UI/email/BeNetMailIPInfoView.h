/***********************************************************************************************\
*	BeNetMailIPInfoView.h																		*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 19-03-99																*
*************************************************************************************************
*	C'est une ShrinkView qui affiche les donnee d'un PEOPLE_ID_NODE.							*
\***********************************************************************************************/
#ifndef __BENET_MAIL_IP_INFO_VIEW__
#define __BENET_MAIL_IP_INFO_VIEW__

#ifndef __SHRINK_VIEW__
#include "ShrinkView.h"
#endif

#ifndef _EMAILIP_
#include "EMailIP.h"
#endif

#include <TextControl.h>


class BeNetMailIPInfoView : public ShrinkView
{
public:
					BeNetMailIPInfoView(BRect frame);
	void 			AttachedToWindow();
	void 			SetInfo(PEOPLE_ID_NODE * pNode);
	
private:
	BTextControl * 	pIP;
	BTextControl * 	pName;
	BTextControl * 	pEMail;
	BTextControl * 	pDate;
}; 

#endif //__BENET_MAIL_IP_INFO_VIEW__
