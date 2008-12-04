#ifndef _SGB_BETTER_SCROLL_VIEW_H_
#define _SGB_BETTER_SCROLL_VIEW_H_

#include <ScrollView.h>

#include "ScrollViewCorner.h"


//******************************************************************************************************
//	CLASS BetterScrollView
//******************************************************************************************************
class BetterScrollView : public BScrollView
{
	public:
		BetterScrollView(const char *name, BView *target, uint32 resizeMask = B_FOLLOW_LEFT | B_FOLLOW_TOP,
			uint32 flags = B_FRAME_EVENTS | B_WILL_DRAW, bool horizontal = true, bool vertical = true,
			bool scroll_view_corner = true, border_style border = B_FANCY_BORDER);
		~BetterScrollView();
		
		virtual void 	SetDataRect(BRect data_rect, bool scrolling_allowed = true);
		virtual	void 	FrameResized(float new_width, float new_height);
		void 			UpdateScrollBars(bool scrolling_allowed);
		BRect 			GetDataRect();

	private:
		BView*				m_target;
		BRect 				m_data_rect;
		BScrollBar*			m_h_scrollbar;
		BScrollBar* 		m_v_scrollbar;
		ScrollViewCorner* 	m_scroll_view_corner;
		
};


#endif
