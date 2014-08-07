/***********************************************************************************************\
*	BeNetMailIPWindow.cpp																		*
*************************************************************************************************
*	Programmer par: Patrick Henri																*
*	Derniere modification: 19-03-99																*
*************************************************************************************************
*	Cette classe permet d'affichier une window modal qui affiche les candidat qui veulent se 	*
*	connecter.																					*
\***********************************************************************************************/
#ifndef __BENET_MAIL_IP_WINDOWS__
#include "BeNetMailIPWindow.h"
#endif

#include <app/Application.h>
#include <storage/Entry.h>

/*=============================================================================================*\
|	BeNetMailIPWindow																			|
+-----------------------------------------------------------------------------------------------+
|	Effet: Creer une window et y inserer les view de control et d'information.					|
|	Entree:																						|
|		PEOPLE_ID_NODE * pNode: Le pointeur vers le premier item de la liste des candidat 		|
|								potentiel.														|
\*=============================================================================================*/
BeNetMailIPWindow::BeNetMailIPWindow(PEOPLE_ID_NODE * pNode) :
	BWindow(BRect(20,20,270,439), "BeNetMailIPWindow", 
			B_MODAL_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE, B_CURRENT_WORKSPACE)
{
	m_pNode = pNode;

	pInfoView = new BeNetMailIPInfoView(BRect(0,0,Bounds().Width(), 124));
	AddChild(pInfoView);

	pPictureView = new BeNetMailIPPictureView(BRect(0,125,Bounds().Width(), 274));
	AddChild(pPictureView);

	pMessageView = new BeNetMailIPMessageView(BRect(0,275,Bounds().Width(), 374));
	AddChild(pMessageView);
	
	pControlView = new BeNetMailIPControlView(BRect(0,375,Bounds().Width(), 419));
	AddChild(pControlView);
	
	SetInfo();	
}//End of BeNetMailIPWindow


/*=============================================================================================*\
|	~BeNetMailIPWindow																			|
+-----------------------------------------------------------------------------------------------+
|	Effet: Detruire les elements de la liste.													|
\*=============================================================================================*/
BeNetMailIPWindow::~BeNetMailIPWindow()
{
	PEOPLE_ID_NODE * pTemp;
	while(m_pNode->pPrev) m_pNode = m_pNode->pPrev;
	while(m_pNode)
	{
		pTemp = m_pNode;
		m_pNode = m_pNode->pNext;
		delete pTemp;
	}
}//End of ~BeNetMailIPWindow


/*=============================================================================================*\
|	MessageReceived																				|
+-----------------------------------------------------------------------------------------------+
|	Effet: Traite un message recu.																|
|	Entree:																						|
|		BMessage *message: Le message recu.														|
\*=============================================================================================*/
void BeNetMailIPWindow::MessageReceived(BMessage *message)
{
	switch ( message->what )
	{
		case BENET_CANCEL:{
				Quit();
			}break;
			
		case BENET_DELETE:{	//Dtruire le noeud present.
				if(m_pNode)
				{
					RemoveCurrentNode();
					SetInfo();
				}
			}break;
			
		case BENET_ACCEPT:{	
				//Inserer ici le code qui demande au network d'etablir la communication.
				Quit();
			}break;
			
		case BENET_NEXT:{
				if(m_pNode)
				{
					if(m_pNode->pNext)
					{
						m_pNode = m_pNode->pNext;
						SetInfo();
					}
				}
			}break;
			
		case BENET_PREV:{	
			if(m_pNode)
				{
					if(m_pNode->pPrev)
					{
						m_pNode = m_pNode->pPrev;
						SetInfo();
					}
				}			
			}break;

			
		case B_QUIT_REQUESTED:{
				be_app_messenger.SendMessage(B_QUIT_REQUESTED);
			}break;
			
   		default:{
				BWindow::MessageReceived(message);
			}break;
	}
}//End of MessageReceived.

/*=============================================================================================*\
|	SetInfo																						|
+-----------------------------------------------------------------------------------------------+
|	Effet: Met a jour les champ dans les view de control et d'information.						|
\*=============================================================================================*/
void BeNetMailIPWindow::SetInfo()
{
	pInfoView->SetInfo(m_pNode);
	pPictureView->SetInfo(m_pNode);
	pMessageView->SetInfo(m_pNode);
	pControlView->SetInfo(m_pNode);
}//End of SetInfo.

/*=============================================================================================*\
|	RemoveCurrentNode																			|
+-----------------------------------------------------------------------------------------------+
|	Effet: Enleve le noeud courant de la liste. Si le neoud courant est le seul de la liste, on |
|			quite la window. Le nouveau noeud courant est le suivant si il y en a un, sinon 	|
|			c'est le precedent.																	|
\*=============================================================================================*/
void BeNetMailIPWindow::RemoveCurrentNode()
{
	if(!m_pNode) Quit();
	PEOPLE_ID_NODE * pTemp = m_pNode;
	if(m_pNode->pNext)
	{
		m_pNode = m_pNode->pNext;
	}
	else if(m_pNode->pPrev)
	{
		m_pNode = m_pNode->pPrev;
	}
	else
	{
		m_pNode = NULL;
	}
				
	if(pTemp->pNext)
	{
		pTemp->pNext->pPrev = pTemp->pPrev;
	}
	if(pTemp->pPrev)
	{
		pTemp->pPrev->pNext = pTemp->pNext;
	}
	BEntry entry(pTemp->pzFile);
	entry.Remove();
	delete pTemp;
	if(!m_pNode)
	{
		Quit();
	}				
}//end of RemoveCurrentNode.



