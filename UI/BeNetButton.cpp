#ifndef __BENET_BUTTON__
#include "BeNetButton.h"
#endif

#include <interface/Window.h>
#include <string.h>

/*=============================================================================================*\
|	BeNetButton																					|
+-----------------------------------------------------------------------------------------------+
|	Effet:	Initialisation des membres du bouton.												|
|																								|
\*=============================================================================================*/
BeNetButton::BeNetButton(BRect frame, BBitmap * pBmp, char * label, uint32 uiMsgID, int iDrawMode, uint32 uiMsgIDToggle, bool bAutoToggle)
	: BButton(frame, "BeNetButton", NULL, NULL, 0, B_WILL_DRAW),
	m_bAutoToggle(bAutoToggle)
{
	rgb_color color = {192, 192, 192, 0};
	SetViewColor(color);
	
	m_pBitmap = pBmp;
	if(pBmp)
	{
		m_bBitmap = true;

	}
	else
	{
		m_bBitmap = false;
	}
	
	if (label == NULL)
	{
		m_bLabel = false;
		strcpy(m_pzLabel, "");
	}
	else
	{
		m_bLabel = true;
		strcpy(m_pzLabel, label);
	}
	
	m_uiMessageIDToggle = uiMsgIDToggle;
	m_uiMessageID = uiMsgID;
	m_bMouseOver = false;
	m_iDrawMode = iDrawMode;
	m_bToggle = false;
}

void BeNetButton::AttachedToWindow()
{
	uint32 * pSource;
	
	if(!m_bBitmap) return;
	if(Parent());
	SetViewColor(Parent()->ViewColor());
				
	pSource = (uint32*)m_pBitmap->Bits();
		
	if(m_pBitmap->ColorSpace() == B_RGB32)
	{
		for(int i = 0; i < m_pBitmap->BitsLength()/4; i++)
		{
			if(pSource[i] == B_TRANSPARENT_MAGIC_RGBA32)
			{
				pSource[i] = Parent()->ViewColor().red * 0x10000 + Parent()->ViewColor().green * 0x100 + Parent()->ViewColor().blue;
			}
		}
	}
	if(m_pBitmap->ColorSpace() == B_RGB32_BIG)
	{
		for(int i = 0; i < m_pBitmap->BitsLength()/4; i++)
		{
			if(pSource[i] == B_TRANSPARENT_MAGIC_RGBA32_BIG)
			{
				pSource[i] = Parent()->ViewColor().red * 0x10000 + Parent()->ViewColor().green * 0x100 + Parent()->ViewColor().blue;
			}
		}
	}
}

