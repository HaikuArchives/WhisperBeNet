#ifndef _SoundControlsView_h
#define _SoundControlsView_h

#ifndef __SHRINK_VIEW__
#include "ShrinkView.h"
#endif

#include <View.h>
#include <Slider.h>
#include <CheckBox.h>
#include <Message.h>

const rgb_color kLightBlue	= {80, 140, 255, 255};
const rgb_color	kRed		= {255, 0, 0, 255};

// View object
class SoundControlsView : public ShrinkView
{
public:
	SoundControlsView(BRect AFrame, bool bShrink = false);
	~SoundControlsView();
	virtual void	MessageReceived(BMessage* msg);
	virtual void	AllAttached();

	bool			IsMute()				{return ((bool)(m_pMuteCheckBox->Value() == B_CONTROL_ON));}
	int32			Volume()				{return m_pSlider->Value();}
	void			Volume(int32 value)		{m_pSlider->SetValue(value);}

private:
	BSlider*		m_pSlider;
	BCheckBox*		m_pMuteCheckBox;
};

#endif	// _SoundControlsView_h
