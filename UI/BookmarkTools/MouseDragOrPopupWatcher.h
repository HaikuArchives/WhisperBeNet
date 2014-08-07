/****DOCUMENTATION
Once started, MouseDragOrPopupWatcher will watch the mouse for twice the double-click time.  If the
primary mouse button is held down and the mouse is not moved more than one pixel in that time, it will
send a MW_MOUSE_POPUP message to the target view.  If the mouse moves before then, it will send a
MW_MOUSE_DRAG message, and if the mouse button is released before either of those, then a
MW_MOUSE_CLICK message is sent. (TargetView is specified as the target handler in the
BMessenger used to send the messages.  The BLooper == window of the target view is determined
automatically by the BMessenger)

These messages will have two data entries:

"where" (B_POINT_TYPE)		- The position of the mouse in TargetView's coordinate system.
"buttons" (B_INT32_TYPE)	- The mouse buttons.  See BView::GetMouse().
"modifiers" (B_INT32_TYPE)	- The modifier keys held down at the time.  See modifiers().

Once it is started, you can't stop it, but that shouldn't matter - the user will most likely release
the buttons soon, and you can interpret the events however you want.

StartMouseDragOrPopupWatcher returns a valid thread ID, or it returns an error code:
B_NO_MORE_THREADS. all thread_id numbers are currently in use.
B_NO_MEMORY. Not enough memory to allocate the resources for another thread.
****/

#ifndef _SGB_MOUSE_DRAG_POPUP_WATCHER_H_
#define _SGB_MOUSE_DRAG_POPUP_WATCHER_H_


//******************************************************************************************************
//**** SYSTEM HEADER FILES
//******************************************************************************************************
#include <View.h>


//******************************************************************************************************
//**** TYPE DEFINITIONS AND CONSTANTS
//******************************************************************************************************
const uint32 MW_MOUSE_DRAG = 'Mw-d';
const uint32 MW_MOUSE_POPUP = 'Mw-P';
const uint32 MW_MOUSE_CLICK = 'Mw-C';


//******************************************************************************************************
//**** FUNCTION DECLARATIONS
//******************************************************************************************************
thread_id StartMouseDragOrPopupWatcher(BView* TargetView);

#endif
