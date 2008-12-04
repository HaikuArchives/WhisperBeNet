#ifndef _Bookmarks_h_
#include "Bookmarks.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif


// singleton instanciation
Bookmarks* Bookmarks::m_pInstance = 0;

Bookmarks::Bookmarks()
{
	//BeNet directory creation for bookmark.dat
	find_directory(B_USER_SETTINGS_DIRECTORY, &m_vPath);
   	m_vPath.Append(BOOKMARKS_FOLDER);
   	m_vPath.Append(BOOKMARKS_FILE);
	
}

Bookmarks::~Bookmarks()
{
	
}

Bookmarks*
Bookmarks::Instance()
{
	if(m_pInstance == 0)
		m_pInstance = new Bookmarks();
		
	return m_pInstance;
}

void
Bookmarks::Save(ColumnListView* i_pBookmarksList)
{
	BookmarkItem* 	v_pItemToBeProceed;

	FILE *m_pBookmarkFile = fopen(m_vPath.Path(), "w");
	
	if(m_pBookmarkFile)
	{
		uint32 v_cItemCount = i_pBookmarksList->CountItems();

		for(uint32 v_iItemIndex = 0; v_iItemIndex < v_cItemCount; v_iItemIndex++)
		{
			v_pItemToBeProceed = dynamic_cast <BookmarkItem*>
									(i_pBookmarksList->ItemAt(v_iItemIndex));

			char *out = BookmarkItem::toString(v_pItemToBeProceed);
			fwrite(out, sizeof(char), strlen(out), m_pBookmarkFile);
			delete out;
		}
	}	

	fclose(m_pBookmarkFile);
}

BList*
Bookmarks::Load()
{
	FILE *m_pBookmarkFile = fopen(m_vPath.Path(), "r");
	
	m_pBookmarksList = new BList();
	
	if(m_pBookmarkFile)
	{
		char c = ' ';
		int size = 256;
		char *line = new char[size];
		int no_char = 0;
		
		while(c != EOF){
			while(c != '\n' && c != EOF){
				c = (char)fgetc(m_pBookmarkFile);
				if(no_char >= size){
					char *tmp = new char[size*2];
					strcpy(tmp, line);
					line = tmp;
				}
				line[no_char++] = c;
			}
			if(c != EOF){
				c = ' ';
				no_char = 0;
				BookmarkItem *item = BookmarkItem::fromString(line);
				m_pBookmarksList->AddItem(item);
			}			
		}			
	}
	
	fclose(m_pBookmarkFile);

	return m_pBookmarksList;

}

BookmarkItem* Bookmarks::FindBookmarkItem(char* name,char* surname)
{

//OliverESP: changed header cos of spaces we cannot trust in this!
//	char *first_name = strtok(i_pLabel, " ");
//	char *name = strtok(NULL, " ");

	bool isFound = false;
	
	BookmarkItem *item = new BookmarkItem;
	
	FILE *m_pBookmarkFile = fopen(m_vPath.Path(), "r");
	m_pBookmarksList = new BList();
	
	if(m_pBookmarkFile)
	{		
		char c = ' ';
		int size = 256;
		char *line = new char[size];
		int no_char = 0;
		
		
		while(c != EOF && !isFound)
		{
			while(c != '\n' && c != EOF)
			{
				c = (char)fgetc(m_pBookmarkFile);
				
				
				if(no_char >= size)
				{
					char *tmp = new char[size*2];
					strcpy(tmp, line);
					line = tmp;
				}

				line[no_char++] = c;
			}
		
			if(c != EOF)
			{
				c = ' ';
				no_char = 0;
								
				item = BookmarkItem::fromString(line);
// printf("line: %s looking for %s %s \n",line,surname,name);
				if (!strcmp(item->GetColumnContentText(0), name) &&
					!strcmp(item->GetColumnContentText(1), surname))						
					{
// printf("found: %s\n",line);
						isFound = true;
					}
			}			
		}			
	}
// printf("\n");	
	fclose(m_pBookmarkFile);

	return item;
}
