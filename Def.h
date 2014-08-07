#ifndef _Def_h
#define _Def_h

#include <GraphicsDefs.h>

#define PREFERENCES_FILE	"prefs.dat"
#define PREFERENCES_FOLDER	"BeNet"
#define BOOKMARKS_FOLDER	"BeNet"
#define	BOOKMARKS_FILE		"Bookmarks.dat"

const uint32 MSG_ABOUT_SCROLL 				= 'abt';

const uint32 MSG_MENU_ABOUT					= 'abt';
const uint32 MSG_MENU_EXIT					= 'exit';
const uint32 MSG_MENU_PREFERENCES			= 'pref';
const uint32 MSG_MENU_BOOKMARKS				= 'bm';
const uint32 MSG_MENU_CONNECT				= 'con';
const uint32 MSG_MENU_ACCEPT				= 'acep';
const uint32 MSG_MENU_DISCONNECT			= 'disc';
const uint32 MSG_MENU_TERMINATE				= 'term';
const uint32 MSG_MENU_MAILIP				= 'mail';
const uint32 MSG_MENU_ONLINE				= 'onl';
const uint32 MSG_MENU_OFFLINE				= 'offl';
const uint32 MSG_MENU_HELP					= 'help';
const uint32 MSG_MENU_STATUSBAR				= 'sbar';
const uint32 MSG_MENU_TOOLBAR				= 'tbar';
const uint32 MSG_MENU_OUTPUTWINDOW			= 'outp';
const uint32 MSG_ABORT_CONNECTION			= 'acx';

const uint32 MSG_GENERAL_SHOW_OSCILLOSCOPE	= 'shos';
const uint32 MSG_GENERAL_SHOW_SOUNDCONTROLS	= 'shsc';
const uint32 MSG_GENERAL_SHOW_CONNECTION	= 'shcx';
const uint32 MSG_GENERAL_SHOW_BOOKMARKS		= 'shbk';
const uint32 MSG_GENERAL_SHOW_OUTPUT		= 'shop';
const uint32 MSG_AUDIO_USE_MODEL_QUALITY	= 'amod';
const uint32 MSG_AUDIO_USE_CODEC			= 'acod';

const uint32 MSG_AUDIO_COMP_CHANGED			= 'acch'; //OliverESP
const uint32 MSG_AUDIO_CUSTOM				= 'actm'; //OliverESP
const uint32 DEVICE							= 'devi'; //OliverESP 

const uint32 MSG_NETWORK_CONNECTION_TIMEOUT	= 'time';
const uint32 MSG_NETWORK_GOONLINE			= 'goon';
const uint32 MSG_NETWORK_SERVER_IP			= 'sip';
const uint32 MSG_NETWORK_SERVER_PORT		= 'spor';
const uint32 MSG_IDENTITY_FIRSTNAME			= 'fnam';
const uint32 MSG_IDENTITY_LASTNAME			= 'lnam';
const uint32 MSG_IDENTITY_EMAIL				= 'mail';

const uint32 MSG_SOUNDCONTROLS_MUTE			= 'mute';
const uint32 MSG_SOUNDCONTROLS_VOLUME		= 'vol';

const uint32 MENU_BOOKMARKS	= 'm_b';

/*Menu*/
const uint32 MSG_MENU_NEW			= 'New';
const uint32 MSG_MENU_EDIT			= 'Edt';
const uint32 MSG_MENU_REMOVE		= 'Rem';
const uint32 MSG_MENU_SAVE			= 'Sav';

/*ColumnListView*/
const uint32 MSG_SELECTION_CHANGED	= 'Sel';
const uint32 MSG_ITEMS_NB_CHANGED	= 'Num';
const uint32 MSG_ADD_REQUESTED		= 'ARq';
const uint32 MSG_EDIT_REQUESTED		= 'ERq';
const uint32 MSG_ITEM_CANCEL		= 'Can';
const uint32 MSG_ITEM_OK		    = 'Ok!';

const uint32 MSG_CONNECTION_MODE   = 'conn';
const uint32 MSG_CONNECTION_FINISH = 'cnfh';  //OliverESP
const uint32 MSG_NOT_CONNECTED     = 'ncOn';  //OliverESP

/*BoomarkItemWindow*/
const uint32 MSG_ITEM_LABEL_EDIT	= 'Lbl';

const rgb_color kGray = {219,219,219,255};

enum E_TEXT_CONTROL_FLAG
{
	NAME,
	FIRST_NAME,
	EMAIL
};



#endif // _Def_h
