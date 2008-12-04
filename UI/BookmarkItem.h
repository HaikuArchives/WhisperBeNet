#ifndef _BookmarkItem_h_
#define _BookmarkItem_h_

#include <string.h>

#include "CLVEasyItem.h"

//*****************************************************************************************************//
//	CLASS BookmarkItem
//*****************************************************************************************************//


class BookmarkItem : public CLVEasyItem 
{

//========================================  MODULE PROTOTYPE  =========================================//
	public:
		BookmarkItem(uint32 level = 0, bool superitem = 0, bool expanded = 0,
		 			 const char* name = NULL, const char* first_name = NULL, const char* ip_adress = NULL);
		~BookmarkItem();

		static char* 			toString(BookmarkItem*);
		static BookmarkItem* 	fromString(char*); 	
		
};


#endif
