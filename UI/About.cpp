#ifndef _About_h
#include "About.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#ifndef _BitmapCatalog_h
#include "BitmapCatalog.h"
#endif

#ifndef _BitmapView_h
#include "BitmapView.h"
#endif

#ifndef _URLView_h
#include "URLView.h"
#endif


#include <interface/StringView.h> // experimentals...
//#include "/boot/yt/src/unstable/headers_p/interface/StringView.h"
#include <locale/Locale.h>

// BRect(100,100,220,400)
AboutWindow::AboutWindow() :
	BWindow(BRect(100,100,520,500), _T("About Whisper"), B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE | B_NOT_ZOOMABLE, B_CURRENT_WORKSPACE),
	m_height(0),
	m_animate(true)
{
	BStringView* stringView;
	BFont font(be_plain_font);

	float LEFT;
	float w , h;
	BRect b;

	font.SetSize(10.0);

	// Creating the bitmap and the new size of the window
	BBitmap* logo = BitmapCatalog::Instance()->FetchBitmap("Bitmaps/logo.bmp",false);
	ResizeTo(logo->Bounds().right - logo->Bounds().left+40, 310);

	//Placing the logo
	m_pLogoView = new BitmapView(logo->Bounds(),logo);
	m_pLogoView->MoveBy(20, 0);
	AddChild(m_pLogoView);

	LEFT = logo->Bounds().left;

	//Main view
	AddChild(m_pAboutView = new BView(Bounds(),"AboutView",0,B_WILL_DRAW));
	m_pAboutView->SetViewColor(222,223,222);


	b = Frame();
	b.bottom  = m_pLogoView->Bounds().bottom;

	b.left = LEFT + 100;
	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "BeNet Whisper ver 1.2" ));
	stringView->SetFont(be_bold_font);

	b = stringView->Frame();
	b.left = LEFT + 93;
	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "2008, an OpenSource project" ));

	b = stringView->Frame();
	b.left = LEFT + 135;
	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, _T("by")));
	stringView->GetPreferredSize( &w , &h);
	stringView->ResizeToPreferred();
	stringView->MoveTo(BPoint(LEFT + ((Bounds().right - Bounds().left) / 2) - ( w / 2 ) ,b.bottom + 1));



	b = stringView->Frame();
	b.left = LEFT + 96;
	b.right = Bounds().right;
	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "Oliver Ruiz Dorantes"));

	b = stringView->Frame();
	b.left = LEFT + 75;
	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "and Bernd Korz"));
	stringView->SetFont(be_bold_font);
	stringView->GetPreferredSize( &w , &h);
	stringView->ResizeToPreferred();
	stringView->MoveTo(BPoint(LEFT + ((Bounds().right - Bounds().left) / 2) - ( w / 2 ) ,b.bottom + 1));

	b = stringView->Frame();
	b.left = LEFT + 75;
	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, _T("Support, suggestions & bugs:") ));
	stringView->GetPreferredSize( &w , &h);
	stringView->ResizeToPreferred();
	stringView->MoveTo(BPoint(LEFT + ((Bounds().right - Bounds().left) / 2) - ( w / 2 ) ,b.bottom + 1));

//	b = stringView->Frame();
//	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "oliver.ruiz.dorantes@gmail.com" ));
	b.left = LEFT ;
	b.right = Bounds().right;
//	URLView *urlView = new URLView(BRect(b.left, b.bottom + 20, b.right, b.bottom + 36),"url","oliver.ruiz.dorantes@gmail.com","mailto: oliver.ruiz.dorantes@gmail.com");
	mailtoView *mailTo = new mailtoView(BRect(b.left, b.bottom + 20, b.right, b.bottom + 36),"oliver.ruiz.dorantes@gmail.com","Whisper: ");

//	urlView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	//urlView->SetHoverEnabled(false);
//	m_pAboutView->AddChild(urlView);
	m_pAboutView->AddChild(mailTo);

//	b = stringView->Frame();
//	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "Oliver Ruiz Dorantes"));
//	b = stringView->Frame();
//	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "Ithamar R. Adema"));
//	b = stringView->Frame();
//	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "Frank Paul Silye"));
//	b = stringView->Frame();
//	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "Oliver Hartmann"));



//	b = urlView->Frame();
	b = mailTo->Frame();
	b.top += 30;

	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.top, b.right, b.top + 15), NULL, _T("BeNet team:")));
	stringView->SetFont(be_bold_font);
	stringView->GetPreferredSize( &w , &h);
	stringView->ResizeToPreferred();
	stringView->MoveTo(BPoint(LEFT + ((Bounds().right - Bounds().left) / 2) - ( w / 2 ) ,b.top));

	b = stringView->Frame();
	b.left = LEFT + 30;
	b.right = Bounds().right;

	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "Martin Brault, Mathieu Brault, Jean-Francois Gelinas" ));
	stringView->SetFont(&font);
	b = stringView->Frame();
	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, 	"Francois Chouinard, Caroline Cote, Francis Deault"));
	stringView->SetFont(&font);
	b = stringView->Frame();
	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, 	"Sebastien Gougeon, Steve Brousseau, Hans Viens"));
	stringView->SetFont(&font);
	b = stringView->Frame();
	m_pAboutView->AddChild(stringView = new BStringView(BRect(b.left, b.bottom + 1, b.right, b.bottom + 16), NULL, "Mathieu Giguere, Patrick Henri, Etienne Robert"));
	stringView->SetFont(&font);




//	m_pAboutView->ScrollBy(0,-200);

/*	ResizeTo(AFrame.right, AFrame.bottom - 1);
	MoveTo(	Preferences::Instance()->RemoteUserX(),
			Preferences::Instance()->RemoteUserY());*/
}

AboutWindow::~AboutWindow()
{

}



bool
AboutWindow::QuitRequested()
{
	m_animate = false;
	Hide();
	return false;
}

//void
//AboutWindow::Animate()
//{
///*	m_pAboutView->ScrollBy(0,1);
//	m_pAboutView->Invalidate();	*/
//	while(m_animate) Scroll();
//}

//void
//AboutWindow::Scroll()
//{
//	Lock();
//
//	if (m_height++ >= 400)
//	{
//		m_pAboutView->ScrollBy(0,-400);
//		m_height = 0;
//	}
//
//	m_pAboutView->ScrollBy(0,1);
//	m_pAboutView->Invalidate();
//	Unlock();
//}
