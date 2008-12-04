#ifndef _BookmarksWindow_H
#include "BookmarksWindow.h"
#endif

#ifndef _Preferences_h
#include "Preferences.h"
#endif

#include <Alert.h>
#include <locale/Locale.h>

#include "CLVEasyItem.h"



#define NAME_COLUMN_WIDTH	120
#define EMAIL_COLUMN_WIDTH	150

//========================================  MODULES DEFINITIONS  =====================================//

//*****************************************************************************************************//
//	CLASS BookmarksWindow
//*****************************************************************************************************//


BookmarksWindow::BookmarksWindow(ConnectView* i_pConnectView) :
	BWindow(BRect(0,0,460,300), _T("Contacts"), B_DOCUMENT_WINDOW, 0),
	m_pConnectView(i_pConnectView)
{
	float v_minWidth, v_maxWidth;
	float v_minHeight, 	v_maxHeight;
	float v_menuHeight;
	

	//Setting size limits for BookmarksWindow
	BRect AFrame = Bounds();
	v_minWidth = AFrame.right - AFrame.left;
	v_maxWidth = v_minWidth * 5;
	v_minHeight = AFrame.bottom - AFrame.top;
	v_maxHeight = v_minHeight * 5;
	
	SetSizeLimits(v_minWidth, v_maxWidth, v_minHeight, v_maxHeight);
	
	//Adding MenuBar to BookmarksWindow
	BRect RectBookmarksMenu(0.000000, 0.000000, -1.000000, 20.000000);
	m_pBookmarksMenu = new BookmarksMenu(RectBookmarksMenu);
	AddChild(m_pBookmarksMenu);
	
	//Setting ColumnListView Rect
	RectBookmarksMenu = m_pBookmarksMenu->Bounds();
	v_menuHeight = RectBookmarksMenu.bottom - RectBookmarksMenu.top;
		
	BRect ARectView = Bounds();
	ARectView.top += v_menuHeight;
	ARectView.right -= B_V_SCROLL_BAR_WIDTH;
	ARectView.bottom -= B_H_SCROLL_BAR_HEIGHT;					
	
	//Creating ColumnListView
	m_pColumnListView = new ColumnListView(	ARectView,
											&m_pContainerView,
											"BookmarksColumnListView",
											B_FOLLOW_ALL_SIDES,
											B_WILL_DRAW|B_FRAME_EVENTS|B_NAVIGABLE,
											B_SINGLE_SELECTION_LIST,
											true,
											true,
											true,
											true,
											B_NO_BORDER);
											
	m_pColumnListView->SetSelectionMessage(new BMessage(MSG_SELECTION_CHANGED));
	AddChild(m_pContainerView);
	
	//Adding default columns to ColumnListView
	m_pColumnListView->AddColumn(new CLVColumn(_T("Name"), NAME_COLUMN_WIDTH,
												 CLV_SORT_KEYABLE | CLV_HEADER_TRUNCATE | CLV_TELL_ITEMS_WIDTH, 50));
	m_pColumnListView->AddColumn(new CLVColumn(_T("Surname"), NAME_COLUMN_WIDTH,
												 CLV_SORT_KEYABLE | CLV_HEADER_TRUNCATE | CLV_TELL_ITEMS_WIDTH, 50));
	m_pColumnListView->AddColumn(new CLVColumn(_T("Email"), EMAIL_COLUMN_WIDTH,
												 CLV_SORT_KEYABLE | CLV_HEADER_TRUNCATE | CLV_TELL_ITEMS_WIDTH));
	
	m_pColumnListView->SetSortFunction(CLVEasyItem::CompareItems);

	// Load the bookmarks
	m_pColumnListView->AddList(Bookmarks::Instance()->Load());

	// Restore window position and size according to the preferences
	MoveTo(	Preferences::Instance()->BookmarksX(),
			Preferences::Instance()->BookmarksY());
	ResizeTo(	Preferences::Instance()->BookmarksW(),
				Preferences::Instance()->BookmarksH());
}

BookmarksWindow::~BookmarksWindow(void)
{
}

bool
BookmarksWindow::QuitRequested(void)
{
	Hide();
	return false;
}

void
BookmarksWindow::FrameMoved(BPoint point)
{
	Preferences::Instance()->BookmarksX(point.x);
	Preferences::Instance()->BookmarksY(point.y);
}

void
BookmarksWindow::FrameResized(float width, float height)
{
	Preferences::Instance()->BookmarksW(width);
	Preferences::Instance()->BookmarksH(height);
}


//========================================   MESSAGES HANDLERS   ========================================//

