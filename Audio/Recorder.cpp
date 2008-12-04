//#define DEBUG
//#define DEBUG_hook
//#define DEBUG_format
#define FILTER

#ifndef _Recorder_h
#include "Recorder.h"
#endif

#ifndef _ScopeView_h
#include "ScopeView.h"
#endif

#ifndef _Preferences_h
#include "Preferences.h"
#endif

#ifndef _BeNetWindow_h
#include "BeNetWindow.h"
#endif

#ifndef _Encode_h
#include "Encode.h"
#endif

#ifndef _Network
#include "network.h"
#endif

#ifndef _NetworkInterface_h
#include "NetworkInterface.h"
#endif

#include <stdio.h>


ScopeView* m_scopeView;
Encode encodeur;
float m_recVolume;
static bigtime_t 		lasttimestamp;       // OliverESP: last timestamp
static bool 			last_non_null_trame; // Last trame silence condition

static TRtpData dataR; //OliverESP: buffering
static size_t pointer; //OliverESP: buffering

Recorder::Recorder(ScopeView* scopeView)
{
	m_scopeView = scopeView;
	m_roster = 0;
	m_recordNode = 0;
	m_recVolume = 1.0;
	pointer = 0; // OliverESP

	SetRecVolume(((float)Preferences::Instance()->RecordVolume()/(float)100));

	status_t error;

	//	Make sure the media roster is there (which means the server is there).
	m_roster = BMediaRoster::Roster(&error);
	if (!m_roster){
		MediaProblem("error: BMediaRoster failed to initialise\n");
		printf("error: BMediaRoster failed to initialise\n");
	}

	if (m_roster->GetAudioInput(&m_audioInputNode) < B_OK){
		printf("error: No audio input found\n");
		MediaProblem("error: No audio input found\n");
		}

	//	Create our internal Node which records sound, and register it.
	m_recordNode = new SoundConsumer("Sound Recorder");
	if (m_roster->RegisterNode(m_recordNode) < B_OK)
	{
		printf("error: SoundConsumer not registered\n");
		MediaProblem("error: SoundConsumer not registered\n");
		if (m_recordNode)
			m_recordNode->Release();

	}
}

Recorder::~Recorder()
{
	//	The sound consumer and producer are Nodes; it has to be Release()d and the Roster
	//	will reap it when it's done.
	if (m_recordNode)
	{
		m_roster->UnregisterNode(m_recordNode);
		m_recordNode->Release();
	}
}


void
Recorder::Record()
{
	//	Hook up input
	status_t err = MakeRecordConnection(m_audioInputNode);
	if (err < B_OK) {
		printf("connect to the selected sound input\n");
		MediaProblem("Problem creating the record connection\n");
		return;
	}

	//	And get it going...
	bigtime_t then = m_recordNode->TimeSource()->Now()+50000LL;
	err = m_roster->StartNode(m_recordNode->Node(), then);
	if (err < B_OK) {
		printf("error starting the record node\n");
		MediaProblem("error starting the record node\n");
		return;
	}

	if (m_audioInputNode.kind & B_TIME_SOURCE) {
		m_roster->StartNode(m_audioInputNode, m_recordNode->TimeSource()->RealTimeFor(then, 0));
	}
	else {
		m_roster->StartNode(m_audioInputNode, then);
	}
}

