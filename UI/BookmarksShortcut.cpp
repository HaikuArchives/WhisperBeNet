#include "locale/Locale.h"

#ifndef _BookmarksShortcut_h_
#include "BookmarksShortcut.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif


BookmarksShortcut::BookmarksShortcut() :
	BPopUpMenu(_T("Bookmarks"))
{
	m_pBookmarksList = Bookmarks::Instance()->Load();
	
	BMessage* v_pMsg;
	char* v_pFullName = new char[256];
		
	uint32 count = m_pBookmarksList->CountItems();
	
	for (uint32 i = 0; i < count; i++)
	{
		BookmarkItem* v_pItem = (BookmarkItem*)m_pBookmarksList->ItemAt(i);
			
		strcpy(v_pFullName, (char*)v_pItem->GetColumnContentText(1));
		strcat(v_pFullName, " ");
		strcat(v_pFullName, v_pItem->GetColumnContentText(0));	
			
		v_pMsg = new BMessage(MENU_BOOKMARKS);
//		v_pMsg->AddString("Bookmark", v_pFullName);  OliverESP: CRAZY! and with spaces???

		v_pMsg->AddString("Bookmark_name", (char*)v_pItem->GetColumnContentText(0));
		v_pMsg->AddString("Bookmark_surname", (char*)v_pItem->GetColumnContentText(1));
				
//		printf("Anhadimos %s\n",v_pFullName);
		AddItem(new BMenuItem(v_pFullName, v_pMsg));
	
	}
	
	delete v_pFullName;	
}


BookmarksShortcut::~BookmarksShortcut()
{
}
