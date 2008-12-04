//#define DEBUG
//#define DEBUG_hook
//#define DEBUG_format
#define FILTER

#ifndef _Recorder2_h
#include "Recorder2.h"
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
#include <Debug.h>

#include <MediaAddOn.h>
#include <MediaRoster.h>
#include <locale/Locale.h>

#include "MediaRecorder.h"

#define CHECK_BUFFER

typedef struct Sample16 {

	int16 left;
	int16 right;

} Sample16;

ScopeView* m_scopeView;
Encode encodeur;
float m_recVolume;
static bigtime_t 		lasttimestamp;       // OliverESP: last timestamp
static bool 			last_non_null_trame; // Last trame silence condition

static TRtpData dataR; //OliverESP: buffering
static size_t pointer; //OliverESP: buffering
media_format fmt;

Recorder::Recorder(ScopeView* scopeView)
{
	status_t err = B_OK;
	mRecorder  = new BMediaRecorder("SimpleRecord");
	mDevice    = 0;
	AudioOK	   = false;
	mRecording = false;

	mRecorder->SetHook(this, RecordHook);

// OliverESP: from old engine
	m_scopeView = scopeView;

	m_recordNode = 0;
	m_recVolume = 1.0;
	pointer = 0; // OliverESP

	SetRecVolume(((float)Preferences::Instance()->RecordVolume()/(float)100));
// OliverESP: from old engine

////////////////////////////
   m_roster = BMediaRoster::Roster(&err);

    if (!m_roster || (err != B_OK)) {
		MediaProblem("media_server is not running\n");
		return;
	}

	err = BMediaRoster::Roster()->GetAudioInput(&m_audioInputNode);

	if (err < B_OK ) {
		MediaProblem("No audio input found\n");
		return;
	}
	else {
		if (CheckNoneInput()) {
			BAlert *myAlert = new BAlert("Hmmm", _T("We found a 'None In' input in the system\nRecording features disabled") , _T("Ok"), NULL, NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_INFO_ALERT);
			myAlert->Go();
			return;
			}
		else {
			BMessage* msg = new BMessage(DEVICE);
			msg->AddInt32("node_id", m_audioInputNode.node);
			SelectDevice(msg);
			AudioOK = true;
		}
	}
////////////////////////////


//	Make sure the media roster is there (which means the server is there).
//	m_roster = BMediaRoster::Roster(&error);
//	if (!m_roster){
//		MediaProblem("error: BMediaRoster failed to initialise\n");
//		printf("error: BMediaRoster failed to initialise\n");
//	}

//	if (m_roster->GetAudioInput(&m_audioInputNode) < B_OK){
//		printf("error: No audio input found\n");
//		MediaProblem("error: No audio input found\n");
//		}
//
	//	Create our internal Node which records sound, and register it.
//	m_recordNode = new SoundConsumer("Sound Recorder");
//	if (m_roster->RegisterNode(m_recordNode) < B_OK)
//	{
//		printf("error: SoundConsumer not registered\n");
//		MediaProblem("error: SoundConsumer not registered\n");
//		if (m_recordNode)
//			m_recordNode->Release();
//
//	}
}

Recorder::~Recorder()
{
	if (mRecording) {
		StopRecording();
	}
//	CloseFile();
	if (mDevice != 0) {
		BMediaRoster::CurrentRoster()->ReleaseNode(*mDevice);
		delete mDevice;
	}

//OLD ENGiNE
	//	The sound consumer and producer are Nodes; it has to be Release()d and the Roster
	//	will reap it when it's done.

//	if (m_recordNode)
//	{
//		m_roster->UnregisterNode(m_recordNode);
//		m_recordNode->Release();
//	}
//OLD ENGiNE

}

void
Recorder::StartRecording()
{
	if (mRecording) {
		return;
	}

	if (!AudioOK)
		return;
	// connect thing if not connected
	if (!ConnectIt()) {
		return;
	}
//mFIFO.Reset();
	// start the thread
/*	mThread = spawn_thread(buffer_flusher, "buffer_flusher", 100, this);
	if (mThread < B_OK) {
		fprintf(stderr, "could not spawn buffer_flusher\n");
		(new BAlert("", strerror(mThread), "Stop"))->Go();
		return;
	}
*/	// set mRecording to true, and start recorder
	mRecording = true;
/*	status_t err = resume_thread(mThread);
	if (err < B_OK) {
		fprintf(stderr, "could not resume buffer_flusher\n");
		kill_thread(mThread);
		mThread = -1;
		(new BAlert("", strerror(err), "Stop"))->Go();
		return;
	}
*/
}
void
Recorder::StopRecording()
{
	m_scopeView->Mute();

//	BreakRecordConnection();
//	m_recordNode->SetHooks(0, 0, 0);

//	m_roster->StopNode(m_recordNode->Node(), 0);
//	m_roster->StartNode(m_audioInputNode, 0);   //OliverESP:Start?? YES! or nothing!
	//m_roster->StopNode(m_audioInputNode, 0);

///////////////

	// set mRecording to false
	mRecording = false;
	// disconnect thing
	status_t err = mRecorder->Disconnect();
	if (err < B_OK) {
		fprintf(stderr, "Disconnect() failed\n");
	//	(new BAlert("", strerror(err), "Stop"))->Go();  // To avoid the message at closing
	}

//	mFIFO.CopyNextBufferIn(&status, 0, B_INFINITE_TIMEOUT, true);
//	wait_for_thread(mThread, &status);

}

