#ifndef _SGB_SCROLL_VIEW_CORNER_H_
#define _SGB_SCROLL_VIEW_CORNER_H_


//******************************************************************************************************
//**** SYSTEM HEADER FILES
//******************************************************************************************************
#include <View.h>


//******************************************************************************************************
//	CLASS ScrollViewCorner
//******************************************************************************************************
//	ABSTRACT & PURPOSE :
//
//	If you have a BScrollView with horizontal and vertical sliders that isn't 
//	seated to the lower-right corner of a B_DOCUMENT_WINDOW, there's a "hole"
//	between the sliders that needs to be filled.  You can use this to fill it.
//	In general, it looks best to set the ScrollViewCorner color to 
//	BeInactiveControlGrey if the vertical BScrollBar is inactive, and the color
//	to BeBackgroundGrey if the vertical BScrollBar is active.  
//
//******************************************************************************************************
class ScrollViewCorner : public BView
{
	public:
		ScrollViewCorner(float Left,float Top);
		~ScrollViewCorner();
		void Draw(BRect Update);
};

#endif
