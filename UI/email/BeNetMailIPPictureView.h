/***********************************************************************************************\
*	BeNetMailIPInfoView.h																		*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 19-03-99																*
*************************************************************************************************
*	C'est une ShrinkView qui affiche l'image d'un PEOPLE_ID_NODE.								*
\***********************************************************************************************/
#ifndef __BENET_MAIL_IP_PICTURE_VIEW__
#define __BENET_MAIL_IP_PICTURE_VIEW__

#ifndef __SHRINK_VIEW__
#include "ShrinkView.h"
#endif

#ifndef _EMAILIP_
#include "EMailIP.h"
#endif

#include <Bitmap.h>


class BeNetMailIPPictureView : public ShrinkView
{
public:
				BeNetMailIPPictureView(BRect frame);
				~BeNetMailIPPictureView();

	void 		SetInfo(PEOPLE_ID_NODE* );
	void 		Draw(BRect updateRect);
	
private:
	BBitmap* 	m_pBitmap;
};

#endif //__BENET_MAIL_IP_PICTURE_VIEW__