void
Recorder::SelectDevice(BMessage* msg)
{
	if (mRecording) {
		return;
	}
//	CloseFile();
/*	if (!mDevWin) {
		mDevWin = new DevWindow(this);
		mDevWin->Show();
		return;
	}
*/

	int32 node_id = -1;
	status_t n_err, d_err;
	ssize_t sz;
	const void * dnip = 0;
	if ((n_err = msg->FindInt32("node_id", &node_id)) && (d_err = msg->FindData("dormant_info", B_RAW_TYPE, &dnip, &sz))) {
/*		mDevWin->Lock();
		if (mDevWin->IsHidden()) {
			mDevWin->Show();
		}
		else {
			mDevWin->Activate();
		}
		mDevWin->Unlock();
		return;
*/
	}
	media_node instantiated;
	if (n_err < B_OK) {
		d_err = BMediaRoster::CurrentRoster()->InstantiateDormantNode(
			*reinterpret_cast<const dormant_node_info *>(dnip),
			&instantiated, B_FLAVOR_IS_GLOBAL);
		if (d_err < B_OK) {
			fprintf(stderr, "InstantiateDormantNode() failed: %s\n", strerror(d_err));
			(new BAlert("", strerror(d_err), "Stop"))->Go();
			return;
		}
	}
	if (mDevice != 0) {
		BMediaRoster::CurrentRoster()->ReleaseNode(*mDevice);
		delete mDevice;
		mDevice = 0;
	}
	mDevice = new media_node;
	if ((node_id >= 0) && !n_err) {
		status_t err = BMediaRoster::Roster()->GetNodeFor(node_id, mDevice);
		if (err < B_OK) {
			fprintf(stderr, "GetNodeFor() failed\n");
			(new BAlert("", strerror(err), "Stop"))->Go();
			delete mDevice;
			mDevice = 0;
			return;
		}
	}
	else {
		ASSERT(instantiated.node >= 0);
		*mDevice = instantiated;
	}
	if (mRecorder->IsConnected()) {
		mRecorder->Disconnect();
	}
	ConnectIt();
}

bool Recorder::CheckNoneInput(){

	status_t err;
	int32 nodeCount = 10;
	media_output output[10];


   	err = BMediaRoster::Roster()->GetFreeOutputsFor(m_audioInputNode, output, 10, &nodeCount);
	if (err != B_OK) {
		MediaProblem("Impossible to check the input");
		//return err;
	}
	if (nodeCount < 1) {
		MediaProblem("Impossible to check the input");
	}
	printf("Name of input '%s'\n",output[0].name);

	return (0 == strncmp("Audio In null",output[0].name,strlen("Audio In null")));

}

