#include "granularsampler.h"

int GranularSampler::Process(){
	int lstsample = 0;
	int cursample = 0;
	u32 lstoffset;
	u32 curoffset;
	this->GrainClockIndex += this->GrainDelta;
	if( this->GrainClockIndex > MAXPHASE || this->GrainClockIndex < 0 ){
		while( this->GrainClockIndex > MAXPHASE ) this->GrainClockIndex -= MAXPHASE;
		while( this->GrainClockIndex < 0 ) this->GrainClockIndex += MAXPHASE;
	}
	if( this->SampleIndex > MAXPHASE || this->SampleIndex < 0 ){
		this->LstSampleOffset = this->LastSamplePos;
		if( this->NewGrainSize >= 0 ){
			this->GrainSize = this->NewGrainSize;
			this->Delta = ( ( MAXPHASE / this->GrainSize ) / this->SampleRate ) * this->ClipSampleRate;
			this->Pitch = this->Delta * this->PitchNumerator / this->PitchDenominator;
			this->NewGrainSize = -1;
		}
		while( this->SampleIndex > MAXPHASE ) this->SampleIndex -= MAXPHASE;
		while( this->SampleIndex < 0 ) this->SampleIndex += MAXPHASE;
		this->CurSampleOffset = ( ( this->GrainClockIndex >> 20 ) * this->SampleLength ) >> 11;
		// --- //
		if( this->NewSampleOffset >= 0 ){
			this->CurSampleOffset = this->NewSampleOffset;
			this->NewSampleOffset = -1;
		}
		while( this->CurSampleOffset > this->SampleLength ) this->CurSampleOffset -= this->SampleLength;
		while( this->CurSampleOffset < 0 ) this->CurSampleOffset += this->SampleLength;
	}
	lstoffset =  this->LstSampleOffset + ( ( ( this->SampleIndex >> 15 ) * this->GrainSize ) >> 16 );
	curoffset =  this->CurSampleOffset + ( ( ( this->SampleIndex >> 15 ) * this->GrainSize ) >> 16 );
	
	lstsample = this->Clip[ lstoffset > this->SampleLength ? lstoffset - this->SampleLength : lstoffset ];
	cursample = this->Clip[ curoffset > this->SampleLength ? curoffset - this->SampleLength : curoffset ];
	this->LastSamplePos = curoffset > this->SampleLength ? curoffset - this->SampleLength : curoffset;
	lstsample = ( lstsample * ( ( MAXPHASE - this->SampleIndex ) >> 16 ) ) >> 15;
	cursample = ( cursample * ( ( this->SampleIndex ) >> 16 ) ) >> 15;
	this->SampleIndex += this->Pitch;
	return lstsample + cursample;
}
GranularSampler::GranularSampler( u32 srate, u32 slen, u32 sampsrate, s16* sample ){
	this->SampleRate = srate;
	this->ClipSampleRate = sampsrate;
	this->LstSampleOffset = 0;
	this->CurSampleOffset = 0;
	this->SampleIndex = 0;
	this->SampleLength = slen;
	this->GrainSize = srate/10;
	this->Delta = ( ( MAXPHASE / this->GrainSize ) / this->SampleRate ) * this->ClipSampleRate;
	this->Pitch = this->Delta;
	this->Loop = TRUE;
	this->NewGrainSize = -1;
	this->NewSampleOffset = -1;
	this->GrainClockIndex = 0;
	this->GrainDelta = ( ( MAXPHASE / this->SampleLength ) * this->ClipSampleRate ) / this->SampleRate;
	this->PitchNumerator = 1;
	this->PitchDenominator = 1;
	this->LastSamplePos = 0;
	this->Clip = sample;
}
void GranularSampler::SetPlaybackRate( u32 numerator, u32 denominator ){
	this->GrainDelta = ( ( ( ( MAXPHASE / this->SampleLength ) * this->ClipSampleRate ) / this->SampleRate ) * numerator ) / denominator;
}
void GranularSampler::SetPlaybackPos( u32 numerator, u32 denominator ){
	this->NewSampleOffset = this->SampleLength * numerator / denominator;
	while( this->NewSampleOffset > (int)this->SampleLength ) this->NewSampleOffset -= (int)this->SampleLength;
	while( this->NewSampleOffset < 0 ) this->NewSampleOffset += (int)this->SampleLength;
}
void GranularSampler::SetPitch( u32 numerator, u32 denominator ){
	this->PitchNumerator = numerator;
	this->PitchDenominator = denominator;
	this->Pitch = this->Delta * numerator / denominator;
}
void GranularSampler::SetGrainSize( u32 ms ){
	this->NewGrainSize = this->ClipSampleRate * ms / 1000 + 1;
}
void GranularSampler::SetLoop( u8 loop ){
	this->Loop = loop;
}
