#ifndef __BENET_MAIL_IP_CONTROL_VIEW__
#include "BeNetMailIPControlView.h"
#endif

BeNetMailIPControlView::BeNetMailIPControlView(BRect frame)
	: BView(frame, "BeNetMailIPControlView", 0, B_WILL_DRAW)
{
	rgb_color color = {192, 192, 192, 0};
	SetViewColor(color);
}

void BeNetMailIPControlView::AttachedToWindow()
{

	pAcceptButton = new BeNetButton(BRect(10,10,35,34), BitmapCatalog::Instance()->GetAccept(), NULL, BENET_ACCEPT, BENET_DM_FULL_BMP);
	AddChild(pAcceptButton);
	
	pCancelButton = new BeNetButton(BRect(50,10,75,34), BitmapCatalog::Instance()->GetCancel(), NULL, BENET_CANCEL, BENET_DM_FULL_BMP);
	AddChild(pCancelButton);
	
	pDeleteButton = new BeNetButton(BRect(90,10,130,34),BitmapCatalog::Instance()->GetDelete(),NULL, BENET_DELETE, BENET_DM_FULL_BMP);
	AddChild(pDeleteButton);
		
	pPrevButton = new BeNetButton(BRect(184,10,209,34), BitmapCatalog::Instance()->GetLeftArrow(), NULL, BENET_PREV, BENET_DM_FULL_BMP);
	AddChild(pPrevButton);
	
	pNextButton = new BeNetButton(BRect(214,10,239,34), BitmapCatalog::Instance()->GetRightArrow(), NULL, BENET_NEXT, BENET_DM_FULL_BMP);
	AddChild(pNextButton);

}

void BeNetMailIPControlView::Draw(BRect updateRect)
{
	BView::Draw(updateRect);
	DrawDegrader(BRect(0,0,Bounds().Width(),8), 255, 192);
	DrawDegrader(BRect(0,Bounds().bottom - 8,Bounds().Width(),Bounds().bottom), 192, 145);
}


void BeNetMailIPControlView::DrawDegrader(BRect rect, int iStartGray, int iEndGray)
{
	int i, iGray;
	
	for(i = 0; i <= rect.Height(); i++)
	{
		iGray = iStartGray + ((iEndGray - iStartGray) / rect.Height()) * i;
		SetHighColor(iGray, iGray, iGray ,0);
		StrokeLine(BPoint(rect.left, i + rect.top), BPoint(rect.right, i + rect.top));
	}
}


void BeNetMailIPControlView::SetInfo(PEOPLE_ID_NODE* pNode)
{
	if(pNode)
	{
		if(pNode->pPrev)
		{
			pPrevButton->SetEnabled(true);
		}
		else
		{
			pPrevButton->SetEnabled(false);
		}
		if(pNode->pNext)
		{
			pNextButton->SetEnabled(true);
		}
		else
		{
			pNextButton->SetEnabled(false);
		}
	}
}