/*=============================================================================================*\
|	Draw																						|
+-----------------------------------------------------------------------------------------------+
|	Effet: Redessine la view selon son status.													|
|																								|
\*=============================================================================================*/
void BeNetButton::Draw(BRect updateRect)
{
	BPoint pPointList[8];
	BPoint cursor;
	uint32 iButton;
	float leftBmp = 0;
	float topBmp = 0;
	float leftLab = 0;
	float topLab = 0;
	bool m_bBorderIn = false;
	bool m_bBorderOut = false;
	BFont font;
	
	GetMouse(&cursor, &iButton);
	
	m_bMouseOver = false;
	if(cursor.x >= 0 && cursor.x < Bounds().Width() && cursor.y >= 0 && cursor.y < Bounds().Height())
	{
		m_bMouseOver = true;
	}

	
	//Mode de dessinement utilisant seulement des bitmap.
	if(m_iDrawMode == BENET_DM_FULL_BMP || (m_iDrawMode == BENET_DM_TOGGLE && !m_bToggle))
	{
		SetDrawingMode(B_OP_COPY);
		//Le bouton est disable.
		if(!IsEnabled())
		{
			DrawBitmap(m_pBitmap, BRect(3 * Bounds().Width(),0, 4 * Bounds().Width()-1,Bounds().Height()-1), BRect(0,0,Bounds().Width()-1,Bounds().Height()-1));
		}	
	
		//Le boutton est enfoncer
		else if(Value())
		{
			DrawBitmap(m_pBitmap, BRect(2 * Bounds().Width(),0, 3 * Bounds().Width()-1,Bounds().Height()-1), BRect(0,0,Bounds().Width()-1,Bounds().Height()-1));
		}
		//La sourie est par dessus le bouton.
		else if(m_bMouseOver)
		{
			DrawBitmap(m_pBitmap, BRect(Bounds().Width(),0, 2 * Bounds().Width()-1,Bounds().Height()-1), BRect(0,0,Bounds().Width()-1,Bounds().Height()-1));
		}
		//boutton par defaut
		else
		{
			DrawBitmap(m_pBitmap, BRect(0,0,Bounds().Width()-1,Bounds().Height()-1), BRect(0,0,Bounds().Width()-1,Bounds().Height()-1));
		}

	}
	else if(m_iDrawMode == BENET_DM_TOGGLE && m_bToggle)
	{
		SetDrawingMode(B_OP_COPY);		//Le bouton est disable.
		if(!IsEnabled())
		{
			DrawBitmap(m_pBitmap, BRect(3 * Bounds().Width(),Bounds().Height(), 4 * Bounds().Width()-1,Bounds().Height()*2-1), BRect(0,0,Bounds().Width()-1,Bounds().Height()-1));
		}	
	
		//Le boutton est enfoncer
		else if(Value())
		{
			DrawBitmap(m_pBitmap, BRect(2 * Bounds().Width(),Bounds().Height(), 3 * Bounds().Width()-1,Bounds().Height()*2-1), BRect(0,0,Bounds().Width()-1,Bounds().Height()-1));
		}
		//La sourie est par dessus le bouton.
		else if(m_bMouseOver)
		{
			DrawBitmap(m_pBitmap, BRect(Bounds().Width(),Bounds().Height(), 2 * Bounds().Width()-1,Bounds().Height()*2-1), BRect(0,0,Bounds().Width()-1,Bounds().Height()-1));
		}
		//boutton par defaut
		else
		{
			DrawBitmap(m_pBitmap, BRect(0,Bounds().Height(),Bounds().Width()-1,Bounds().Height()*2-1), BRect(0,0,Bounds().Width()-1,Bounds().Height()-1));
		}


	}
	

	
	///////////////////////////
	
	// Debut de l'ancient mode de dessinement.
	else if(m_iDrawMode == BENET_DM_DEFAULT)
	{
		//Aller chercher les propriete du font.
		GetFont(&font);		
		font_height height;
		font.GetHeight(&height);
		
		//Trouver le coin superieur gauche du bitmap.
		if(m_bLabel)
		{
			topBmp = 2;
		}
		else
		{
			if(m_bBitmap)topBmp= Bounds().Height() / 2 - m_pBitmap->Bounds().Height() / 2; 
		}
		if(m_bBitmap)leftBmp = Bounds().Width() / 2 - m_pBitmap->Bounds().Width() / 2;
		
		//Trouver le coin inferieur gauche de lu label.
		if(m_bBitmap)
		{
			topLab = Bounds().Height() -2 - height.descent;
		}
		else
		{
			topLab = Bounds().Height() / 2 + font.Size() / 2;
		}
		leftLab = Bounds().Width() / 2 - StringWidth(m_pzLabel) / 2;
		
		
		//Le bouton est disable.
		// Il est a 25% d'intensite.
		if(!IsEnabled())
		{
			SetHighColor(192,192,192,0);
			FillRect(BRect(0,0,Bounds().Width(), Bounds().Height()));
			SetDrawingMode(B_OP_BLEND);
			if(m_bBitmap)
			{
				DrawBitmap(m_pBitmap, BPoint(leftBmp,topBmp));
			}
			if(m_bLabel)
			{
				SetHighColor(0, 0, 0, 0);
				DrawString(m_pzLabel, BPoint(leftLab,topLab));
			}
			SetHighColor(192,192,192,0);
			FillRect(BRect(0,0,Bounds().Width(), Bounds().Height()));
			SetDrawingMode(B_OP_COPY);
		}	
	
		//Le boutton est enfoncer
		// les couleur du boutton sont inverser.
		else if(Value())
		{
			SetHighColor(192, 192, 192, 0);
			FillRect(BRect(2,2,Bounds().Width()-2, Bounds().Height()-2));
			if(m_bBitmap)
			{
				DrawBitmap(m_pBitmap, BPoint(leftBmp,topBmp));
			}
			if(m_bLabel)
			{
				SetHighColor(0, 0, 0, 0);
				DrawString(m_pzLabel, BPoint(leftLab,topLab));
			}			
			m_bBorderIn = true;
			SetDrawingMode(B_OP_INVERT);
			FillRect(BRect(2,2,Bounds().Width()-2, Bounds().Height()-2));
			SetDrawingMode(B_OP_COPY);
		}
		//Le boutton est simplement dessiner
		else 
		{
			if(m_bMouseOver) m_bBorderOut = true;
			SetHighColor(192, 192, 192, 0);
			FillRect(BRect(0,0,Bounds().Width(), Bounds().Height()));
			if(m_bBitmap)
			{
				DrawBitmap(m_pBitmap, BPoint(leftBmp,topBmp));
			}
			if(m_bLabel)
			{
				SetHighColor(0, 0, 0, 0);
				DrawString(m_pzLabel, BPoint(leftLab,topLab));
			}
		}
	

		//Le contour foncer
		if(m_bBorderIn || m_bBorderOut)
		{
			SetHighColor(64, 64, 64, 0);
			pPointList[0] = BPoint(0,2);
			pPointList[1] = BPoint(2,0);
			pPointList[2] = BPoint(Bounds().Width() -2,0);
			pPointList[3] = BPoint(Bounds().Width(),2);
			pPointList[4] = BPoint(Bounds().Width(),Bounds().Height() -2);
			pPointList[5] = BPoint(Bounds().Width() -2,Bounds().Height());
			pPointList[6] = BPoint(2,Bounds().Height());
			pPointList[7] = BPoint(0,Bounds().Height() -2);
			StrokePolygon(pPointList, 8, true,  B_SOLID_HIGH);	
		}

		// Le boutton est enfoncer
		if (m_bBorderIn)
		{
			SetHighColor(128, 128, 128, 0);
			pPointList[0] = BPoint(1,Bounds().Height() -2);
			pPointList[1] = BPoint(1,2);
			pPointList[2] = BPoint(2,1);
			pPointList[3] = BPoint(Bounds().Width() -2,1);
			StrokePolygon(pPointList, 4, false,  B_SOLID_HIGH);	
	
			SetHighColor(255, 255, 255, 0);
			pPointList[0] = BPoint(Bounds().Width() -1,2);
			pPointList[1] = BPoint(Bounds().Width() -1,Bounds().Height() -2);
			pPointList[2] = BPoint(Bounds().Width() -2,Bounds().Height() -1);
			pPointList[3] = BPoint(2,Bounds().Height() -1);
			StrokePolygon(pPointList, 4, false,  B_SOLID_HIGH);
			SetHighColor(192, 192, 192, 0);
		}

		// Le boutton est surelever.
		else if(m_bBorderOut)
		{	
			SetHighColor(255, 255, 255, 0);
			pPointList[0] = BPoint(1,Bounds().Height() -2);
			pPointList[1] = BPoint(1,2);
			pPointList[2] = BPoint(2,1);
			pPointList[3] = BPoint(Bounds().Width() -2,1);
			StrokePolygon(pPointList, 4, false,  B_SOLID_HIGH);	
	
			SetHighColor(128, 128, 128, 0);
			pPointList[0] = BPoint(Bounds().Width() -1,2);
			pPointList[1] = BPoint(Bounds().Width() -1,Bounds().Height() -2);
			pPointList[2] = BPoint(Bounds().Width() -2,Bounds().Height() -1);
			pPointList[3] = BPoint(2,Bounds().Height() -1);
			StrokePolygon(pPointList, 4, false,  B_SOLID_HIGH);
			SetHighColor(192, 192, 192, 0);
		}
	}
}

