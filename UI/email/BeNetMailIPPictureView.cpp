/***********************************************************************************************\
*	BeNetMailIPInfoView.h																		*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 19-03-99																*
*************************************************************************************************
*	C'est une ShrinkView qui affiche l'image d'un PEOPLE_ID_NODE.								*
\***********************************************************************************************/
#ifndef __BENET_MAIL_IP_PICTURE_VIEW__
#include "BeNetMailIPPictureView.h"
#endif

#ifndef _BitmapCatalog_h
#include "BitmapCatalog.h"
#endif

#include "locale/Locale.h"

/*=============================================================================================*\
|	BeNetMailIPPictureView																		|
+-----------------------------------------------------------------------------------------------+
|	Effet: Appelle le constructeur de la ShrinkView.											|
|	Entree:																						|
|		BRect frame: Le rectangle (dans le system de coordonne du parent) ou sera afficher la	|
|					view.																		|
\*=============================================================================================*/
BeNetMailIPPictureView::BeNetMailIPPictureView(BRect frame)
	: ShrinkView(frame, (char*) _T("Picture"), false)
{
	m_pBitmap = NULL;
}//Fin de BeNetMailIPPictureView.


/*=============================================================================================*\
|	~BeNetMailIPPictureView																		|
+-----------------------------------------------------------------------------------------------+
|	Effet: Appelle le constructeur de la ShrinkView.											|
|	Entree:																						|
|		BRect frame: Le rectangle (dans le system de coordonne du parent) ou sera afficher la	|
|					view.																		|
\*=============================================================================================*/
BeNetMailIPPictureView::~BeNetMailIPPictureView()
{
	if(m_pBitmap)
	{
		delete m_pBitmap;
	}
}//Fin de BeNetMailIPPictureView. 

/*=============================================================================================*\
|	Draw																						|
+-----------------------------------------------------------------------------------------------+
|	Effet: Redessiner la view. Utilisation du Draw par defaut de la ShrinkView plus le 			|
|			dessinement du bitmap.																|
|	Entree:																						|
|		BRect updateRect: La region a redessiner.												|
\*=============================================================================================*/
void BeNetMailIPPictureView::Draw(BRect updateRect)
{
	float fRatio;
	float fViewRatio;
	BRect rect;
	
	ShrinkView::Draw(updateRect);
	if(m_pBitmap && !IsShrink())
	{
		
		//Trouver le ratio de l'image et de l'interieur de la view.
		fRatio = m_pBitmap->Bounds().Height() / m_pBitmap->Bounds().Width();
		fViewRatio = ( Bounds().Height() - 26) / (Bounds().Width() - 10);
		
		if(fRatio > fViewRatio)
		{
			rect.top = 17;
			rect.bottom = Bounds().Height() - 9;
			rect.right = rect.Height() / fRatio;
			rect.left = 0;
			rect.OffsetBy(Bounds().Width() / 2 - rect.Width() / 2,0);	
		}
		else
		{
			rect.right = 5;
			rect.left = Bounds().Width() - 5;
			rect.top = 0; 
			rect.bottom = rect.Width() * fRatio;
			rect.OffsetBy(0, Bounds().Height() / 2 - rect.Height() / 2);
		}		
		DrawBitmap(m_pBitmap, rect);
	}
	ShrinkView::Draw(updateRect);
}//Fin de Draw.


/*=============================================================================================*\
|	SetInfo																						|
+-----------------------------------------------------------------------------------------------+
|	Effet: Definir la nouvelle image a afficher.												|
|	Entree:																						|
|		PEOPLE_ID_NODE * pNode: Le noeud qui contient le path de l'image.						|
\*=============================================================================================*/
void BeNetMailIPPictureView::SetInfo(PEOPLE_ID_NODE * pNode)
{
	//Effacer l'interieur de la view.
	SetHighColor(192,192,192,0);
	FillRect(BRect(8, 17, Bounds().Width(), Bounds().Height() - 9));

	if(pNode)
	{
		if(m_pBitmap)
		{
			delete m_pBitmap;
		}
		m_pBitmap = BitmapCatalog::Instance()->FetchBitmap(pNode->pzPicture);
		Draw(Bounds());
	}	
}//Fin de SetInfo.