status_t
Recorder::MakeRecordConnection(const media_node & input)
{
	//	Find an available output for the given input node.
	int32 count = 0;
	status_t err = m_roster->GetFreeOutputsFor(input, &m_audioOutput, 1, &count, B_MEDIA_RAW_AUDIO);
	if (err < B_OK) {
		printf("Recorder::MakeRecordConnection(): couldn't get free outputs from audio input node\n");
		return err;
	}
	if (count < 1) {
		printf("Recorder::MakeRecordConnection(): no free outputs from audio input node\n");
		return B_BUSY;
	}

// OliverESP: this is probable, but I think very improbable, we could make the buffer bigger
// OliverESP: but then we will lose performace. -> Moved here
if ( m_audioOutput.format.u.raw_audio.buffer_size > RTPPLLEN_RAW ) {

	char* msg = new char[220]; //OliverESP: 212B is the whole string -> printf("###%d\n",strlen(msg));

	sprintf(msg,"Our apologies\nWishper has problems setting the sound environment.\n\nPlease report this code number to oliver.ruiz.dorantes@gmail.com:\n   %08x%02x%1x%08x\nThanks!",
				(uint32)m_audioOutput.format.u.raw_audio.frame_rate,
				(uint32)m_audioOutput.format.u.raw_audio.format,
				(uint32)m_audioOutput.format.u.raw_audio.channel_count,
				(size_t)m_audioOutput.format.u.raw_audio.buffer_size);

	BAlert *myAlert = new BAlert("Known", msg,"Ok", NULL, NULL,
												B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT);
	myAlert->Go();
	delete msg;
	return m_audioOutput.format.u.raw_audio.buffer_size; //OliverESP: for example
}

	//	Find an available input for our own Node. Note that we go through the
	//	MediaRoster; calling Media Kit methods directly on Nodes in our app is
	//	not OK (because synchronization happens in the service thread, not in
	//	the calling thread).
	// TODO: explain this
	err = m_roster->GetFreeInputsFor(m_recordNode->Node(), &m_recInput, 1, &count, B_MEDIA_RAW_AUDIO);
	if (err < B_OK) {
		printf("Recorder::MakeRecordConnection(): couldn't get free inputs for sound recorder\n");
		return err;
	}
	if (count < 1) {
		printf("Recorder::MakeRecordConnection(): no free inputs for sound recorder\n");
		return B_BUSY;
	}

	//	Find out what the time source of the input is.
	//	For most nodes, we just use the preferred time source (the DAC) for synchronization.
	//	However, nodes that record from an input need to synchronize to the audio input node
	//	instead for best results.
	//	MakeTimeSourceFor gives us a "clone" of the time source node that we can manipulate
	//	to our heart's content. When we're done with it, though, we need to call Release()
	//	on the time source node, so that it keeps an accurate reference count and can delete
	//	itself when it's no longer needed.
	// TODO: what about filters connected to audio input?
	media_node use_time_source;
	BTimeSource * tsobj = m_roster->MakeTimeSourceFor(input);
	if (! tsobj) {
		printf("Recorder::MakeRecordConnection(): couldn't clone time source from audio input node\n");
		return B_MEDIA_BAD_NODE;
	}

	//	Apply the time source in effect to our own Node.
	err = m_roster->SetTimeSourceFor(m_recordNode->Node().node, tsobj->Node().node);
	if (err < B_OK) {
		printf("Recorder::MakeRecordConnection(): couldn't set the sound recorder's time source\n");
		tsobj->Release();
		return err;
	}

	//	Get a format, any format.
	media_format fmt;
	fmt.u.raw_audio = m_audioOutput.format.u.raw_audio;
	fmt.type = B_MEDIA_RAW_AUDIO;

	//	Tell the consumer where we want data to go.
	err = m_recordNode->SetHooks(DataEncode, NotifyDataEncode, this);
	if (err < B_OK) {
		printf("Recorder::MakeRecordConnection(): couldn't set the sound recorder's hook functions\n");
		tsobj->Release();
		return err;
	}

	//	Using the same structs for input and output is OK in BMediaRoster::Connect().
	err = m_roster->Connect(m_audioOutput.source, m_recInput.destination, &fmt, &m_audioOutput, &m_recInput);
	if (err < B_OK) {
		printf("Recorder::MakeRecordConnection(): failed to connect sound recorder to audio input node.\n");
		tsobj->Release();
		m_recordNode->SetHooks(0, 0, 0);
		return err;
	}

	//	Start the time source if it's not running.
	if ((tsobj->Node() != input) && !tsobj->IsRunning()) {
		m_roster->StartNode(tsobj->Node(), BTimeSource::RealTime());
	}
	tsobj->Release();	//	we're done with this time source instance!
	return B_OK;
}

