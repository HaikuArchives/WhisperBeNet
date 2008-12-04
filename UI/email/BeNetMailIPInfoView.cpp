/***********************************************************************************************\
*	BeNetMailIPInfoView.cpp																		*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 19-03-99																*
*************************************************************************************************
*	C'est une ShrinkView qui affiche les donnee d'un PEOPLE_ID_NODE.							*
\***********************************************************************************************/
#ifndef __BENET_MAIL_IP_INFO_VIEW__
#include "BeNetMailIPInfoView.h"
#endif

#ifndef _BitmapCatalog_h
#include "BitmapCatalog.h"
#endif
/*=============================================================================================*\
|	BeNetMailIPInfoView																			|
+-----------------------------------------------------------------------------------------------+
|	Effet: Appelle le constructeur de la ShrinkView.											|
|	Entree:																						|
|		BRect frame: Le rectangle (dans le system de coordonne du parent) ou sera afficher la	|
|					view.																		|
\*=============================================================================================*/
BeNetMailIPInfoView::BeNetMailIPInfoView(BRect frame)
	: ShrinkView(frame, "Information", false)
{
}//Fin de BeNetMailIPInfoView.

/*=============================================================================================*\
|	AttachedToWindow																			|
+-----------------------------------------------------------------------------------------------+
|	Effet: Cree les control d'affichage de text et les attache a la view.						|
\*=============================================================================================*/
void BeNetMailIPInfoView::AttachedToWindow()
{
	//Cree le control qui affichera l'IP
	pIP = new BTextControl(BRect(10, 20, Bounds().Width() - 20, 40), 
							"BeNetMailIPInfoIP", "IP", "", NULL);
	pIP->SetEnabled(false);
	pIP->SetDivider(50);
	AddChild(pIP);
	
	//Cree le control qui affichera le nickname.
	pName = new BTextControl(BRect(10, 45, Bounds().Width() - 20, 65), 
							"BeNetMailIPInfoName", "NickName", "", NULL);
	pName->SetEnabled(false);
	pName->SetDivider(50);
	AddChild(pName);
	
	//Cree le control qui affichera le E-Mail.
	pEMail = new BTextControl(BRect(10, 70, Bounds().Width() - 20, 90), 
							"BeNetMailIPInfoEmail", "E-Mail", "", NULL);
	pEMail->SetEnabled(false);
	pEMail->SetDivider(50);
	AddChild(pEMail);
	
	//Cree le control qui affichera le Date.
	pDate = new BTextControl(BRect(10, 95, Bounds().Width() - 20, 115), 
							"BeNetMailIPInfoDate", "Date", "", NULL);
	pDate->SetEnabled(false);
	pDate->SetDivider(50);
	AddChild(pDate);
}//Fin de attachedTowindow.

/*=============================================================================================*\
|	SetInfo																						|
+-----------------------------------------------------------------------------------------------+
|	Effet: Change le text dex control avec les valeurs du nouveau noeud.						|
|	Entree: 																					|
|		PEOPLE_ID_NODE * pNode: Le noeud qui contient les informations.							|
\*=============================================================================================*/
void BeNetMailIPInfoView::SetInfo(PEOPLE_ID_NODE * pNode)
{
	if(!pNode)
	{
		return;
	}
	pIP->SetText(pNode->pzIP);
	pName->SetText(pNode->pzName);
	pEMail->SetText(pNode->pzEMail);
	pDate->SetText(pNode->pzDate);
}//Fin de SetInfo.
