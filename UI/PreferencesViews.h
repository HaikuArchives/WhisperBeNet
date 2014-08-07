
#ifndef _PreferencesViews_h
#define _PreferencesViews_h

#include <View.h>
#include <Button.h>
#include <Rect.h>
#include <CheckBox.h>
#include <RadioButton.h>
#include <Slider.h>
#include <Message.h>
#include <TextControl.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <Alert.h>
#include <MenuItem.h>

#include <MediaAddOn.h>
#include <MediaRoster.h>

class GeneralPreferencesView : public BView
{
public:
	GeneralPreferencesView(BRect frame);
	virtual void	AllAttached();
	virtual void	MessageReceived(BMessage* msg);

private:
	BCheckBox*		m_shrinkOscilloscope;
	BCheckBox*		m_shrinkSoundControls;
	BCheckBox*		m_shrinkConnection;
	BCheckBox*		m_bookmarksWindow;
	BCheckBox*		m_outputWindow;
	BButton*		m_pApply;
};


class AudioPreferencesView : public BView
{
public:
	AudioPreferencesView(BRect frame);
	virtual void	AllAttached();
	virtual void	MessageReceived(BMessage* msg);
	void			CreateInputs();
private:
//	BCheckBox*		m_useCodec;
//	BCheckBox*		m_useModemQuality;

	BRadioButton*	m_useCodec;
	BRadioButton*	m_useModemQuality;
	BRadioButton*	m_custom;
	BSlider*		m_slider_compression; // OliverESP
	BPopUpMenu*		mInputs;
	BMenuField*		m_inputs;
};


class NetworkPreferencesView : public BView
{
public:
	NetworkPreferencesView(BRect frame);
	virtual void	AllAttached();
	virtual void	MessageReceived(BMessage* msg);

private:
	BCheckBox*		m_pGoOnlineOnStartup;
	BTextControl*	m_connectionTimeout;
	BTextControl*	m_pServerIP;
	BTextControl*	m_pServerPort;
};


class IdentityPreferencesView : public BView
{
public:
	IdentityPreferencesView(BRect frame);
	virtual void	AllAttached();
	virtual void	MessageReceived(BMessage* msg);

private:
	BTextControl*	m_pFirstName;
	BTextControl*	m_pLastName;
	BTextControl*	m_pEmail;
};

#endif
