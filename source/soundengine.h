#ifndef SOUNDENGINE
#define SOUNDENGINE

#include "blackbox.h"
#include "granularsampler.h"
#include "module.h"
#define GLOBALBUFFERSIZE 3000
#define INDEXFIXEDPOINT 16

class SoundEngine : public BlackBox {
  public:
    SoundEngine(int sr){
      int sample = 0;
      lastSample = 0;
      clamp = MAXAMP;
      this->sampleTime = 0;
      this->numvoices = 1;
      this->oscs = (SinOsc**)malloc( sizeof( SinOsc* ) * this->numvoices );
      for( int i = 0; i < this->numvoices; i++ ){
        this->oscs[i] = new SinOsc( sr );
      }
      // -- fill buffer -- //
      this->oscs[0]->SetAmplitude(MAXAMP/2);
      this->polySeq = new PolySeq(sr);
      this->simpleSynth = new SimpleSynth(sr);
      r2rA = new Reel2Reel(sr);
      r2rB = new Reel2Reel(sr);
      r2rA->tapeColor = RGB15(15,31,15);
      r2rB->tapeColor = RGB15(15,15,31);
      sampleEditor = new SampleEditor(sr);
    }
    //void playnote( int note, int voice );
    //void stop( int voice );
    int Process(){
      int sample = 0;
      sample += r2rA->Process(lastSample);
      sample += r2rB->Process(lastSample);
      sample += simpleSynth->Process();
      sample += polySeq->Process();
      sample += sampleEditor->Process();
      if(abs(sample) > MAXAMP) {
        clamp = (MAXAMP*MAXAMP)/abs(sample);
      }
      if(clamp < MAXAMP) clamp++;
      lastSample = sample;
      this->sampleTime++;
      return sample;
    }
    u32 sampleTime;
    int clamp;
    Reel2Reel* r2rA;
    Reel2Reel* r2rB; 
    SimpleSynth* simpleSynth;
    PolySeq* polySeq;
    SampleEditor* sampleEditor;
  private:
    int lastSample;
    int numvoices;
    SinOsc** oscs;
};
#endif
