//Useful until be implements BBitmap::BBitmap(BRect bounds, color_space space, const void *data, 
//										 	  bool acceptsViews, bool needsContiguousMemory)
//or something like it...


#ifndef _SGB_PREFILLED_BITMAP_H_
#define _SGB_PREFILLED_BITMAP_H_


//******************************************************************************************************
//**** SYSTEM HEADER FILES
//******************************************************************************************************
#include <Bitmap.h>


//******************************************************************************************************
//**** CLASS DECLARATIONS
//******************************************************************************************************
class PrefilledBitmap : public BBitmap
{
	public:
		PrefilledBitmap(BRect bounds, color_space space, const void *data, int32 length = 0,
			bool acceptsViews = false, bool needsContiguousMemory = false);
			//length can be automatically calculated for B_CMAP8 and B_RGB32
		~PrefilledBitmap();
};

#endif