status_t
Recorder::BreakRecordConnection()
{
	status_t err;

	//	If we are the last connection, the Node will stop automatically since it
	//	has nowhere to send data to.
	err = m_roster->StopNode(m_recInput.node, 0);
	err = m_roster->Disconnect(m_audioOutput.node.node, m_audioOutput.source, m_recInput.node.node, m_recInput.destination);
	m_audioOutput.source = media_source::null;
	m_recInput.destination = media_destination::null;
	return err;
}

status_t
Recorder::StopRecording()
{
	m_scopeView->Mute();

	BreakRecordConnection();
	m_recordNode->SetHooks(0, 0, 0);

	m_roster->StopNode(m_recordNode->Node(), 0);
	//m_roster->StartNode(m_audioInputNode, 0);   //OliverESP:Start??
	m_roster->StopNode(m_audioInputNode, 0);   //OliverESP:Start??

	return B_OK;
}

void
Recorder::DataEncode(void* cookie, bigtime_t timestamp, void* data, size_t size, const media_raw_audio_format& format)
{
#ifdef DEBUG
bigtime_t proc = real_time_clock_usecs();
#endif

#ifdef DEBUG_hook
				cout << "RECORDER -> HooK: " << size << endl;
#endif

#ifdef DEBUG_format
				cout << "FORMAT: rate    " << format.frame_rate << endl;
				cout << "FORMAT: channel " << format.channel_count << endl;
				cout << "FORMAT: format  " << format.format << endl;
				cout << "FORMAT: size    " << format.buffer_size << endl;
#endif

// OliverESP: this is probable, but I think very improbable, we could make the buffer bigger
// OliverESP: but then we will lose performace. -> Move this to makerecordconnection()
//if ( format.buffer_size > RTPPLLEN_RAW) {
//
//	char* msg = new char[220]; //OliverESP: 212B is the whole string -> printf("###%d\n",strlen(msg));
//
//	sprintf(msg,"Our apologies\nWishper will crash now. This is of course a known posibility.\n\nSurely you should restart the media services in Preferences.\n\nPlease report this code number to oliver.ruiz.dorantes@gmail.com:\n   %08x%02x%1x%08x\nThanks!",
//				(uint32)format.frame_rate,
//				(uint32)format.format,
//				(uint32)format.channel_count,
//				(size_t)format.buffer_size);
//
//	BAlert *myAlert = new BAlert("Known", msg,"Ok", NULL, NULL,
//												B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT);
//	myAlert->Go();
//	delete msg;
//}

// OliverESP: Building the Buffer
memcpy((void*)&dataR.data[pointer],data,size);
pointer += size;

// OliverESP: if The buffer is done
if (pointer == RTPPLLEN_RAW) {
	pointer = 0;
	size = RTPPLLEN_RAW;
	data = dataR.data;

	bool non_null_trame = true;
	TRtpData rtpdata; // 32776 total size: 32768 + 4 size_t + 4 int

//	int  nbr_skip = 4;
//	if(Preferences::Instance()->UseModemQuality())
//		nbr_skip = 6;

// OliverESP: New compression interface
// OliverESP: but we should not ask EACH hook about the compression... Preferences should comunicate us?
	int32 nbr_skip = Preferences::Instance()->Compression();


//	if(Preferences::Instance()->UseCodec())
	if(nbr_skip != 0)
	{
#ifdef FILTER
		if((non_null_trame = encodeur.decisionToEncode(data, size, nbr_skip, m_recVolume)) == true || last_non_null_trame)
		{
#endif
			int size_of_buf;

			// OliverESP: IMP,
			// char *buffer = encodeur.createBuffer(size, size_of_buf, nbr_skip);
			   size_of_buf = size / 4 / (nbr_skip*2);
			   char *buffer = rtpdata.data;

			encodeur.encodeBuffer(data, size, buffer, nbr_skip);
			data = (void*)buffer;
			//NetworkInterface::Instance()->Send(data, size);
			//OliverESP: Send calls receive and receive plays it...
			#ifdef DEBUG
				cout << "RECORDER: Encoded bytes: " << size_of_buf <<  " of " << size << " when? " << timestamp - lasttimestamp << " µsec " << endl;
				lasttimestamp = timestamp;
			#endif
			size = size_of_buf;
#ifdef FILTER
		}
#endif
	}

	// tell the network to send the voice data


	// OliverESP: The problem is: the max is RTPPLLEN_MAX 2048 but the hook sends us
	// 32768 bytes!
	// 1st solution, change the value of RTPPLLEN_MAX do what hook says.
	// OPS -> definitios.h §28 #define RTPPLLEN_RAW 		2048		// FIXME!

// OliverESP: improvement: we should by some way send the buffer here created
// and dont memcpy it again

//	memcpy(rtpdata.data, data, RTPPLLEN_MAX);  //OliverESP: temporal working
//	memcpy(rtpdata.data, data, size); // OliverESP: IMP,

	rtpdata.timestamp = CNetwork::Instance()->GetTimeStamp();
	rtpdata.size = size;

//  OliverESP: With the new compression interface a lot of the next code can be deleted.
//	As now is not important the packet type but the size
//	Also we will leave the interface open if is desired work without compression

	if (CNetwork::Instance()->IsConnectionReady())
	{
//		if(Preferences::Instance()->UseCodec())
		if (nbr_skip != 0 ) // there is compression
		{
#ifdef FILTER
			if(non_null_trame == true || last_non_null_trame){
#endif
//				if(Preferences::Instance()->UseModemQuality())
//  			{
//					if(send_data(find_thread(NET_THREAD_SEND_RTP), NET_ID_MODEM, &rtpdata, sizeof(TRtpData) - RTPPLLEN_MAX + size /* sizeof(TRtpData)*/) != B_NO_ERROR)
//						printf("FATAL ERROR BLEU SCREEN\n");
//				}
//				else
//				{
					if(send_data(find_thread(NET_THREAD_SEND_RTP), NET_ID_CODEC, &rtpdata, sizeof(TRtpData) - RTPPLLEN_MAX + size /* sizeof(TRtpData)*/) != B_NO_ERROR)
						printf("FATAL ERROR BLEU SCREEN\n");
//				}
#ifdef FILTER
			}
#endif
		}
		else	// OliverESP: There is still here the possibity to send FULL RAW audio!
		{
			if(send_data(find_thread(NET_THREAD_SEND_RTP), NET_ID_RAW, &rtpdata, sizeof(TRtpData)) != B_NO_ERROR)
				printf("FATAL ERROR BLEU SCREEN\n");
		}
	}

last_non_null_trame = non_null_trame; // keep information about the last

	//OliverESP: The scope will be updated only AFTER the data is sent!
	if (!m_scopeView->IsShrink())
		m_scopeView->NewData( (int16*) dataR.data , non_null_trame);

} // buffer creator
#ifdef DEBUG
	cout << real_time_clock_usecs() - proc << endl;
#endif

}


void
Recorder::NotifyDataEncode(void * cookie, int32 code, ...)
{
// cout << "Notified data encode" << endl;
}

void
Recorder::SetRecVolume(float volume)
{
	m_recVolume = volume;
}

void Recorder::MediaProblem(const char* string) {
		BString problem;

		problem << "There was an error setting the sound recording:\n\n" << string;

		BAlert *myAlert = new BAlert("Known", problem.String(),"Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT);
		myAlert->Go();

}
