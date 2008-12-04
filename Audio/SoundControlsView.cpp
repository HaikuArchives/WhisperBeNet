#ifndef _SoundControlsView_h
#include "SoundControlsView.h"
#endif

#ifndef _Preferences_h
#include "Preferences.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#include <interface/Window.h>
#include <locale/Locale.h>

SoundControlsView::SoundControlsView(BRect AFrame, bool bShrink)
	: ShrinkView(AFrame, (char *) _T("Sound Controls"), bShrink)
{
	rgb_color color = {192, 192, 192, 0};
	SetViewColor(color);
	float w , h;

	// Adding MicroMuteCheckBox
	m_pMuteCheckBox = new BCheckBox(BRect(AFrame.right - 50, 17, AFrame.right - 5, 0), "MuteCheckBox", _T("Mute"), new BMessage(MSG_SOUNDCONTROLS_MUTE), B_FOLLOW_RIGHT);
	// OliverESP: with localitzation it eats the strings
	m_pMuteCheckBox->GetPreferredSize( &w , &h);
	m_pMuteCheckBox->ResizeToPreferred();
	m_pMuteCheckBox->MoveTo(BPoint(AFrame.right - w - 5, 17) );
	AddChild(m_pMuteCheckBox);

	//Adding MicroSlider
	m_pSlider = new BSlider(BRect(5, 20, AFrame.right - w - 15, 0), "MicroSlider", NULL, new BMessage(MSG_SOUNDCONTROLS_VOLUME), 0, 100, B_TRIANGLE_THUMB, B_FOLLOW_RIGHT);
	m_pSlider->SetHashMarks(B_HASH_MARKS_TOP); 
	m_pSlider->SetHashMarkCount(10);
	m_pSlider->SetBarColor(kLightBlue);
	m_pSlider->UseFillColor(true, &kRed);
	AddChild(m_pSlider);


	m_pSlider->SetModificationMessage(new BMessage(MSG_SOUNDCONTROLS_VOLUME));
}


SoundControlsView::~SoundControlsView()
{
}


void
SoundControlsView::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case MSG_SOUNDCONTROLS_MUTE:
			m_pSlider->SetEnabled(!(bool)(m_pMuteCheckBox->Value() == B_CONTROL_ON));
			Window()->MessageReceived(msg);
			break;
		default:
			BView::MessageReceived(msg);
			break;
	}
}

void
SoundControlsView::AllAttached()
{
	m_pSlider->SetTarget(this);
	m_pMuteCheckBox->SetTarget(this);
}
