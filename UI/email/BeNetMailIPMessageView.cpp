/***********************************************************************************************\
*	BeNetMailIPMessageView.h																	*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 19-03-99																*
*************************************************************************************************
*	C'est une ShrinkView qui affiche le message d'un PEOPLE_ID_NODE.							*
\***********************************************************************************************/
#ifndef __BENET_MAIL_IP_MESSAGE_VIEW__
#include "BeNetMailIPMessageView.h"
#endif

#ifndef _BitmapCatalog_h
#include "BitmapCatalog.h"
#endif


BeNetMailIPMessageView::BeNetMailIPMessageView(BRect frame)
	: ShrinkView(frame, "Message", false)
{
	pMessage = NULL;
}

void BeNetMailIPMessageView::Draw(BRect updateRect)
{
	ShrinkView::Draw(updateRect);
/*	
	if(IsShrink()) return;
	
	float fLeft = 4;
	float fRight = Bounds().Width() - 4;
	float fTop = 20;
	float fBottom = Bounds().Height() - 12;
	
	SetHighColor(160,160,160,0);
	StrokeLine(BPoint(fLeft, fTop),BPoint(fRight, fTop));
	StrokeLine(BPoint(fLeft, fTop),BPoint(fLeft, fBottom));	
	SetHighColor(128,128,128,0);
	StrokeLine(BPoint(fLeft-1, fTop-1),BPoint(fRight+1, fTop-1));
	StrokeLine(BPoint(fLeft-1, fTop-1),BPoint(fLeft-1, fBottom+1));	
	
	SetHighColor(208,208,208,0);
	StrokeLine(BPoint(fRight, fBottom),BPoint(fLeft, fBottom));
	StrokeLine(BPoint(fRight, fBottom),BPoint(fRight, fTop));	
	SetHighColor(224,224,224,0);
	StrokeLine(BPoint(fRight+1, fBottom+1),BPoint(fLeft-1, fBottom+1));
	StrokeLine(BPoint(fRight+1, fBottom+1),BPoint(fRight+1, fTop -1));	
	*/

}

void BeNetMailIPMessageView::AttachedToWindow()
{
	pMessage = new BTextView(BRect(8,16,Bounds().Width()-8, Bounds().Height() - 8), "MailIPMessage" , BRect(2,2,Bounds().Width()-12, 64), 0 , B_WILL_DRAW);

	AddChild(pMessage);

}
		
void BeNetMailIPMessageView::SetInfo(PEOPLE_ID_NODE* pNode)
{
	pMessage->SetText(pNode->pzMessage, strlen(pNode->pzMessage));
}
