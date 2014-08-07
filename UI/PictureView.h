#ifndef _PictureView_h
#define _PictureView_h

#ifndef __SHRINK_VIEW__
#include "ShrinkView.h"
#endif


class PictureView : public ShrinkView
{
	public :
		PictureView(BRect i_frame);
		~PictureView();
		
		virtual void AttachedToWindow();
				
	private : 
		BBitmap*	m_pBitmapPicture;

};

#endif // _PictureView
