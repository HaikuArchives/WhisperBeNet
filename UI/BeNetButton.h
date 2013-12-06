#ifndef __BENET_BUTTON__
#define __BENET_BUTTON__

#include <Button.h>
#include <Bitmap.h>

#define BENET_DM_DEFAULT 0
#define BENET_DM_FULL_BMP 1 
#define BENET_DM_TOGGLE 2 

class BeNetButton : public BButton
{
public:
	BeNetButton(BRect farme, BBitmap *, char * label, uint32 uiMsgID, int iDrawMode, uint32 uiMsgIDToggle = 0, bool bAutoToggle = false);
	void				SetBitmap(BBitmap * pBmp);
	void				SetLabel(char * label);
	void				Toggle();
	void				SetToggle(bool bToggle);
	bool				GetToggle();
	virtual void		Draw(BRect updateRect);
	virtual void		MouseMoved(BPoint point, uint32 transit, const BMessage* message);
	virtual status_t	Invoke(BMessage *message = NULL);
	virtual void		AttachedToWindow();

private:
	uint32		m_uiMessageID;
	uint32		m_uiMessageIDToggle;
	BBitmap*	m_pBitmap;
	char		m_pzLabel[32];
	bool		m_bMouseOver;		
	bool		m_bLabel;
	bool		m_bBitmap;
	int			m_iDrawMode;
	bool		m_bToggle;
	bool		m_bAutoToggle;
};

#endif // __BENET_BUTTON__
