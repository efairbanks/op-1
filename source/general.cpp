#include "general.h"

extern bool pauseAudio;

u8 sindex = 0;
u8 inc = 0;
u8 filter = 0;
int ym1 = 0;
BlackBox* GenSoundEngine;

WAVheader* loadWAV( char* filename ){
	fatInitDefault();
	FILE* wavfile = fopen( filename, "rb" );
	if( wavfile == NULL ) return NULL;
	WAVheader* wav = (WAVheader*)malloc( sizeof( WAVheader ) );
	if( fread( wav, 1, sizeof( WAVheader ), wavfile ) == 0 ) return NULL;
	return wav;
}
int processsound(){
	return GenSoundEngine->Process();
}
mm_word on_stream_request( mm_word length, mm_addr dest, mm_stream_formats format ){
//----------------------------------------------------------------------------------
	s16* target = (s16*)dest;
	int len = length;
	for( ; len; len-- ){
		int sample;
		sample = processsound();//ym1;
		//ym1 = ( ((sample*filter)>>8) + ((ym1*(256-filter)>>8)) );
		//sample = ym1;
		// output sample for left
		*target++ = sample;
		// output inverted sample for right
		*target++ = -sample;
	}
	return length;	
}

void SoundSetup( BlackBox* sndeng, int srate, int bufsize ){
	GenSoundEngine = sndeng;
	mm_ds_system* sys = (mm_ds_system*)malloc( sizeof( mm_ds_system) );
	
	sys->mod_count 			= 0;
	sys->samp_count			= 0;
	sys->mem_bank			= 0;
	sys->fifo_channel		= FIFO_MAXMOD;
	
	mmInit( sys );
	
	//----------------------------------------------------------------
	// open stream
	//----------------------------------------------------------------
	
	mm_stream* mystream = (mm_stream*)malloc( sizeof( mm_stream ) );

	mystream->sampling_rate	= srate;					// sampling rate = 25khz
	mystream->buffer_length	= bufsize;					// buffer length = 1200 samples
	mystream->callback		= on_stream_request;		// set callback function
	mystream->format		= MM_STREAM_16BIT_STEREO;	// format = stereo 16-bit
	mystream->timer			= MM_TIMER0;				// use hardware timer 0
	mystream->manual		= false;					// use automatic filling
	mmStreamOpen( mystream );
}

void SoundRestart(int srate, int bufsize) {
  mmStreamClose();
/*
	mm_stream* mystream = (mm_stream*)malloc( sizeof( mm_stream ) );

	mystream->sampling_rate	= srate;					// sampling rate = 25khz
	mystream->buffer_length	= bufsize;					// buffer length = 1200 samples
	mystream->callback		= on_stream_request;		// set callback function
	mystream->format		= MM_STREAM_16BIT_STEREO;	// format = stereo 16-bit
	mystream->timer			= MM_TIMER0;				// use hardware timer 0
	mystream->manual		= false;					// use automatic filling
	mmStreamOpen( mystream );
*/}

int ipower( int var, int power ){
	if( power < 1 ) return 1;
	for( int i = 0; i < power; i++ ) var *= var;	
	return var;
}
bool recttest( int x, int y, int rx, int ry, int rwidth, int rheight ){
	if( ( x > rx && x < rx + rwidth ) && ( y > ry && y < ry + rheight ) ) return true;
	return false;
}
