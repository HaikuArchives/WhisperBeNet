#ifndef __BENET_MAIL_IP_CONTROL_VIEW__
#define __BENET_MAIL_IP_CONTROL_VIEW__


#ifndef __BENET_BUTTON__
#include "BeNetButton.h"
#endif

#ifndef _EMAILIP_
#include "EMailIP.h"
#endif

#ifndef _BitmapCatalog_h
#include "BitmapCatalog.h"
#endif

#include <View.h>


#ifndef BENET_NEXT
#define BENET_NEXT 0x000F0003
#endif

#ifndef BENET_PREV
#define BENET_PREV 0x000F0004
#endif

#ifndef BENET_ACCEPT
#define BENET_ACCEPT 0x000F0005
#endif


#ifndef BENET_CANCEL
#define BENET_CANCEL 0x000F0006
#endif

#ifndef BENET_DELETE
#define BENET_DELETE 0x000F0007
#endif

class BeNetMailIPControlView : public BView{
	public:
		BeNetMailIPControlView(BRect frame);
		void AttachedToWindow();
		void Draw(BRect updateRect);
		void SetInfo(PEOPLE_ID_NODE* pNode);
		
	private:
		void DrawDegrader(BRect rect, int iStartGray, int iEndGray);
		
		BeNetButton * pNextButton;
		BeNetButton * pPrevButton;
		BeNetButton * pAcceptButton;
		BeNetButton * pCancelButton;
		BeNetButton * pDeleteButton;
};


#endif // __BENET_MAIL_IP_CONTROL_VIEW__
