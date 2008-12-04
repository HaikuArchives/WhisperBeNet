#ifndef __INFORMATION_VIEW__
#define __INFORMATION_VIEW__

#include <View.h>
#include <TextControl.h>

class InformationView : public BView
{
public:
	InformationView(BRect, char *);
	~InformationView();
		
	virtual void AttachedToWindow(void);
	virtual void Draw(BRect i_updatedRect);		
	void Update();

private: // functions
	void DrawDegrader(BRect i_rect, int i_startGray, int i_endGray);
	
private: // data
	BTextControl*	m_pLastName;
	BTextControl*	m_pFirstName;
	BTextControl*   m_pEmail;
};

#endif // __INFORMATION_VIEW__
