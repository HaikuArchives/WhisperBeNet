#ifndef _BookmarksWindow_H
#define _BookmarksWindow_H

#ifndef _BookmarkItem_h_
#include "BookmarkItem.h"
#endif

#ifndef _Bookmarks_h_
#include "Bookmarks.h"
#endif

#ifndef _Def_h
#include "Def.h"
#endif

#ifndef _ConnectView_h
#include "ConnectView.h"
#endif

#include <View.h>
#include <Window.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <Button.h>
#include <TextControl.h>

#include <stdio.h>
#include <string.h>

#include "ColumnListView.h"
#include "CLVEasyItem.h"
#include "BetterScrollView.h"
#include "PostDispatchInvoker.h"




//========================================   CLASS DEFINITIONS   ======================================//

class BookmarkItemWindow;
class BookmarkItemView;
class BookmarkItem;

//*****************************************************************************************************//
//	CLASS BookmarksWindow
//*****************************************************************************************************//

class BookmarksWindow: public BWindow
{

//========================================   MODULE PROTOTYPES   ======================================//
	public: 
		BookmarksWindow(ConnectView* i_pConnectView);
		~BookmarksWindow(void);
		virtual void MessageReceived(BMessage* message);
		virtual bool QuitRequested(void);
		virtual void FrameMoved(BPoint point);
		virtual void FrameResized(float width, float height);

	private: 

//		void	AddBookmarkItem(BMessage* i_pMessage);
		void	AddBookmarkItem();
//		void	EditBookmarkItem(BMessage* i_pMessage);
		void	EditBookmarkItem();
//		void	RemoveBookmarkItem(BMessage* i_pMessage);
		void	RemoveBookmarkItem();
//		void	BookmarkSelectionChanged(BMessage* i_pMessage);
		void	BookmarkSelectionChanged();
		void	ShowEditBookmarkItemWindow(BookmarkItem *i_pSelectedItem);
		void	ShowNewBookmarkItemWindow();
				
//======================================   VARIABLES DEFINITIONS   ====================================//	
	private:
		
		BookmarkItemWindow*	m_pBookmarkItemWindow;
		BMenu*				m_pBookmarksMenu;
		CLVListItem*		m_pNewCLVListItem;
		ColumnListView*		m_pColumnListView;
		BetterScrollView*	m_pContainerView;
		BookmarkItem*		m_pSelectedItem;
		int32				m_vSelectedItemIdx;
		ConnectView*		m_pConnectView;
};

//*****************************************************************************************************//
//	CLASS BookmarksMenu
//*****************************************************************************************************//
class BookmarksMenu: public BMenuBar
{

//========================================   MODULE PROTOTYPES   ======================================//
	public: 
		BookmarksMenu(BRect AFrame);
		~BookmarksMenu(void);
		
		virtual void AttachedToWindow(void);
		virtual void MessageReceived(BMessage *message);
		
			
//======================================   VARIABLES DEFINITIONS   ====================================//	
	private :

		BMenu 		*m_pEdit;
		BMenu 		*m_pFile;
		BMenuItem 	*m_pRemoveBookmarkItem;
		BMenuItem 	*m_pSaveBookmarksItem;
		BMenuItem 	*m_pEditBookmarkItem;
		BMenuItem 	*m_pNewBookmarkItem;
		
};


//*****************************************************************************************************//
//	CLASS BookmarkItemWindow
//*****************************************************************************************************//

class BookmarkItemWindow : public BWindow 
{

//========================================  MODULE PROTOTYPE  =========================================//
	public:
		BookmarkItemWindow(char* i_pTitle, BRect i_AFrame, BookmarksWindow* i_pBookmarksWindow = NULL,
							ColumnListView* i_pColumnListView = NULL, BookmarkItem* i_pSelectedItem = NULL);
		~BookmarkItemWindow(void);
		
		BookmarkItem*	SendBookmarkItem(void);
		virtual void 	MessageReceived(BMessage* i_pMessage);
		virtual bool 	QuitRequested(void);
//		virtual void 	OkRequested(BMessage* i_pMessage = NULL) = 0;
		virtual void 	OkRequested(void) = 0;
		
//======================================  VARIABLES DEFINITION  =========================================//
	protected:
		BookmarksWindow*	m_pBookmarksWindow;
	
	
	private:
		BookmarkItemView*	m_pBookmarkItemView;	
		

};

//*****************************************************************************************************//
//	CLASS EditBookmarkItemWindow
//*****************************************************************************************************//

class EditBookmarkItemWindow : public BookmarkItemWindow 
{

//========================================  MODULE PROTOTYPE  =========================================//
	public:		
		EditBookmarkItemWindow(char* i_pTitle, BRect i_AFrame, BookmarksWindow* i_pBookmarksWindow,
							ColumnListView* i_pColumnListView, BookmarkItem *i_pSelectedItem);
		~EditBookmarkItemWindow(void);

//		virtual void 	OkRequested(BMessage* i_pMessage);
		virtual void 	OkRequested(void);

	private:
		BookmarkItem* 	m_pSelectedItem;
		ColumnListView* m_pColumnListView;
};

//*****************************************************************************************************//
//	CLASS NewBookmarkItemWindow
//*****************************************************************************************************//

class NewBookmarkItemWindow : public BookmarkItemWindow 
{

//========================================  MODULE PROTOTYPE  =========================================//
	public:
		NewBookmarkItemWindow(char* i_pTitle, BRect i_AFrame, BookmarksWindow* i_pBookmarksWindow = NULL);
		~NewBookmarkItemWindow(void);

//		virtual void 	OkRequested(BMessage* i_pMessage);
		virtual void 	OkRequested(void);
};



//*****************************************************************************************************//
//	CLASS BookmarkItemView
//*****************************************************************************************************//

class BookmarkItemView: public BView
{

//========================================  MODULE PROTOTYPE  =========================================//
	public:
		
		BookmarkItemView(BRect AFrame, BookmarkItem* i_pSelectedItem = NULL);//char* i_pName = NULL, char* i_pFirstName = NULL, char* i_pEmail = NULL);
		~BookmarkItemView(void);
		virtual void	MessageReceived(BMessage *i_pMessage);
		virtual void 	AttachedToWindow(void);

		const char*		SendTextControlString(E_TEXT_CONTROL_FLAG i_flag); 	
	
	private:
		void 	CheckTextControl(void);


//======================================  VARIABLES DEFINITION  =========================================//
	private:
		BButton*					m_pCancelButton;
		BButton*					m_pOkButton;
		BTextControl*				m_pNameTextControl;
		BTextControl*				m_pFirstNameTextControl;
		BTextControl*				m_pEmailTextControl;

};

#endif
