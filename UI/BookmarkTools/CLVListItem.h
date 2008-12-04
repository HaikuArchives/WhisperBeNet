#ifndef _CLV_LIST_ITEM_H_
#define _CLV_LIST_ITEM_H_

#include <ListItem.h>


class ColumnListView;


//******************************************************************************************************//
//	CLASS CLVListItem 																					//
//******************************************************************************************************//
//	ABSTRACT & PURPOSE :																				//
//																										//
//	Items in the list to be added to the ColumnListView.												//
//																										//
//******************************************************************************************************//



class CLVListItem : public BListItem
{
	public:
		//Constructor and destructor
		CLVListItem(uint32 level = 0, bool superitem = false, bool expanded = false, float minheight = 0.0);
		~CLVListItem();

		virtual void 	DrawItemColumn(BView* owner, BRect item_column_rect, int32 column_index, bool complete) = 0;
								//column_index (0-N) is based on the order in which the columns were added
								//to the ColumnListView, not the display order.  An index of -1 indicates
								//that the program needs to draw a blank area beyond the last column.  The
								//main purpose is to allow the highlighting bar to continue all the way to
								//the end of the ColumnListView, even after the end of the last column.
		virtual void 	DrawItem(BView* owner, BRect itemRect, bool complete);
								//In general, you don't need or want to override DrawItem().
		BRect 			ItemColumnFrame(int32 column_index, ColumnListView* owner);
		float 			ExpanderShift(int32 column_index, ColumnListView* owner);
		virtual void 	Update(BView* owner, const BFont* font);
		bool 			IsSuperItem() const;
		void 			SetSuperItem(bool superitem);
		uint32 			OutlineLevel() const;
		void 			SetOutlineLevel(uint32 level);
		virtual void 	ColumnWidthChanged(int32 column_index, float column_width, ColumnListView* the_view);
		virtual void 	FrameChanged(int32 column_index, BRect new_frame, ColumnListView* the_view);

	private:
		friend class ColumnListView;

		bool 			fSuperItem;
		uint32 			fOutlineLevel;
		float 			fMinHeight;
		BRect 			fExpanderButtonRect;
		BRect 			fExpanderColumnRect;
		BList* 			fSortingContextBList;
		ColumnListView* fSortingContextCLV;
};


//******************************************************************************************************
//**** UTILITY FUNCTIONS
//******************************************************************************************************
char* ToUpperStr(char* str);		//Returns an uppercase equivalent of the given string.  Must be delete[]'d.


#endif
