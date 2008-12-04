#include <locale/Locale.h>

#ifndef __INFORMATION_VIEW__
#include "InformationView.h"
#endif

#ifndef _CLIENT_H
#include "client.h"
#endif

#ifndef _Network
#include "network.h"
#endif


InformationView::InformationView(BRect i_pARect, char* i_pName)
	: BView(i_pARect, i_pName, B_FOLLOW_NONE, B_WILL_DRAW) 
{	
	rgb_color color = {192, 192, 192, 0};
	SetViewColor(color);
	
}					

InformationView::~InformationView(void)
{
}

void InformationView::AttachedToWindow(void)
{
	AddChild(m_pLastName = new BTextControl(BRect(5, 15, 195, 30), "firstname", _T("Name"), _T("unknown"), NULL, B_FOLLOW_NONE, B_WILL_DRAW));
	m_pLastName->SetDivider(60);
	m_pLastName->SetAlignment(B_ALIGN_LEFT, B_ALIGN_CENTER);
	m_pLastName->TextView()->MakeEditable(false);

	AddChild(m_pFirstName = new BTextControl(BRect(5, 35, 195, 50), "lastname", _T("Surname"), _T("unknown"),	NULL, B_FOLLOW_NONE, B_WILL_DRAW));
	m_pFirstName->SetDivider(60);
	m_pFirstName->SetAlignment(B_ALIGN_LEFT, B_ALIGN_CENTER);
	m_pFirstName->TextView()->MakeEditable(false);
		
	AddChild(m_pEmail = new BTextControl(BRect(5, 55, 195, 70), "email", _T("Email"), _T("unknown"), NULL, B_FOLLOW_NONE,	B_WILL_DRAW));
	m_pEmail->SetDivider(60);
	m_pEmail->SetAlignment(B_ALIGN_LEFT, B_ALIGN_CENTER);
	m_pEmail->TextView()->MakeEditable(false);
}

void InformationView::Draw(BRect i_pupdatedRect)
{
	DrawDegrader(BRect(0,0,Bounds().Width(),15), 255, 192);
	DrawDegrader(BRect(0,Bounds().bottom - 8,Bounds().Width(),Bounds().bottom), 192, 128);
}

void InformationView::DrawDegrader(BRect rect, int iStartGray, int iEndGray)
{
	int i, iGray;
	
	for(i = 0; i <= rect.Height(); i++)
	{
		iGray = iStartGray + ((iEndGray - iStartGray) / (int)rect.Height()) * i;
		SetHighColor(iGray, iGray, iGray ,0);
		StrokeLine(BPoint(rect.left, i + rect.top), BPoint(rect.right, i + rect.top));
	}
}

void
InformationView::Update()
{
	if (TClient::Instance()->IsOnline())
	{
		m_pLastName->SetText(TClient::Instance()->GetLastNameByIP(CNetwork::Instance()->GetRemoteIP()));
		m_pFirstName->SetText(TClient::Instance()->GetFirstNameByIP(CNetwork::Instance()->GetRemoteIP()));
		m_pEmail->SetText(TClient::Instance()->GetEmailByIP(CNetwork::Instance()->GetRemoteIP()));
	}
	else
	{
		m_pLastName->SetText(_T("unknown"));
		m_pFirstName->SetText(_T("unknown"));
		m_pEmail->SetText(_T("unknown"));
	}
}