bool
Recorder::ConnectIt()
{
	// connect thing if not connected
	status_t err = B_OK;
	//media_format fmt;
	fprintf(stderr, "ConnectIt(): %s\n", mRecorder->IsConnected() ? "already connected" : "not yet connected");
	if (!mRecorder->IsConnected()) {
		fmt.type = B_MEDIA_RAW_AUDIO;
		char str[200];
		SetCodecFormat(fmt); // OliverESP
		string_for_format(codecformat, str, 200);
		fprintf(stderr, "Demanded format %s\n", str);
		if (mDevice != 0) {
			fprintf(stderr, "mDevice is non-NULL, connecting to id %ld\n", mDevice->node);
			mRecorder->Connect(*mDevice, 0, &fmt);    		  // THE CONNECTION
//			mRecorder->Connect(*mDevice, 0, &codecformat);    // THE CONNECTION
		}
		else {
			fprintf(stderr, "connecting to default for format\n");
			err = mRecorder->Connect(fmt);
		}
		if (err < B_OK) {
			fprintf(stderr, "cannot connect recorder to input\n");
			(new BAlert("", strerror(err), "Stop"))->Go();
			return false;
		}
		fmt = mRecorder->Format();
		string_for_format(fmt, str, 200);
		fprintf(stderr, "connect format %s\n", str);

#if 0
if (format_is_compatible(fmt,codecformat))
			(new BAlert("", "true returned is compatible", "Hmmm"))->Go();
else
			(new BAlert("", "false returned is not compatible", "Hmmm"))->Go();
#endif

#ifdef CHECK_BUFFER
		// OliverESP: this is probable, but I think very improbable, we could make the buffer bigger
		// OliverESP: but then we will lose performace.
		if ( fmt.u.raw_audio.buffer_size > RTPPLLEN_RAW ) {

			char* msg = new char[220]; //OliverESP: 212B is the whole string -> printf("###%d\n",strlen(msg));

			sprintf(msg,"Our apologies\nWishper has problems setting the sound environment.\n\nPlease report this code number to oliver.ruiz.dorantes@gmail.com:\n   %08x%02x%1x%08x\nThanks!",
						(uint32)fmt.u.raw_audio.frame_rate,
						(uint32)fmt.u.raw_audio.format,
						(uint32)fmt.u.raw_audio.channel_count,
						(size_t)fmt.u.raw_audio.buffer_size);

			BAlert *myAlert = new BAlert("Known", msg,"Ok", NULL, NULL,
												B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT);
			myAlert->Go();
			delete msg;
			return false;
		}
#endif

		mFrameSize = (fmt.u.raw_audio.format & 0xf)*fmt.u.raw_audio.channel_count;
		fprintf(stderr, "frame size: %ld\n", mFrameSize);
		fprintf(stderr, "data rate: %.3f MB/s\n", fmt.u.raw_audio.frame_rate*mFrameSize/(1024.0*1024.0));
//		mPeakFormat = fmt.u.raw_audio.format;
		mRecorder->Start();
	}
	return true;
}

void
Recorder::SetCodecFormat(media_format m)
{
   codecformat = m;
   codecformat.u.raw_audio = m.u.raw_audio;
   codecformat.type = B_MEDIA_RAW_AUDIO;

   codecformat.u.raw_audio.frame_rate = 48000;
   codecformat.u.raw_audio.channel_count = 2;
   codecformat.u.raw_audio.byte_order = B_MEDIA_HOST_ENDIAN;
   codecformat.u.raw_audio.buffer_size = 8*1024;
   codecformat.u.raw_audio.format = 0x2;
}

void
Recorder::RecordHook(void *cookie, void *data, size_t size, const media_header &header)
//void
//Recorder::DataEncode(void* cookie, bigtime_t timestamp, void* data, size_t size, const media_raw_audio_format& format)
{
uint32 offset;

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

// OliverESP: Building the Buffer


// OliverESP: Building the Buffer
// ReFormating possible 8bitted format
if ( fmt.u.raw_audio.format == (uint32) 0x1  /*B_AUDIO_CHAR*/ ) {

	offset = 0;
	Sample16* sample;

	while (offset < size) {

		sample = (Sample16*) &dataR.data[pointer];
		sample->left =  ((char*)data)[offset] << 8;
		sample->right = ((char*)data)[offset+1] << 8;

	    offset += 2;	//  8 bits left & right
		pointer += 4;   // 16 bits left & right
	}
	//printf("Number of samples processed %ld, information processed %ld, Sample16:%d\n",offset ,size*2,sizeof(Sample16));

} else {

	memcpy((void*)&dataR.data[pointer],data,size);
	pointer += size;

}
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
// OliverESP: Or each X seconds....?

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
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
////////////OLD STUFF/////////////////
//////////////////////////////////////
//////////////////////////////////////

#if 0
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
// OliverESP: but then we will lose performace. -> Move this to makerecordconnection()
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
//	err = m_recordNode->SetHooks(DataEncode, NotifyDataEncode, this);
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

void
Recorder::RecordHook(void *cookie, void *data, size_t size, const media_header &header)
{
	// fprintf(stderr,"Record: Size %d\n",size);

	Recorder * rw = (Recorder *)cookie;
//fprintf(stderr, "RecordHook: %s\n", rw->mRecording ? "true" : "false");
	if (rw->mRecording) {
//		status_t err = rw->mTrack->WriteFrames(data, size /* /rw->mFrameSize */, B_MEDIA_KEY_FRAME);

//		status_t err = rw->mFIFO.CopyNextBufferIn(data, size, B_INFINITE_TIMEOUT, false);
		status_t err = size;
		if (err < (int32)size) {
			fprintf(stderr, "CopyNextBufferIn(%p, %ld, B_INFINITE_TIMEOUT, false) failed with %ld.\n",
				data, size, err);
			fprintf(stderr, "error while recording: %s; bailing\n", strerror(err));
			rw->mRecording = false;
		}
	}
	//	do peak data
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

void
Recorder::NotifyDataEncode(void * cookie, int32 code, ...)
{
// cout << "Notified data encode" << endl;
}
#endif
