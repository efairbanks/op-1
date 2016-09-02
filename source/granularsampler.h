#include <nds.h>
#include <stdio.h>
#include "general.h"
#include "blackbox.h"

class GranularSampler : public BlackBox {
public:
	int Process();
	GranularSampler( u32 srate, u32 slen, u32 sampsrate, s16* sample );
	void SetPlaybackPos( u32 numerator, u32 denominator );
	void SetPlaybackRate( u32 numerator, u32 denominator );
	void SetPitch( u32 numerator, u32 denominator );
	void SetGrainSize( u32 ms );
	void SetLoop( u8 loop );
protected:
	u32 SampleRate;
	u32 ClipSampleRate;
	u32	LstSampleOffset;	
	u32	CurSampleOffset;
	u32	SampleIndex;
	u32 Delta;
	u32	SampleLength;
	int Pitch;
	u32 GrainSize;
	u8	Loop;
	int NewGrainSize;
	int NewSampleOffset;
	int PitchNumerator;
	int PitchDenominator;
	u32 LastSamplePos;
	u32 GrainClockIndex;
	u32 GrainDelta;
	s16* Clip;
};