void BookmarksWindow::MessageReceived(BMessage* i_pMessage)
{
	switch (i_pMessage->what)
	{
	case MSG_MENU_NEW:
		ShowNewBookmarkItemWindow();
		break;
	case MSG_MENU_EDIT:
		ShowEditBookmarkItemWindow(m_pSelectedItem);
		break;
	case MSG_MENU_REMOVE:
//		RemoveBookmarkItem(i_pMessage);
		RemoveBookmarkItem();
		break;
	case MSG_MENU_SAVE:
		Bookmarks::Instance()->Save(m_pColumnListView);
			
			//Delivering message to ConnectView
			//OliverESP: hmmmm less lines and dont need to use brackets
			
			// BMessenger* v_pMessenger = new BMessenger(m_pConnectView); OliverESP: no lo se porque
			// v_pMessenger->SendMessage(new BMessage(MSG_MENU_SAVE));
		BMessenger(m_pConnectView).SendMessage(new BMessage(MSG_MENU_SAVE));
		break;	
	case MSG_SELECTION_CHANGED:
//		BookmarkSelectionChanged(i_pMessage);   // parameter not used
		BookmarkSelectionChanged(); 			// parameter not used
		break;
	case MSG_EDIT_REQUESTED:
//		EditBookmarkItem(i_pMessage);
		EditBookmarkItem();
		break;
	case MSG_ADD_REQUESTED:
//		AddBookmarkItem(i_pMessage);
		AddBookmarkItem();		
		break;		
	default:
		BWindow::MessageReceived(i_pMessage);
		break;
	}
}

void BookmarksWindow::ShowEditBookmarkItemWindow(BookmarkItem *i_pSelectedItem)
{
	BRect ARectBookmarkItemWindow(180, 230, 480, 390);
	m_pBookmarkItemWindow = new EditBookmarkItemWindow( (char*) _T("Edit Bookmark"), ARectBookmarkItemWindow, this,
														m_pColumnListView, i_pSelectedItem);
	m_pBookmarkItemWindow->SetFlags(B_NOT_RESIZABLE);	
	m_pBookmarkItemWindow->Show();
}

void BookmarksWindow::ShowNewBookmarkItemWindow()
{
	BRect ARectBookmarkItemWindow(180, 230, 480, 390);
	m_pBookmarkItemWindow = new NewBookmarkItemWindow((char*) _T("New Bookmark"), ARectBookmarkItemWindow, this);
													
	m_pBookmarkItemWindow->SetFlags(B_NOT_RESIZABLE);	

	m_pBookmarkItemWindow->Show();
	
}


//void BookmarksWindow::RemoveBookmarkItem(BMessage* i_pMessage)
void BookmarksWindow::RemoveBookmarkItem()
{
	
	m_pColumnListView->RemoveItem(m_pSelectedItem); 
	
	delete m_pSelectedItem; // deletes item	
	
	m_pBookmarksMenu->FindItem(MSG_MENU_SAVE)->SetEnabled(true);

	//See if ColumnListView is empty
	if(!(m_pColumnListView->CountItems()))
	{
		//ColumnListView is empty
		m_pBookmarksMenu->FindItem(MSG_MENU_REMOVE)->SetEnabled(false);
		m_pBookmarksMenu->FindItem(MSG_MENU_EDIT)->SetEnabled(false);
	}
		
	
}

//void BookmarksWindow::BookmarkSelectionChanged(BMessage* i_pMessage)
void BookmarksWindow::BookmarkSelectionChanged()
{
	bool 	v_bookmarkIsSelected;
	
	//See if an item in the outline list is selected
	m_vSelectedItemIdx = m_pColumnListView->CurrentSelection();
	v_bookmarkIsSelected = (m_vSelectedItemIdx >= 0);
	
	m_pSelectedItem = dynamic_cast <BookmarkItem*>
							(m_pColumnListView->ItemAt(m_vSelectedItemIdx));
	
	if(v_bookmarkIsSelected)
	{
		m_pBookmarksMenu->FindItem(MSG_MENU_REMOVE)->SetEnabled(true);
		m_pBookmarksMenu->FindItem(MSG_MENU_EDIT)->SetEnabled(true);
	}
	else
	{
		m_pBookmarksMenu->FindItem(MSG_MENU_REMOVE)->SetEnabled(false);
		m_pBookmarksMenu->FindItem(MSG_MENU_EDIT)->SetEnabled(false);	
	}
}

