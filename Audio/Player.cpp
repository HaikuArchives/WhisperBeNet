//#define DEBUG

#ifndef _Player_h
#include "Player.h"
#endif

#ifndef _Decode_h
#include "Decode.h"
#endif

//#ifndef _Encode_h	// OliverESP: we should not need this here
//#include "Encode.h"
//#endif

#ifndef _NetworkInterface_h
#include "NetworkInterface.h"
#endif

#ifndef Preferences_h		//OliverESP: for the codecs configuration... also not needed
#include "Preferences.h"
#endif

#include "network.h"

Decode decodeur;
//Encode encodeur;   // OliverESP: we should not need this here

Player::Player(ScopeView* scopeView) :
	m_scopeView(scopeView),
	m_playVolume(1.0)
{
	// tell the network who is the player.
	CNetwork::Instance()->SetPlayer(this);
	NetworkInterface::Instance()->SetPlayer(this);


	m_player.Start();
	m_player.SetVolume(m_playVolume); 
}

Player::~Player()
{
	m_player.Stop();

	// tell the network that he is going to sleep
	CNetwork::Instance()->SetPlayer(NULL);
}


//  OliverESP: this method is not used around the code
//  I guess developers wanted here to add the posibility
//  to play encoded packets. So PlayOld() for old data
//  Play() ... 
void Player::Play(void* data, size_t size)  // we need type of packet and size.
{
	int nbr_skip = 4;

	// OliverESP thinks is better to know the codification from the packet
	//			 and not from the local preferences(just sending pref)
	// 			 as in the old version:
	//      	 int nbr_skip = 32768 / size / 8; // OliverESP: 8kHz!!!
	if(Preferences::Instance()->UseModemQuality())
		nbr_skip = 6;
	if(Preferences::Instance()->UseCodec())
	{
		size_t size_buf;

//		char* buffer = encodeur.createBuffer(size, size_of_buf, nbr_skip);
//		encodeur.encodeBuffer(data, size, buffer, nbr_skip);
//		decodeur.decodeBuffer(buffer, size_of_buf, data, nbr_skip);
//		delete buffer;
	
		char *buffer = (char*) data;
		void *result = decodeur.createBuffer(size, size_buf, nbr_skip); // for 32kb bucket nbr_skip = 0
		decodeur.decodeBuffer(buffer, size, result, nbr_skip);
		data = result;
		size = size_buf;	
	}


/*	int16* j = (int16*)data;
	for (int i = 0; i < 100; i++)
		printf("%i", j[i]);
	printf("\n");
*/	
	if (!m_scopeView->IsShrink())
		m_scopeView->NewData( (int16*)data, true );

	BSound *sound; 
	BSoundPlayer::play_id id;

	media_raw_audio_format format;
	format.frame_rate = 48000;
	format.channel_count = 2;
	format.format = 0x2;
	format.byte_order = B_MEDIA_LITTLE_ENDIAN;
//	format.buffer_size = 2048;                 // <-- hmmm
	format.buffer_size = size;                 // size after codifications should be 32KB

//	sound = new BSound(data, 2048, format);    // <-- 2048 hmmm
	sound = new BSound(data, size, format);    // size after codifications
	if (sound->InitCheck() == B_OK)
	{
		id = m_player.StartPlaying(sound); 
		sound->ReleaseRef();
	}
}


void Player::PlayOld(void* data, size_t size,int packetype)
{


int nbr_skip;

//if (size == 2048 || size == 64 || size == 42)
//		nbr_skip = 2048 / size / 8;
//else
//if (size == 1024 || size == 32 || size == 21)
//		nbr_skip = 1024 / size / 8;
//else
		nbr_skip = 32768 / size / 8;
		
//	int nbr_skip = 2048 / size / 8;  // for 32kb bucket(size) -> 0
//	int nbr_skip = 32768 / size / 8; // OliverESP: 8kHz!!!
	
//	if(size != 2048)
	if(size != 32768 /* && size != 2048 && size != 1024*/)	// OliverESP: if not encoded bucket ?
	{
		size_t size_buf;
		char *buffer = (char*)data;

		void *result = decodeur.createBuffer(size, size_buf, nbr_skip); // for 32kb bucket nbr_skip = 0

#ifdef DEBUG
	cout << "PLAYER: decoded: " << size_buf << " of " << size << " nbr: " << nbr_skip << endl;	
#endif		
		decodeur.decodeBuffer(buffer, size, result, nbr_skip);
		data = result;
		size = size_buf;	

	}

if (!m_scopeView->IsShrink())
	m_scopeView->NewData( (int16*)data, true );

	BSound *sound; 
	BSoundPlayer::play_id id;

	media_raw_audio_format f;
	f.frame_rate = 48000;
	f.channel_count = 2;
	f.format = 0x2;
	f.format = media_raw_audio_format::B_AUDIO_SHORT;
	f.byte_order = B_MEDIA_LITTLE_ENDIAN;
	f.buffer_size = size;

	sound = new BSound(data, size, f); 
	if (sound->InitCheck() == B_OK)
	{
		id = m_player.StartPlaying(sound); 
		sound->ReleaseRef();
	}

}


void
Player::SetPlayVolume(float volume)
{
	m_playVolume = volume;
	m_player.SetVolume(volume);
}
