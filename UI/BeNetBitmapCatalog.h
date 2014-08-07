/***********************************************************************************************\
*	BeNetBitmapCatalog.h																		*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 17-03-99																*
*************************************************************************************************
*	Cette classe singleton permet de stocker les bitmap.										*
\***********************************************************************************************/

#ifndef __BENET_BITMAP_CATALOG__
#define __BENET_BITMAP_CATALOG__

#include <Be.h>

class BeNetBitmapCatalog{
	public:
		static BeNetBitmapCatalog * Instance();
		~BeNetBitmapCatalog();

		BBitmap * FetchBitmap(char *filename, bool bTran = false); 
				
		BBitmap * GetLeftArrow() {return m_pLArrow;}
		BBitmap * GetRightArrow() {return m_pRArrow;}
		BBitmap * GetAccept() {return m_pAccept;}
		BBitmap * GetCancel() {return m_pCancel;}
		BBitmap * GetDelete() {return m_pDelete;}
	
	private:	//function
		BeNetBitmapCatalog();
		
	private:	//Data
		static BeNetBitmapCatalog * m_pInstance;
		
		BBitmap * m_pLArrow;
		BBitmap * m_pRArrow;
		BBitmap * m_pAccept;
		BBitmap * m_pCancel;
		BBitmap * m_pDelete;
};

#endif __BENET_BITMAP_CATALOG__