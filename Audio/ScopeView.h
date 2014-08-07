#ifndef _ScopeView_h
#define _ScopeView_h

#ifndef __SHRINK_VIEW__
#include "ShrinkView.h"
#endif

#include <Bitmap.h>
#include <Message.h>

// Dimensions
#define SCOPE_WIDTH				120	// Scope grid parameters
#define SCOPE_HEIGHT			96
#define WINDOW_WIDTH			CHECKBOX_LENGTH

// ScopeView
class ScopeView : public ShrinkView
{
public:
	ScopeView(BRect frame, bool bShrink = false);

	void			NewData(int16* data, bool non_null_frame);
	void			FillBitmap(int16 *buf, uint8 color);
	void			Mute();
	virtual void	AttachedToWindow();
	virtual void	FrameResized(float w, float h);
public:
	BBitmap*		m_bitmap;
	float			m_w;
	float			m_h;
	uint8			m_back_color;
	uint8			m_mute_color;
	uint8			m_beam_color;
	uint8			m_null_color; //OliverESP
};

#endif	// _ScopeView_h
