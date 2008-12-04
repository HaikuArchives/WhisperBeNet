
#ifndef _Recorder_h
#define _Recorder_h

#ifndef _SoundConsumer_h
#include "SoundConsumer.h"
#endif

#ifndef _TYPES
#include "types.h"
#endif

#include <MediaRoster.h>
#include <TimeSource.h>
#include <MediaFiles.h>

class ScopeView;


class Recorder
{
public:
	Recorder(ScopeView* scopeView = NULL);
	virtual	~Recorder();

	void 			Record();
	status_t		MakeRecordConnection(const media_node & input);
	status_t        MakeTestConnection(const media_node & input);  // debug method

	status_t		BreakRecordConnection();
	status_t		StopRecording();
	static void		DataEncode(void * cookie, bigtime_t timestamp, void * data, size_t size, const media_raw_audio_format & format);
	static void		NotifyDataEncode(void * cookie, int32 code, ...);

	void			SetRecVolume(float volume);
	void			SetPlayVolume(float volume);

	void 			MediaProblem(const char* string);

private:
	BMediaRoster*	m_roster;
	media_input		m_curInput;
	media_output	m_myNode;
	SoundConsumer*	m_recordNode;

	media_node		m_audioInputNode;
	media_output	m_audioOutput;
	media_input		m_recInput;

	media_node    audioMIXNode;   // OliverESP debugging
	media_node    timeSourceNode; // OliverESP debugging

	float			m_playVolume;
};

#endif
