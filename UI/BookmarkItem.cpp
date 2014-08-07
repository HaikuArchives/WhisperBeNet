#ifndef _BookmarkItem_h_
#include "BookmarkItem.h"
#endif

//*****************************************************************************************************//
//	CLASS BookmarksItem
//*****************************************************************************************************//

BookmarkItem::BookmarkItem(uint32 level, bool superitem, bool expanded, const char* name,
							const char* first_name, const char* email_adress)
				: CLVEasyItem(level, superitem, expanded, 20.0)
{
	SetColumnContent(0,name);
	SetColumnContent(1,first_name);
	SetColumnContent(2,email_adress);
	
}


BookmarkItem::~BookmarkItem()
{ 
}

char*
BookmarkItem::toString(BookmarkItem* item)
{ 
	char *result = new char[strlen(item->GetColumnContentText(0)) +
							strlen(item->GetColumnContentText(1)) +
							strlen(item->GetColumnContentText(2)) + 5];
	result = strcpy(result, item->GetColumnContentText(0));
	result = strcat(result, "|");
	result = strcat(result, item->GetColumnContentText(1));
	result = strcat(result,  "|");
	result = strcat(result, item->GetColumnContentText(2));
	result = strcat(result, "\n");
	return result;
}

BookmarkItem*
BookmarkItem::fromString(char* string)
{
	char *name = strtok(string, "|");
	char *first_name = strtok(NULL, "|");
	char *email = strtok(NULL, "\n");
	return new BookmarkItem(0, false, false, name, first_name, email);
}
