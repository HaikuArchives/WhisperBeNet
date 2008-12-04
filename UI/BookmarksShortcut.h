#ifndef _BookmarksShortcut_h_
#define _BookmarksShortcut_h_

#include <PopUpMenu.h>
#include <MenuItem.h>
#include <Menu.h>
#include <MenuField.h>

#ifndef _Bookmarks_h_
 #include "Bookmarks.h"
#endif

#include "BookmarkItem.h"



class BookmarksShortcut : public BPopUpMenu
{
	public :
		BookmarksShortcut();
		~BookmarksShortcut();
		
		
	private: 
		BList*	m_pBookmarksList;



};

#endif