//void BookmarksWindow::AddBookmarkItem(BMessage* i_pMessage)
void BookmarksWindow::AddBookmarkItem()
{
	uint32 			v_cItemCount;
	uint32 			v_iItemIndex;
	bool 			v_bItemFound;
	BookmarkItem* 	v_pNewItem;
	BookmarkItem* 	v_pChekedItem;
	
	v_cItemCount = m_pColumnListView->CountItems();
	v_pNewItem = m_pBookmarkItemWindow->SendBookmarkItem();	
		
	if(v_cItemCount > 0)
	{
		v_bItemFound = 0;
		
		for(v_iItemIndex = 0; v_iItemIndex < v_cItemCount && !v_bItemFound; v_iItemIndex++)
		{
			v_pChekedItem = dynamic_cast <BookmarkItem*>
								(m_pColumnListView->ItemAt(v_iItemIndex));
								
			if(	!strcmp(v_pChekedItem->GetColumnContentText(0), v_pNewItem->GetColumnContentText(0)) &&
				!strcmp(v_pChekedItem->GetColumnContentText(1), v_pNewItem->GetColumnContentText(1)) &&
				!strcmp(v_pChekedItem->GetColumnContentText(2), v_pNewItem->GetColumnContentText(2)))
					v_bItemFound = 1;
		}
		
	}
	
	if(!v_cItemCount || !v_bItemFound) 
	{
		m_pColumnListView->AddItem(v_pNewItem);

		//ColumnListView contains at least 1 item
		m_pBookmarksMenu->FindItem(MSG_MENU_SAVE)->SetEnabled(true);
	}
	
	
	
	
	
	m_pBookmarkItemWindow->Lock();
	m_pBookmarkItemWindow->Quit();
		
}

//void BookmarksWindow::EditBookmarkItem(BMessage* i_pMessage)
void BookmarksWindow::EditBookmarkItem()
{
	m_pColumnListView->RemoveItem(m_pSelectedItem);
	m_pColumnListView->AddItem(m_pBookmarkItemWindow->SendBookmarkItem(), m_vSelectedItemIdx);

	m_pBookmarksMenu->FindItem(MSG_MENU_SAVE)->SetEnabled(true);
	
	m_pBookmarkItemWindow->Lock();
	m_pBookmarkItemWindow->Quit();
		
}


//*****************************************************************************************************//
//	CLASS BookmarksMenu
//*****************************************************************************************************//


BookmarksMenu::BookmarksMenu(BRect AFrame)
				: BMenuBar(AFrame,"BookmarksMenuBar")
{
	//"File"
	m_pFile = new BMenu(_T("File"));
	AddItem(m_pFile);
	m_pFile->SetTargetForItems(this);
	
	//Adding "New Bookmark" item to "File" menu
	m_pNewBookmarkItem = new BMenuItem(_T("New Bookmark"), new BMessage(MSG_MENU_NEW), 'N');
	m_pFile->AddItem(m_pNewBookmarkItem);
	
	//Adding "Edit Bookmark" item to "File" menu
	m_pEditBookmarkItem = new BMenuItem(_T("Edit Bookmark"), new BMessage(MSG_MENU_EDIT), 'E');
	m_pFile->AddItem(m_pEditBookmarkItem);
	m_pEditBookmarkItem->SetEnabled(false);
	
	//Adding "Remove Bookmark" item to "File" menu
	m_pRemoveBookmarkItem = new BMenuItem(_T("Remove Bookmark"), new BMessage(MSG_MENU_REMOVE), 'R');
	m_pFile->AddItem(m_pRemoveBookmarkItem);
	m_pRemoveBookmarkItem->SetEnabled(false);
	
	//Adding a separator item to "File" menu
	m_pFile->AddSeparatorItem();
	
	//Adding "Save Bookmarks" item to "File" menu
	m_pSaveBookmarksItem = new BMenuItem(_T("Save"), new BMessage(MSG_MENU_SAVE), 'S');
	m_pFile->AddItem(m_pSaveBookmarksItem);
	m_pSaveBookmarksItem->SetEnabled(false);
	
	//"Edit"  
	// OliverESP: nobody use this element
	// m_pEdit = new BMenu("Edit");
	// AddItem(m_pEdit);
	// m_pEdit->SetTargetForItems(this);
}

BookmarksMenu::~BookmarksMenu(void)
{
}

void BookmarksMenu::AttachedToWindow(void)
{
	BMenuBar::AttachedToWindow();
}

void BookmarksMenu::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		default :
			 BMenuBar::MessageReceived(message);
			break;
	}
}


//*****************************************************************************************************//
//	CLASS BookmarkItemWindow
//*****************************************************************************************************//

