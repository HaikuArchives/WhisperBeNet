
/*

    Bubblehelp class Copyright (C) 1998 Marco Nelissen <marcone@xs4all.nl>
    Freely usable in non-commercial applications, as long as proper credit
    is given.

    Usage:
	- Add the file BubbleHelper.cpp to your project
	- #include "BubbleHelper.h" in your files where needed
    - Create a single instance of BubbleHelper (it will serve your entire
      application). It is safe to create one on the stack or as a global.
    - Call SetHelp(view,text) for each view to which you wish to attach a text.
    - Use SetHelp(view,NULL) to remove text from a view.
    
    This could be implemented as a BMessageFilter as well, but that means using
    one bubblehelp-instance for each window to which you wish to add help-bubbles.
    Using a single looping thread for everything turned out to be the most practical
    solution.

*/

#ifndef _BubbleHelper_H
#define _BubbleHelper_H

#include <kernel/OS.h>

#include <interface/View.h>

class BTextView;
class BWindow;

class BubbleHelper
{
public:
	virtual ~BubbleHelper();
	static BubbleHelper*	Instance();
	void					SetHelp(BView *view, char *text);
	void					EnableHelp(bool enable=true);

private: // functions
	BubbleHelper();
	void 					HideBubble();
	void 					ShowBubble(BPoint dest);
	static long 			_helper(void *arg);
	void					DisplayHelp(char *text,BPoint where);
	void					Helper();
	char*					GetHelp(BView *view);
	BView*					FindView(BPoint where);

private: // data
	static BubbleHelper*	m_instance;
	thread_id				helperthread;
	BList*					helplist;
	BWindow*				textwin;
	BTextView*				textview;
	bool					enabled;

	static long 			runcount;
};

#endif
