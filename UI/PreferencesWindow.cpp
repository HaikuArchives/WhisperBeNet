#ifndef _PreferencesWindow_h
#include "PreferencesWindow.h"
#endif

#ifndef _PreferencesViews_h
#include "PreferencesViews.h"
#endif

#ifndef Preferences_h
#include "Preferences.h"
#endif

//#include <experimental/TabView.h>
#include <interface/TabView.h>
#include <locale/Locale.h>

PreferencesWindow::PreferencesWindow() 
	: BWindow(BRect(0,0,280,175), _T("Preferences"), B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	BTab *tab; 

	BRect r = Bounds(); 
	BTabView* tabView = new BTabView(r, "tab_view"); 
	tabView->SetViewColor(216,216,216,0); 
	r = tabView->Bounds(); 
	r.bottom -= tabView->TabHeight(); 

	tab = new BTab(); 
	tabView->AddTab(new GeneralPreferencesView(r), tab); 
	tab->SetLabel(_T("General")); 

	tab = new BTab(); 
	tabView->AddTab(new AudioPreferencesView(r), tab); 
	tab->SetLabel(_T("Audio"));

	tab = new BTab(); 
	tabView->AddTab(new NetworkPreferencesView(r), tab); 
	tab->SetLabel(_T("Network"));

	tab = new BTab(); 
	tabView->AddTab(new IdentityPreferencesView(r), tab); 
	tab->SetLabel(_T("Identity"));

	AddChild(tabView);
	tabView->SetTabWidth(B_WIDTH_FROM_WIDEST);

	MoveTo(	Preferences::Instance()->PreferencesX(),
			Preferences::Instance()->PreferencesY());
}



bool
PreferencesWindow::QuitRequested(void)
{
	Hide();
	return false;
}


void
PreferencesWindow::FrameMoved(BPoint point)
{
	Preferences::Instance()->PreferencesX(point.x);
	Preferences::Instance()->PreferencesY(point.y);
}