BookmarkItemWindow::BookmarkItemWindow(char* i_pTitle, BRect i_AFrame, BookmarksWindow* i_pBookmarksWindow = NULL,
										ColumnListView* /* i_pColumnListView = NULL */, BookmarkItem* i_pSelectedItem = NULL)
						: BWindow(i_AFrame, i_pTitle, B_FLOATING_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, 0)
{
	m_pBookmarksWindow = i_pBookmarksWindow;
	m_pBookmarkItemView = new BookmarkItemView(Bounds(), i_pSelectedItem);
	
	rgb_color kGray = {219,219,219,255};
	m_pBookmarkItemView->SetViewColor(kGray);
	AddChild(m_pBookmarkItemView);
		
	
}

BookmarkItemWindow::~BookmarkItemWindow(void)
{
}

BookmarkItem* BookmarkItemWindow::SendBookmarkItem(void)
{
	BookmarkItem* v_pNewBookmarkItem = new BookmarkItem(0, false, true,
														m_pBookmarkItemView->SendTextControlString(NAME),
														m_pBookmarkItemView->SendTextControlString(FIRST_NAME),
														m_pBookmarkItemView->SendTextControlString(EMAIL));
	return v_pNewBookmarkItem;
}
//========================================   MESSAGES HANDLERS   ========================================//

void BookmarkItemWindow::MessageReceived(BMessage* i_pMessage)
{
	switch (i_pMessage->what)
	{
		case MSG_ITEM_CANCEL:
			QuitRequested();
			break;
		case MSG_ITEM_OK:
			//OkRequested(i_pMessage);
			OkRequested();
			break;
		default:
			BWindow::MessageReceived(i_pMessage);
			break;
	}
}

bool BookmarkItemWindow::QuitRequested(void)
{
	Quit();
	return (TRUE);
}

//*****************************************************************************************************//
//	CLASS EditBookmarkItemWindow
//*****************************************************************************************************//

EditBookmarkItemWindow::EditBookmarkItemWindow(char* i_pTitle, BRect i_AFrame, BookmarksWindow* i_pBookmarksWindow,
										ColumnListView* i_pColumnListView, BookmarkItem *i_pSelectedItem)
						: BookmarkItemWindow(i_pTitle, i_AFrame, i_pBookmarksWindow, i_pColumnListView, i_pSelectedItem)
{
	m_pSelectedItem = i_pSelectedItem;
	m_pColumnListView = i_pColumnListView;
}

EditBookmarkItemWindow::~EditBookmarkItemWindow(void)
{
}

//void EditBookmarkItemWindow::OkRequested(BMessage* i_pMessage)
void EditBookmarkItemWindow::OkRequested(void)
{
	//Delivering message to BookmarksWindow
	BMessenger* v_pMessenger = new BMessenger(m_pBookmarksWindow);
	v_pMessenger->SendMessage(new BMessage(MSG_EDIT_REQUESTED));
	
}

//*****************************************************************************************************//
//	CLASS NewBookmarkItemWindow
//*****************************************************************************************************//

NewBookmarkItemWindow::NewBookmarkItemWindow(char* i_pTitle, BRect i_AFrame, BookmarksWindow* i_pBookmarksWindow)
						: BookmarkItemWindow(i_pTitle, i_AFrame, i_pBookmarksWindow)
{
}

NewBookmarkItemWindow::~NewBookmarkItemWindow(void)
{
}

//void NewBookmarkItemWindow::OkRequested(BMessage* i_pMessage)
void NewBookmarkItemWindow::OkRequested(void)
{
	//Delivering message to BookmarksWindow
	BMessenger* v_pMessenger = new BMessenger(m_pBookmarksWindow);
	v_pMessenger->SendMessage(new BMessage(MSG_ADD_REQUESTED));
	
}


//*****************************************************************************************************//
//	CLASS BookmarkItemView
//*****************************************************************************************************//

