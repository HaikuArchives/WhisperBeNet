#ifndef _Bookmarks_h_
#define _Bookmarks_h_

#ifndef _BookmarkItem_h_
 #include "BookmarkItem.h"
#endif

#include "ColumnListView.h"

#include <FindDirectory.h>
#include <Directory.h>
#include <Path.h>

#include <stdio.h>

/*Fichier des bookmark*/


class Bookmarks
{
	public:
		static Bookmarks* 	Instance();
		~Bookmarks();
		
		void 				Save(ColumnListView* i_pBookmarksList);
		BList* 				Load();
		BookmarkItem*		FindBookmarkItem(char* name,char* surname);
		
	private:
		Bookmarks();
	
		static Bookmarks* 	m_pInstance;
		BList*				m_pBookmarksList;
		BDirectory			m_vDirectory;	
		BPath				m_vPath;

};

#endif
