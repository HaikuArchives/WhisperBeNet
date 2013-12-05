#include "locale/Locale.h"

#ifndef _PictureView_h
#include "PictureView.h"
#endif


PictureView::PictureView(BRect i_frame)
		: ShrinkView(i_frame, _T("Picture"), true)
{
	m_pBitmapPicture = NULL;
}

PictureView::~PictureView()
{

}

void PictureView::AttachedToWindow()
{

}

