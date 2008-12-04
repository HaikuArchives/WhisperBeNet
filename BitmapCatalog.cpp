/***********************************************************************************************\
*	BeNetBitmapCatalog.cpp																		*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 17-03-99																*
*************************************************************************************************
*	Cette classe singleton permet de stocker les bitmap.										*
\***********************************************************************************************/
#ifndef _BitmapCatalog_h
#include "BitmapCatalog.h"
#endif

#include <File.h>
#include <TranslatorRoster.h>
#include <BitmapStream.h>
#include <TranslationUtils.h> //OliverESP:

BitmapCatalog* BitmapCatalog::m_pInstance = NULL;

/*=============================================================================================*\
|	BeNetBitmapCatalog																			|
+-----------------------------------------------------------------------------------------------+
|	Effet: Charge en memoire les bitmap.														|
\*=============================================================================================*/
BitmapCatalog::BitmapCatalog()
{
	m_pLArrow = FetchBitmap("Bitmaps/LArrow.jpg", false);
	m_pRArrow = FetchBitmap("Bitmaps/RArrow.jpg", false);
	m_pAccept = FetchBitmap("Bitmaps/Accept.jpg", false);
	m_pCancel = FetchBitmap("Bitmaps/Cancel.jpg", false);
	m_pDelete = FetchBitmap("Bitmaps/Delete.jpg", false);

	m_pStatusBarRemote		= FetchBitmap("Bitmaps/statusbar-remote.bmp", true);
	m_pStatusBarServer		= FetchBitmap("Bitmaps/statusbar-server.bmp", true);
	m_pToolBarRemote		= FetchBitmap("Bitmaps/toolbar_connect.bmp", true);
	m_pToolBarServer		= FetchBitmap("Bitmaps/toolbar-online.bmp", true);
	m_pToolBarBookmarks		= FetchBitmap("Bitmaps/toolbar-contacts.bmp", true);
	m_pToolBarPreferences	= FetchBitmap("Bitmaps/toolbar-settings.bmp", true);
	m_pToolBarHelp			= FetchBitmap("Bitmaps/toolbar-help.bmp", true);
}

/*=============================================================================================*\
|	~BeNetBitmapCatalog																			|
+-----------------------------------------------------------------------------------------------+
|	Effet: Detruit la seule instance de la classe ainsi que les bitmap.							|
\*=============================================================================================*/
BitmapCatalog::~BitmapCatalog()
{
	delete m_pLArrow;
	delete m_pRArrow;
	delete m_pAccept;
	delete m_pCancel;
	delete m_pDelete;
	delete m_pStatusBarRemote;
	delete m_pStatusBarServer;
	delete m_pToolBarRemote;
	delete m_pToolBarServer;
	delete m_pToolBarBookmarks;
	delete m_pToolBarPreferences;
	delete m_pToolBarHelp;
	
	delete m_pInstance;	
}


/*=============================================================================================*\
|	Instance																					|
+-----------------------------------------------------------------------------------------------+
|	Effet: Cree la seule instance de la classe si elle n'existe pas deja.						|
|	Sortie:																						|
|		BeNetBitmapCatalog*: Le pointeur vers la seule instance de la classe.					|
\*=============================================================================================*/
BitmapCatalog* BitmapCatalog::Instance()
{
	if(m_pInstance == NULL)
		m_pInstance = new BitmapCatalog;

	return m_pInstance;
}

/*=============================================================================================*\
|	FetchBitmap																					|
+-----------------------------------------------------------------------------------------------+
|	Effet: Converie une image en un BBitmap. La couleur de transparence est celle du pixel dans	|
|			le coin superieur gauche.															|
|	Entree:																						|
|		char *pzFileName: Le path du fichier image a convertir.									|
|		bool bTran: True si on utilise la transparence, false sinon.							|
|	Sortie:																						|
|		BBitmap *: Le pointeur le bitmap de l'image. NULL si la conversion a echouer.			|
\*=============================================================================================*/
BBitmap*
BitmapCatalog::FetchBitmap(char* pzFileName, bool bTrans) 
{ 


/*	BFile file(pzFileName, B_READ_ONLY); 
	BTranslatorRoster *roster = BTranslatorRoster::Default(); 
	BBitmapStream stream; 
	BBitmap *result = NULL; 
	if (roster->Translate(&file, NULL, NULL, &stream, B_TRANSLATOR_BITMAP) < B_OK) 
		return NULL; 
	stream.DetachBitmap(&result); 
*/

//  OliverESP: 7 x 1 so -> #include <TranslationUtils.h> //OliverESP:
//			   less code and works
	BBitmap *result = BTranslationUtils::GetBitmapFile(pzFileName);
	
	if (result == NULL)
		return NULL;

	if(!bTrans)
		return result;
	
	int32 iLenght = result->BitsLength() / 4;
	int32 i;
	int32 * cBit = (int32*)result->Bits();
	int32 backColor = cBit[result->Bounds().IntegerWidth() - 1];
	int32 iTrans = 0;
      
	//Determine le mode de definition de couleur
	switch(result->ColorSpace())
	{
	case B_RGB32:		iTrans = B_TRANSPARENT_MAGIC_RGBA32; break;
	case B_RGB32_BIG:	iTrans = B_TRANSPARENT_MAGIC_RGBA32_BIG; break;
	default:			break; //TODO: Major screwup here!
	}

	if (iTrans)
	{
		for(i = 0; i < iLenght; i++)
		{
			if(cBit[i] == backColor)
				cBit[i] = iTrans;
		}
	}

	return result; 
}
