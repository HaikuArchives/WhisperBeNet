/***********************************************************************************************\
*	BeNetBitmapCatalog.cpp																		*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 17-03-99																*
*************************************************************************************************
*	Cette classe singleton permet de stocker les bitmap.										*
\***********************************************************************************************/
#ifndef __BITMAP_CATALOG__
#include "BitmapCatalog.h"
#endif

BeNetBitmapCatalog* BeNetBitmapCatalog::m_pInstance = NULL;

/*=============================================================================================*\
|	BeNetBitmapCatalog																			|
+-----------------------------------------------------------------------------------------------+
|	Effet: Charge en memoire les bitmap.														|
\*=============================================================================================*/
BeNetBitmapCatalog::BeNetBitmapCatalog()
{
	m_pLArrow = FetchBitmap("/boot/home/LArrow.jpg", true);
	m_pRArrow = FetchBitmap("/boot/home/RArrow.jpg", true);
	m_pAccept = FetchBitmap("/boot/home/Accept.tif", true);
	m_pCancel = FetchBitmap("/boot/home/Cancel.jpg", true);
	m_pDelete = FetchBitmap("/boot/home/Delete.jpg", true);
}//Fin de BeNetBitmapCatalog.

/*=============================================================================================*\
|	~BeNetBitmapCatalog																			|
+-----------------------------------------------------------------------------------------------+
|	Effet: Detruit la seule instance de la classe ainsi que les bitmap.							|
\*=============================================================================================*/
BeNetBitmapCatalog::~BeNetBitmapCatalog()
{
	delete m_pLArrow;
	delete m_pRArrow;
	delete m_pAccept;
	delete m_pCancel;
	delete m_pDelete;
	
	delete m_pInstance;	
}//Fin de ~BeNetBitmapCatalog.


/*=============================================================================================*\
|	Instance																					|
+-----------------------------------------------------------------------------------------------+
|	Effet: Cree la seule instance de la classe si elle n'existe pas deja.						|
|	Sortie:																						|
|		BeNetBitmapCatalog *: Le pointeur vers la seule instance de la classe.					|
\*=============================================================================================*/
BeNetBitmapCatalog * BeNetBitmapCatalog::Instance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new BeNetBitmapCatalog;
	}
	return m_pInstance;
}//fin de Instance.

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
BBitmap * BeNetBitmapCatalog::FetchBitmap(char *pzFileName, bool bTrans) 
{ 
      BFile file(pzFileName, B_READ_ONLY); 
      BTranslatorRoster *roster = BTranslatorRoster::Default(); 
      BBitmapStream stream; 
      BBitmap *result = NULL; 
      if (roster->Translate(&file, NULL, NULL, &stream, 
            B_TRANSLATOR_BITMAP) < B_OK) 
         return NULL; 
      stream.DetachBitmap(&result); 
            
      
      if(!bTrans) return result;
      int32 iLenght = result->BitsLength() / 4;
      int32 i;
      int32 * cBit = (int32)result->Bits();
      int32 backColor = cBit[result->Bounds().IntegerWidth() - 1];
      int32 iTrans = 0;
      
      //Determine le mode de definition de couleur
      switch(result->ColorSpace())
      {
      		case B_RGB32:{
      				iTrans = B_TRANSPARENT_MAGIC_RGBA32;
      			}break;
      		case B_RGB32_BIG:{
      				iTrans = B_TRANSPARENT_MAGIC_RGBA32_BIG;
      			}break;
       }
      
      if(iTrans)
      {
     	for(i = 0; i < iLenght; i++)
      	{
      		if(cBit[i] == backColor)
      		{
     			cBit[i] = B_TRANSPARENT_MAGIC_RGBA32_BIG;
      		}
      	}
      }
      
      return result; 
}//Fin de FetchBitmap.
