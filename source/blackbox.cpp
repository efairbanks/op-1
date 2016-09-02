#include "blackbox.h"
#include <nds.h>
#include <maxmod9.h>

int Oscillator::SetFreqHZ( int hz ){
	if(this->samplerate == 32768) {
    this->delta = (MAXPHASE>>15)*hz;
  }
  else {
    this->delta = (MAXPHASE/this->samplerate)*hz;
  }
	return this->delta;
}
	
int Oscillator::SetFreqMS( int ms ){
	this->delta = (MAXPHASE/((this->samplerate*ms)/1000));
	return this->delta;
}
int Oscillator::SetAmplitude( int amp ){
	this->amplitude = amp;
	return this->amplitude;
}
int Oscillator::SetPhase( int p ){
	this->phase = p;
	return this->phase;
}
Phasor::Phasor( int sr ){
	this->samplerate = sr;
	this->phase = 0;
	this->amplitude = 0;
	this->delta = 0;
}
int Phasor::Process(){
	int sample = 0;
	this->phase += this->delta;
	
	while( this->phase >= MAXPHASE ) this->phase -= MAXPHASE;
	sample = ((this->phase>>16)*amplitude)>>15;
	
	return sample;
}
FeedBackDelay::FeedBackDelay(int sr, int maxdelay){
	this->samplerate = sr;
	this->buffersize = (sr*maxdelay)/1000;
	this->amplitude  = MAXAMP;
	this->buffer     = (int*)malloc( this->buffersize*sizeof(int) );
	memset( this->buffer, 0, this->buffersize*sizeof(int) );
	this->delta = 0;
	this->feedback = 0;
	this->playcursor = 0;
}
int FeedBackDelay::Process(){
	int out = 0;
	out = this->signal + this->buffer[this->playcursor];
	this->buffer[this->playcursor] = ( out * this->feedback ) >> 15;
	this->playcursor++;
	while( this->playcursor > delta ) this->playcursor -= delta;
	return ( out * this->amplitude ) >> 15;
}
int FeedBackDelay::Signal( int sig ){
	this->signal = sig;
	return this->signal;
}
int FeedBackDelay::SetAmplitude( int amp ){
	this->amplitude = amp;
	return this->amplitude;
}
int FeedBackDelay::SetDelay( int ms ){
	this->delta = (this->samplerate*ms)/1000;
	return this->delta;
}
int FeedBackDelay::SetFeedBack( int fb){
	this->feedback = fb;
	return this->feedback;
}
SinOsc::SinOsc( int sr ){
	this->samplerate = sr;
	this->phase = 0;
	this->amplitude = 0;
	this->delta = 0;
}
int SinOsc::Process(){
	int sample = 0;
  if(this->amplitude == 0) return 0;
	this->phase += this->delta;
	while( this->phase >= MAXPHASE ) this->phase -= MAXPHASE;
	sample = (sinLerp(this->phase >> 16) * this->amplitude) >> 12;
	return sample;
}
