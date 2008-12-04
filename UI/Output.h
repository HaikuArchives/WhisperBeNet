#ifndef _Output_h
#define _Output_h

#include <Window.h>
#include <TextView.h>
#include <ScrollBar.h>
#include <Button.h>

//#include <experimental/TabView.h>
#include <interface/TabView.h>

const uint32 MSG_OUTPUT_RESET		= 'outr';
const uint32 MSG_OUTPUT_RESET_ALL	= 'opra';


class OutputView : public BView
{
public:
	OutputView(BRect frame);
	virtual void	FrameResized(float width, float height);

	void			Add(char* text)			{m_pTextView->Insert(text);}
	void			Clear()					{m_pTextView->Delete(0,m_pTextView->TextLength());}

private:
	BTextView*		m_pTextView;
	BScrollBar*		m_pScrollBar;
};



class Output : public BWindow
{
public:
	static Output*	Instance();
	~Output();
	virtual bool	QuitRequested();
	virtual void	MessageReceived(BMessage* msg);
	virtual void	FrameMoved(BPoint point);

	void			Audio(char* text)			{Add(text, m_pAudio);}
	void			Network(char* text)			{Add(text, m_pNetwork);}
	void			UI(char* text)				{Add(text, m_pUI);}

private: // functions
	Output();
	void			Add(char* text, OutputView* view);

private: // data
	static Output*	m_instance;
	BTab*			m_pAllTab;
	BTab*			m_pAudioTab;
	BTab*			m_pNetworkTab;
	BTab*			m_pUITab;
	OutputView*		m_pAll;
	OutputView*		m_pAudio;
	OutputView*		m_pNetwork;
	OutputView*		m_pUI;
	BButton*		m_pReset;
	BButton*		m_pResetAll;
};


#endif // _Output_h
