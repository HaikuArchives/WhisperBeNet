/***********************************************************************************************\
*	BeNetBitmapCatalog.h																		*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 17-03-99																*
*************************************************************************************************
*	Cette classe singleton permet de stocker les bitmap.										*
\***********************************************************************************************/

#ifndef _BitmapCatalog_h
#define _BitmapCatalog_h

#include <Bitmap.h>

class BitmapCatalog
{
public:
	static BitmapCatalog* Instance();
	~BitmapCatalog();

	BBitmap* FetchBitmap(char *filename, bool bTran = false); 
				
	BBitmap* GetLeftArrow()				{return m_pLArrow;}
	BBitmap* GetRightArrow()			{return m_pRArrow;}
	BBitmap* GetAccept()				{return m_pAccept;}
	BBitmap* GetCancel()				{return m_pCancel;}
	BBitmap* GetDelete()				{return m_pDelete;}

	BBitmap* GetStatusBarRemote()		{return m_pStatusBarRemote;}
	BBitmap* GetStatusBarServer()		{return m_pStatusBarServer;}
	BBitmap* GetToolBarRemote()			{return m_pToolBarRemote;}
	BBitmap* GetToolBarServer()			{return m_pToolBarServer;}
	BBitmap* GetToolBarBookmarks()		{return m_pToolBarBookmarks;}
	BBitmap* GetToolBarPreferences()	{return m_pToolBarPreferences;}
	BBitmap* GetToolBarHelp()			{return m_pToolBarHelp;}
	
private:	//function
	BitmapCatalog();
		
private:	//Data
	static BitmapCatalog * m_pInstance;

	BBitmap*	m_pLArrow;
	BBitmap*	m_pRArrow;
	BBitmap*	m_pAccept;
	BBitmap*	m_pCancel;
	BBitmap*	m_pDelete;
	BBitmap*	m_pStatusBarRemote;
	BBitmap*	m_pStatusBarServer;
	BBitmap*	m_pToolBarRemote;
	BBitmap*	m_pToolBarServer;
	BBitmap*	m_pToolBarBookmarks;
	BBitmap*	m_pToolBarPreferences;
	BBitmap*	m_pToolBarHelp;
};

#endif // _BitmapCatalog_h