/*=============================================================================================*\
|	MouseMoved																					|
+-----------------------------------------------------------------------------------------------+
|	Effet:	Redessine le bouton si le curseur est par dessus.									|
|																								|
\*=============================================================================================*/
void BeNetButton::MouseMoved(BPoint point, uint32 transit, const BMessage *message)
{
	//Le curseur est sur la view.
	if( transit == B_ENTERED_VIEW  || 
		( point.x >= 0 && point.x < Bounds().Width() && point.y >= 0 && point.y < Bounds().Height()) && 
		 !m_bMouseOver)
	{
		m_bMouseOver = true;
		if(IsEnabled())
		{
			Draw(BRect(0,0,Bounds().Width(),Bounds().Height()));
		}
	}
	
	//Le curseur est sortie de la view.
	if(transit == B_EXITED_VIEW || 
		!( point.x >= 0 || point.x < Bounds().Width() && point.y >= 0 && point.y < Bounds().Height()) &&
		 m_bMouseOver)
	{
		m_bMouseOver = false;
		if(IsEnabled())
		{
			Draw(BRect(0,0,Bounds().Width(),Bounds().Height()));
		}
	}
	BButton::MouseMoved(point, transit, message);

}


/*=============================================================================================*\
|	Invoke																						|
+-----------------------------------------------------------------------------------------------+
|	Effet: 										|
|																								|
\*=============================================================================================*/
status_t BeNetButton::Invoke(BMessage *message)
{
	if(Window())
	{
		BMessage msg;
		if(m_iDrawMode == BENET_DM_TOGGLE && m_bToggle)
		{
			msg.what = m_uiMessageIDToggle;
		}
		else
		{
			msg.what = m_uiMessageID;
		}
		BMessenger(this->Window()).SendMessage(&msg);
		
	}
	if (m_iDrawMode == BENET_DM_TOGGLE && m_bToggle && !m_bAutoToggle)
		m_bToggle = false;
	else if (m_iDrawMode == BENET_DM_TOGGLE && !m_bAutoToggle)
		m_bToggle = true;

	BButton::Invoke(message);
	
	return B_OK;
}


/*=============================================================================================*\
|	SetBitmap																					|
+-----------------------------------------------------------------------------------------------+
|	Effet:	Change l'image du bouton.					 										|
|																								|
\*=============================================================================================*/
void
BeNetButton::SetBitmap(BBitmap * pBmp)
{
	m_pBitmap = pBmp;
	Draw(Bounds());
} 


/*=============================================================================================*\
|	SetLabel																					|
+-----------------------------------------------------------------------------------------------+
|	Effet: 	Change le texte du bouton.															|
|																								|
\*=============================================================================================*/
void
BeNetButton::SetLabel(char * label)
{
	if (label == NULL)
		strcpy(m_pzLabel, "");
	else
		strcpy(m_pzLabel, label);

	Draw(Bounds());
} 


bool
BeNetButton::GetToggle()
{
	return m_bToggle;
}


void
BeNetButton::SetToggle(bool bToggle)
{
	if(m_iDrawMode == BENET_DM_TOGGLE)
	{
		m_bToggle = bToggle;
		Draw(Bounds());
	}
}


void
BeNetButton::Toggle()
{
	if(m_iDrawMode == BENET_DM_TOGGLE)
	{
		m_bToggle = !m_bToggle;
		Draw(Bounds());
	}
}
