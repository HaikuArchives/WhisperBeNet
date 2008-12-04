#ifndef _ScopeView_h
#include "ScopeView.h"
#endif

#include <interface/Screen.h>
#include <interface/Window.h>

#include <string.h>	// for the memset()
#include <locale/Locale.h>


ScopeView::ScopeView(BRect frame, bool bShrink)
	: ShrinkView(frame,(char*) _T("Oscilloscope"), bShrink)
{
	rgb_color color = {192, 192, 192, 0};
	SetViewColor(color);

	m_w = frame.right - frame.left;
	m_h = frame.bottom - frame.top;


	// Allocate bitmap
	m_bitmap = new BBitmap(BRect(0, 0, m_w, m_h), B_CMAP8);
}


void
ScopeView::AttachedToWindow()
{
	// Look up colors for scope
	{
		BScreen scr(Window());
      //m_back_color = scr.IndexForColor(0, 32, 16);  OliverESP: for transparent efect
        m_back_color = scr.IndexForColor(192, 192, 192);
        
		m_mute_color = scr.IndexForColor(192, 192, 192);
	//	m_beam_color = scr.IndexForColor(255, 0, 0);  OliverESP: looks nicer with transparent
		m_beam_color = scr.IndexForColor(0, 0, 255);
		
		m_null_color = scr.IndexForColor(75, 75, 75);//OliverESP: 
	}
}


void
ScopeView::FrameResized(float w, float h)
{
	m_w = w;
	m_h = h;
}


void
ScopeView::NewData(int16* data, bool non_null_frame)
{
	// Ceci ralentit l'execution, alors ne l'utilisez que si vous faites du
	// resize dynamique de la vue de l'oscilloscope.
	//delete m_bitmap;
	//m_bitmap = new BBitmap(BRect(0, 0, m_w, m_h), B_COLOR_8_BIT);

	// Draw dark black backgroundB_TRANSPARENT_8_BIT
	memset((uint8 *)m_bitmap->Bits(), m_back_color, m_bitmap->BytesPerRow() * (size_t)m_h);  //OliverESP transparent?
// 	memset((uint8 *)m_bitmap->Bits(), B_TRANSPARENT_8_BIT, m_bitmap->BytesPerRow() * (size_t)m_h);  //OliverESP transparent?

	//OliverESP: Null scope
	if (non_null_frame)
		// Remplir le bitmap des points correspondant au wave.
		FillBitmap((int16*)data,m_beam_color);
	else
		// Remplir le bitmap des points correspondant au wave.
		FillBitmap((int16*)data,m_null_color);	
	
	// On affiche le bitmap a l'ecran.
	if( Window()->LockWithTimeout(100000) == B_OK )
	{
		DrawBitmap(m_bitmap, Bounds(), BRect(0, 20, m_w, m_h));
		Window()->Unlock();
	}
}


// Draw oscilloscope beam
void
ScopeView::FillBitmap(int16 *buf, uint8 color)
{
	// y1 is top (maximum value), y2 is bottom (minimum value)
	int16 old_y1 = *buf++;
	int16 old_y2 = *buf++;
	int16 y1, y2;

	int y_offset = (int)m_h/2;
	int y_height = (int)m_h;

	// Loop for all samples in buffer
	for( int i = 0; i < m_w - 1; i++, old_y1 = y1, old_y2 = y2 )
	{
		// Get next sample values
		y1 = *buf++;
		y2 = *buf++;

		// Make sure that lines are connected
		if( y1 > old_y1 && y2 > old_y1 )
			y2 = old_y1;
		if( y1 < old_y2 && y2 < old_y2 )
			y1 = old_y2;

		uint32 y1_scr = y_offset - y1 * y_height / 65533;
		uint32 y2_scr = y_offset - y2 * y_height / 65533;

		if( y1_scr >= (unsigned)m_h && y2_scr >= (unsigned)m_h )
			continue;
		if( y1_scr >= (unsigned)m_h )
			y1_scr = y1_scr & 0x80000000 ? (uint32)m_h-1 : 0;
		if( y2_scr >= (unsigned)m_h )
			y2_scr = y2_scr & 0x80000000 ? (uint32)m_h-1 : 0;

		uint32 c_index = (y2_scr - y1_scr) * 16 / (uint32)m_h;
		if( c_index > 15 )
			continue;
		uint8 *p = (uint8 *)m_bitmap->Bits() + m_bitmap->BytesPerRow() * y1_scr + i;
		for( uint32 j = y1_scr; j <= y2_scr; j++ )
		{
		//	*p = m_beam_color;
			*p = color;
			p += m_bitmap->BytesPerRow();
		}
	}
}


void
ScopeView::Mute()
{
	// Draw dark black background
	memset((uint8 *)m_bitmap->Bits(), m_mute_color, m_bitmap->BytesPerRow() * (size_t)m_h);
 
	// On affiche le bitmap a l'ecran.
	if( Window()->LockWithTimeout(100000) == B_OK )
	{
		DrawBitmap(m_bitmap, Bounds(), BRect(0, 20, m_w, m_h));
		Window()->Unlock();
	}
}
