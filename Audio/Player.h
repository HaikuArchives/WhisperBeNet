#ifndef _Player_h
#define _Player_h

#ifndef _ScopeView_h
#include "ScopeView.h"
#endif

#include <TimeSource.h>
#include <MediaFiles.h>
#include <SoundPlayer.h>
#include <Sound.h>
#include <MediaDefs.h>

class Player
{
public:
	Player(ScopeView* scopeView);
	~Player();

	void			Play(void* data,size_t size);
	void			PlayOld(void* data, size_t size,int); // OliverESP: to find out the exact decoding...
	void			SetPlayVolume(float volume);

private:
	BSoundPlayer	m_player;
	ScopeView*		m_scopeView;
	float			m_playVolume;
	media_output	m_audioOutput;
	char dataR[32768]; //OliverESP: debugging 
};

#endif // _Player_h
