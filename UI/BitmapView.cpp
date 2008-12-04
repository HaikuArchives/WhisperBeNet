#include <Debug.h>
#include "BitmapView.h"
#include <Bitmap.h>
#include <Window.h>


BitmapView::BitmapView( BRect frame, BBitmap *bmap )
			   : BView( frame, "bitmap_view", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE )
{
	SetViewColor( 216, 216, 216 );
	themap = bmap;
	bmbounds = themap->Bounds();
}


BitmapView::~BitmapView() {
	delete themap;
}


void BitmapView::AttachedToWindow() {
//	if( Parent() )
//		SetViewColor( Parent()->ViewColor() );
//	else
		SetViewColor( 216, 216, 216 );
}


void BitmapView::Draw( BRect updateRect ) {

	// calculate the offsets to center the bitmap
	float LOffset = (Bounds().Width() - bmbounds.Width()) / 2;
	float TOffset = (Bounds().Height() - bmbounds.Height()) / 2;	

	// Make a new rect w/ those coordinates
	BRect drawRect( LOffset, TOffset, Bounds().Width() - LOffset, Bounds().Height() - TOffset );

	if ( Window()->Lock() )
	{
		DrawBitmap( themap, drawRect );
		Window()->Unlock();
	}
}
