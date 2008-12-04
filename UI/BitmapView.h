#ifndef BITMAP_VIEW_H
#define BITMAP_VIEW_H

#include <View.h>

class Bitmap;
class BRect;

// A BView derived class that holds  and displays a BBitmap.
class BitmapView : public BView
{
	public:
		BitmapView(BRect frame, BBitmap *bmap);
		~BitmapView();
		void Draw(BRect updateRect);
		void AttachedToWindow();
		
	private:
		BBitmap	*themap;
		BRect bmbounds;
};

#endif