BookmarkItemView::BookmarkItemView(BRect i_AFrame, BookmarkItem* i_pSelectedItem) 
					: BView(i_AFrame, NULL, B_FOLLOW_ALL_SIDES, B_NAVIGABLE|B_WILL_DRAW)
{
	char* v_pName = NULL;
	char* v_pFirstName = NULL;
	char* v_pEmail = NULL;
	
	//Adding "Cancel" button to BookmarkItemView
	BRect ARectCancelButton(140, 130, 200, 155);
	m_pCancelButton = new BButton(ARectCancelButton,
									"cancelButton",
									_T("Cancel"),
									new BMessage(MSG_ITEM_CANCEL),
									B_FOLLOW_NONE,
									B_WILL_DRAW | B_NAVIGABLE);
									
	m_pCancelButton->MakeDefault(false);
	AddChild(m_pCancelButton);
	
	//Adding "OK" button to BookmarkItemView
	BRect ARectOkButton(220, 130, 280, 155);
	m_pOkButton = new BButton(ARectOkButton,
								"okButton",
								_T("OK"),
								new BMessage(MSG_ITEM_OK),
								B_FOLLOW_NONE,
								B_WILL_DRAW | B_NAVIGABLE);
	
	m_pOkButton->SetEnabled(false);							
	m_pOkButton->MakeDefault(true);
	AddChild(m_pOkButton);
	
	if(i_pSelectedItem != NULL)
	{
		//Item selected
		v_pName = (char*)i_pSelectedItem->GetColumnContentText(0);
		v_pFirstName = (char*)i_pSelectedItem->GetColumnContentText(1);
		v_pEmail = (char*)i_pSelectedItem->GetColumnContentText(2);

		m_pOkButton->SetEnabled(true);
	}
	
	//Adding "Name" text control to BookmarkItemView
	BRect ARectNameTextControl(10, 30, 280, 50);
	m_pNameTextControl = new BTextControl(ARectNameTextControl,
											"nameTextControl",
											_T("Name : "), v_pName,
											NULL,
											B_FOLLOW_NONE,
											B_WILL_DRAW | B_NAVIGABLE);
	m_pNameTextControl->SetDivider(60);
	AddChild(m_pNameTextControl);
	
	//Adding "First Name" text control to BookmarkItemView
	BRect ARectFirstNameTextControl(10, 60, 280, 80);
	m_pFirstNameTextControl = new BTextControl(ARectFirstNameTextControl,
												"firstNameTextControl",
												_T("Surname : "), v_pFirstName,
												NULL,
												B_FOLLOW_NONE,
												B_WILL_DRAW | B_NAVIGABLE);
	m_pFirstNameTextControl->SetDivider(60);
	AddChild(m_pFirstNameTextControl);
	
	//Adding "Email" text control to BookmarkItemView
	BRect ARectEmailTextControl(10, 90, 280, 110);
	m_pEmailTextControl = new BTextControl(ARectEmailTextControl,
											"emailTextControl",
											_T("Email : "), v_pEmail,
											NULL,
											B_FOLLOW_NONE,
											B_WILL_DRAW | B_NAVIGABLE);
	m_pEmailTextControl->SetDivider(60);
	AddChild(m_pEmailTextControl);
	
}

BookmarkItemView::~BookmarkItemView(void)
{

}

const char* BookmarkItemView::SendTextControlString(E_TEXT_CONTROL_FLAG i_flag)
{
	switch(i_flag)
	{
		case NAME:
			return m_pNameTextControl->Text();
			break;
		case FIRST_NAME:
			return m_pFirstNameTextControl->Text();
			break;
		case EMAIL:
			return m_pEmailTextControl->Text();
			break;
	}
	return NULL;
}

//========================================   MESSAGES HANDLERS   ========================================//

void BookmarkItemView::MessageReceived(BMessage *i_pMessage)
{
	switch(i_pMessage->what)
	{
		case MSG_ITEM_LABEL_EDIT :
			CheckTextControl();
			break;
		default :
			 BView::MessageReceived(i_pMessage);
			break;
	}
}

void BookmarkItemView::CheckTextControl(void)
{
	if(	strlen(m_pNameTextControl->Text()) &&
		strlen(m_pFirstNameTextControl->Text()) &&
		strlen(m_pEmailTextControl->Text()))
	{
		m_pOkButton->SetEnabled(true);	
	}
	else
	{
		m_pOkButton->SetEnabled(false);
	}	
}

void BookmarkItemView::AttachedToWindow(void)
{
	// Wrap a message filter/invoker around the labels control's text field
	// that will post us MSG_ITEM_LABEL_EDIT msg after the text field processes a
	// B_KEY_DOWN message.
	m_pNameTextControl->TextView()->AddFilter(
							new PostDispatchInvoker(B_KEY_DOWN,	new BMessage(MSG_ITEM_LABEL_EDIT), this));
	m_pFirstNameTextControl->TextView()->AddFilter(
							new PostDispatchInvoker(B_KEY_DOWN,	new BMessage(MSG_ITEM_LABEL_EDIT), this));
	m_pEmailTextControl->TextView()->AddFilter(
							new PostDispatchInvoker(B_KEY_DOWN,	new BMessage(MSG_ITEM_LABEL_EDIT), this));
	
	BView::AttachedToWindow();
}

