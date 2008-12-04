#ifndef _BeNetApp_h
#include "BeNetApp.h"
#endif

#include <Entry.h>	// just for the patch
#include <Alert.h>	// just for the patch

int main(int nArg, char** ppchArg)
{
	BeNetApp* theApp = new BeNetApp();

	BEntry ac97("/dev/audio/multi/ich_ac97/1");
//	BEntry ac97("/dev/audio/maestro2/1"); //my card

	if (ac97.Exists()){
			BAlert *myAlert = new BAlert("Known", "Whisper detects an AC97 soundcard on your system. At the moment this driver does not support recording audio.","Ok", NULL, NULL, 
												B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_STOP_ALERT); 
			myAlert->Go();
			delete theApp;
	}else{
			
		theApp->Run();
		return 0;
		
	}
}
