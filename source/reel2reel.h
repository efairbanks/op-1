#ifndef REEL2REEL
#define REEL2REEL

#include <nds.h>

#include "blackbox.h"
#include "soundengine.h"
#include "graphicseng.h"

class Reel2Reel : public BlackBox {
public:
  Reel2Reel(int sr) {
    recBufIndex = 0;
    recBufSize = sr * 5;
    recBuf = (s16*)malloc(recBufSize * sizeof(s16));
    playHeadSpeed = 0;
    playHeadDelta = 0;
    playing = false;
    recording = false;
  }
  int Process(int input) {  
	  int sample = 0;
    sample = recBuf[(int)recBufIndex];
    if(recording) recBuf[(int)recBufIndex] = input; // make sure to interpolate
	  sample = sample;
    playHeadDelta += (playHeadSpeed-playHeadDelta)*0.00025;
    recBufIndex += playHeadDelta;
    if(recBufIndex > recBufSize) recBufIndex -= recBufSize;
    if(recBufIndex < 0) recBufIndex += recBufSize;
	  return recording ? 0 : sample; // maybe should not auto mute when recording? tape delay?
  }
  int Process() {
    return Process(0);
  }
  float recBufIndex;
  int recBufSize;
  s16* recBuf;
  float playHeadSpeed;
  bool playing;
  bool recording;
private:
  float playHeadDelta;
};
#endif
