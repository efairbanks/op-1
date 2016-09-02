#ifndef MODULE
#define MODULE

#define MAXCLOCK 1<<24

#include <nds.h>

#include "blackbox.h"
#include "soundengine.h"
#include "graphicseng.h"

extern s16* clipBoard;
extern int clipBoardLength;
extern bool pauseAudio;

inline void DrawOffline(gLayer layer);
inline void DrawReel(int x, int y, int angle, int radius, gLayer layer);
inline void DrawReel2Reel(int position, gLayer layer);
inline void DrawTimeline(u32 sampleIndex, u32 markerA, u32 markerB, u32 bufferSize, int offset, int height, int markerSpacing, u16 tapeColor, gLayer layer);
inline bool TestRect(int x, int y, int width, int height, touchPosition touch);

class Module {
  public:
    bool offline;
    virtual void HandleButtons(u32,u32,u32,u32) = 0;
    virtual void HandleTouchScreen(u32,u32,u32,touchPosition,u32) = 0;
    virtual void Draw(gLayer layer) = 0;
    virtual void AsyncProcess() = 0;
};

class SampleEditor : public BlackBox, public Module {
  public:
    int sampleIndex;
    SampleEditor(int sr) {
      sampleIndex = -1;
      markerA = 0;
      markerB = 255;
      command = NO_COMMAND;
    }
    int Process() {
      int sample = 0;
      if(sampleIndex<clipBoardLength && sampleIndex>-1) sample = clipBoard[sampleIndex++];
      if(sampleIndex>=clipBoardLength) sampleIndex = -1;
      return sample;
    }
    void Draw(gLayer layer) {
      u8 selectionStart = markerA > markerB ? markerB : markerA;
      u8 selectionEnd = markerA > markerB ? markerA : markerB;
      GraphicsEng::Instance()->DrawLine(
          selectionStart,0,
          selectionStart,SCREEN_HEIGHT,
          RGB15(8,15,8),
          ALPHA_ON,
          layer
          );
      GraphicsEng::Instance()->DrawLine(
          selectionEnd,0,
          selectionEnd,SCREEN_HEIGHT,
          RGB15(8,15,8),
          ALPHA_ON,
          layer
          );
      for(int i=0; i<SCREEN_WIDTH; i++) {
        int maxAmp=0, minAmp=0;
        for(int j=i*clipBoardLength/SCREEN_WIDTH; j<(i+1)*clipBoardLength/SCREEN_WIDTH; j++) {
          if(clipBoard[j]>0) {
            if(clipBoard[j]>maxAmp) maxAmp = clipBoard[j];
          } else {
            if(clipBoard[j]<minAmp) minAmp = clipBoard[j];
          }
        }
        GraphicsEng::Instance()->DrawPoint(
            i,(maxAmp+MAXAMP)*SCREEN_HEIGHT/(MAXAMP*2),
            RGB15(31,31,31),
            ALPHA_ON,
            layer
            );
        GraphicsEng::Instance()->DrawPoint(
            i,(minAmp+MAXAMP)*SCREEN_HEIGHT/(MAXAMP*2),
            RGB15(31,31,31),
            ALPHA_ON,
            layer
            );
      }
      // --- //
      if(offline) DrawOffline(layer);
    }
    enum Command {
      NO_COMMAND,
      FADE_IN,
      FADE_OUT,
      TRIM
    };
    void AsyncProcess() {
      int selectionStart;
      int selectionEnd;
      switch(command) {
        case FADE_IN:
          pauseAudio = true;
          selectionStart = markerA > markerB ? markerB : markerA;
          selectionEnd = markerA > markerB ? markerA : markerB;
          selectionStart = selectionStart*clipBoardLength/SCREEN_WIDTH;
          selectionEnd = selectionEnd*clipBoardLength/SCREEN_WIDTH;
          for(int i = selectionStart; i < selectionEnd; i++) clipBoard[i] = clipBoard[i] * (i-selectionStart) / (selectionEnd-selectionStart);
          pauseAudio = false;
          break;
        case FADE_OUT:
          pauseAudio = true;
          selectionStart = markerA > markerB ? markerB : markerA;
          selectionEnd = markerA > markerB ? markerA : markerB;
          selectionStart = selectionStart*clipBoardLength/SCREEN_WIDTH;
          selectionEnd = selectionEnd*clipBoardLength/SCREEN_WIDTH;
          for(int i = selectionStart; i < selectionEnd; i++) clipBoard[i] = clipBoard[i] * (selectionEnd-i) / (selectionEnd-selectionStart);
          pauseAudio = false;
          break;
        case TRIM:
          pauseAudio = true;
          s16* oldClipBoard = clipBoard;
          selectionStart = markerA > markerB ? markerB : markerA;
          selectionEnd = markerA > markerB ? markerA : markerB;
          selectionStart = selectionStart*clipBoardLength/SCREEN_WIDTH;
          selectionEnd = selectionEnd*clipBoardLength/SCREEN_WIDTH;
          clipBoard = (s16*)malloc((selectionEnd-selectionStart) * sizeof(s16));
          clipBoardLength = selectionEnd-selectionStart;
          for(int i = selectionStart; i < selectionEnd; i++) clipBoard[i-selectionStart] = oldClipBoard[i];
          free(oldClipBoard);
          markerA = 0;
          markerB = 255;
          pauseAudio = false;
          break;
      }
      command = NO_COMMAND;
    }
    void HandleButtons(u32 down, u32 up, u32 held, u32 inputFrames) {
      if(down&KEY_START && clipBoard!=NULL) {
        sampleIndex = 0;
      }
      if(down&KEY_LEFT && clipBoard!=NULL) {
        command = FADE_IN;
      }
      if(down&KEY_RIGHT && clipBoard!=NULL) {
        command = FADE_OUT;
      }
      if(down&KEY_DOWN && clipBoard!=NULL) {
        command = TRIM;
      }
    }
    void HandleTouchScreen(u32 down, u32 up, u32 held, touchPosition touch, u32 inputFrames) {
      if(down&KEY_TOUCH) markerA = touch.px;
      if(held&KEY_TOUCH) markerB = touch.px;
    }
    Command command;
    u8 markerA;
    u8 markerB;
};

class PolySeq : public BlackBox, public Module {
  public:
    int sampleRate;    

    int numTracks;
    u8** trackData;
    u8* trackIndices;
    u8* trackLengths;
    u8 maxTrackLength;

    Sampler** samplers;

    u32 sampleCounter = 0;

    PolySeq(int sr) {
      sampleRate = sr;
      offline = false;
      numTracks = 3;
      maxTrackLength = 16;
      command = NO_COMMAND;
      commandArg = 0;
      // --- //
      trackIndices = (u8*)malloc(numTracks*sizeof(u8));
      trackLengths = (u8*)malloc(numTracks*sizeof(u8));
      memset(trackLengths,0,numTracks*sizeof(u8));
      memset(trackLengths,8,numTracks*sizeof(u8));
      trackData = (u8**)malloc(numTracks*sizeof(u8*));
      for(int i=0; i<numTracks; i++) {
        trackData[i] = (u8*)malloc(maxTrackLength*sizeof(u8));
        for(int j=0; j<trackLengths[i]; j++) trackData[i][j] = 0;
      }
      // --- //
      samplers = (Sampler**)malloc(sizeof(Sampler*) * numTracks);
      for(int i=0; i<numTracks; i++) {
        s16* newSampleData = (s16*)malloc(sizeof(s16)*100);
        for(int i=0; i<100; i++) newSampleData[i] = rand();
        samplers[i] = new Sampler(sr,newSampleData,100);
      }
    }
    int Process() {
      int sample = 0;
      if(offline) return 0;
      while(sampleCounter >= MAXAMP>>3) {
        for(int i=0; i<numTracks; i++) {
          if(trackData[i][trackIndices[i]] > 0) samplers[i]->Play();
          trackIndices[i] = (trackIndices[i] + 1) % trackLengths[i];
        }
        sampleCounter -= MAXAMP>>3;
      }
      // --- //
      for(int i=0; i<numTracks; i++) {
        sample += samplers[i]->Process();
      }
      sampleCounter++;
      return sample;
    }
    void Draw(gLayer layer) {
      for(int track=0; track<numTracks; track++) {
        int radius = (track*10)+30;
        int points = trackLengths[track];
        GraphicsEng::Instance()->DrawCircle(
            SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0,
            radius,-40,
            (radius%20)==0 ? RGB15(31,15,31) : RGB15(15,31,31),
            ALPHA_ON,
            layer
            );
        for(int i=0; i<trackLengths[track]; i++) {
          int x = (sinLerp(i*DEGREES_IN_CIRCLE/points)*radius)>>12;
          int y = (cosLerp(i*DEGREES_IN_CIRCLE/points)*radius)>>12;
          int boxWidth = 6;
          GraphicsEng::Instance()->DrawRect(
              SCREEN_WIDTH/2+x-(boxWidth/2),
              SCREEN_HEIGHT/2+y-(boxWidth/2),
              boxWidth,boxWidth,
              boxWidth,
              trackData[track][i] > 0 ? RGB15(31,31,31) : RGB15(0,0,0),
              ALPHA_ON,layer
              );
          GraphicsEng::Instance()->DrawRect(
              SCREEN_WIDTH/2+x-(boxWidth/2),
              SCREEN_HEIGHT/2+y-(boxWidth/2),
              boxWidth,boxWidth,
              0,
              RGB15(31,31,31),ALPHA_ON,layer
              );
        }
      }
      if(offline) DrawOffline(layer);
    }
    enum Command {
      NO_COMMAND,
      PASTE
    };
    Command command;
    int commandArg;
    void AsyncProcess() {
      switch(command) {
        case PASTE:
          pauseAudio = true;
          samplers[commandArg]->CopySample(clipBoard, clipBoardLength);
          pauseAudio = false;
        break;
      }
      command = NO_COMMAND;
    }
    void HandleButtons(u32 down, u32 up, u32 held, u32 inputFrames) {
      if(down&KEY_LEFT) { commandArg = 0; command = PASTE; }
      if(down&KEY_DOWN) { commandArg = 1; command = PASTE; }
      if(down&KEY_RIGHT) { commandArg = 2; command = PASTE; }
    }
    void HandleTouchScreen(u32 down, u32 up, u32 held, touchPosition touch, u32 inputFrames) {
      if(down&KEY_TOUCH) {
        for(int track=0; track<numTracks; track++) {
          int radius = (track*10)+30;
          int points = trackLengths[track];
          for(int i=0; i<trackLengths[track]; i++) {
            int x = (sinLerp(i*DEGREES_IN_CIRCLE/points)*radius)>>12;
            int y = (cosLerp(i*DEGREES_IN_CIRCLE/points)*radius)>>12;
            int boxWidth = 6;
            if(TestRect(SCREEN_WIDTH/2+x-(boxWidth/2),
                  SCREEN_HEIGHT/2+y-(boxWidth/2),
                  boxWidth,boxWidth,
                  touch)) {
              trackData[track][i] = trackData[track][i] > 0 ? 0 : 1;
            }
          }
        }
      }
    }
};

class SimpleSynth : public BlackBox, public Module {
  public:
    SinOsc* modulator;
    SinOsc* carrier;
    int envelope;
    int freq;
    int note;
    u8 keyColors[12] = {1,0,1,0,1,1,0,1,0,1,0,1};
    SimpleSynth(int sr) {
      carrier = new SinOsc(sr);
      carrier->SetAmplitude(0);
      carrier->SetFreqHZ(440);
      modulator = new SinOsc(sr);
      modulator->SetAmplitude(0);
      modulator->SetFreqHZ(440);
      freq = 440;
      envelope = 0;
      note = 0;
      offline = false;
    }
    int Process() {
      int sample = 0;
      if(offline) return 0;
      if(envelope > 0) {
        envelope = (envelope * (MAXAMP-1)) >> 15;
        modulator->SetAmplitude(envelope >> 7);
        carrier->SetFreqHZ(modulator->Process() + freq);
        carrier->SetAmplitude(envelope >> 1);
        sample = carrier->Process();
      }
      return sample;
    }
    void Draw(gLayer layer) {
      int margin = 4;
      for(int i=0; i<12; i++) {
        for(int j=0; j<7; j++) {
          GraphicsEng::Instance()->DrawRect(
              (SCREEN_WIDTH*i)/12+(margin/2),(SCREEN_HEIGHT*j)/7+(margin/2),
              SCREEN_WIDTH/12-margin,SCREEN_HEIGHT/7-margin,
              keyColors[i%12] > 0 ? SCREEN_WIDTH/7 : 1,
              RGB15(7,7,7),ALPHA_ON,layer
              );
          if((i+(j*12)) == note && envelope > 0) {
            int illumination = (envelope*31)/MAXAMP;
            GraphicsEng::Instance()->DrawRect(
                (SCREEN_WIDTH*i)/12+(margin/2),(SCREEN_HEIGHT*j)/7+(margin/2),
                SCREEN_WIDTH/12-margin,SCREEN_HEIGHT/7-margin,
                SCREEN_WIDTH/7,
                RGB15(illumination,illumination*2/3,illumination*2/3),ALPHA_ON,layer
                );
          }
          GraphicsEng::Instance()->DrawRect(
              (SCREEN_WIDTH*i)/12+(margin/2),(SCREEN_HEIGHT*j)/7+(margin/2),
              SCREEN_WIDTH/12-margin,SCREEN_HEIGHT/7-margin,
              0,RGB15(31,31,31),ALPHA_ON,layer
              );
        }
      }
      if(offline) DrawOffline(layer);
    }
    void AsyncProcess() {}
    void HandleButtons(u32 down, u32 up, u32 held, u32 inputFrames) {
    }
    void HandleTouchScreen(u32 down, u32 up, u32 held, touchPosition touch, u32 inputFrames) {
      if(down&KEY_TOUCH) {
        int pitch = (touch.px*12)/SCREEN_WIDTH, octave = (touch.py*7)/SCREEN_HEIGHT;
        note = pitch + (octave*12);
        freq = pow(2.0,((float)pitch)/12.0) * (55.0/4.0) * pow(2,octave);
        modulator->SetFreqHZ(freq);
        envelope = MAXAMP;
      }
    }
};

class Reel2Reel : public BlackBox, public Module {
  public:
    Reel2Reel(int sr) {
      sampleRate = sr;
      recBufIndex = 0;
      recBufSize = sr * 5;
      recBuf = (s16*)malloc(recBufSize * sizeof(s16));
      playHeadSpeed = 0;
      playHeadDelta = 0;
      playing = false;
      recording = false;
      lastTouchX = 666;
      tapeColor = RGB15(24,24,24);
      offline = false;
      markerA = 0;
      markerB = recBufSize;
      newIndex = 0;
      clock = 0;
      delta = 0;
      desiredDelta = 0;
      command = NO_COMMAND;
    }
    int newIndex;
    int clock;
    int delta;
    int desiredDelta;
    int Process(int input) {
      if(offline) return 0;
      int sample = 0;
      sample = recBuf[newIndex];
      if(recording) recBuf[newIndex] = input;
      // --- //
      delta += (desiredDelta - delta) >> 13;
      if(recording && (abs(delta)>MAXCLOCK)) delta = delta > 0 ? MAXCLOCK : -MAXCLOCK;
      // --- ///
      clock += abs(delta);
      while(clock > MAXCLOCK) {
        clock -= MAXCLOCK;
        if(delta > 0) newIndex++;
        if(delta < 0) newIndex--;
      }
      while(newIndex >= recBufSize) newIndex -= recBufSize;
      while(newIndex < 0) newIndex += recBufSize;
      // --- //
      recBufIndex = newIndex; 
      // --- //
      return recording ? 0 : sample; // maybe should not auto mute when recording? tape delay?
    }
    int Process() {
      return Process(0);
    }
    void Draw(gLayer layer) {
      int playHeadPos = (int)recBufIndex;
      DrawTimeline(playHeadPos, markerA, markerB, recBufSize, SCREEN_HEIGHT*3/4, 20, 16, tapeColor, layer);
      DrawReel2Reel(recBufIndex/300, layer);
      // --- clear graphics artifacts --- //
      for(int i = 0 ; i < SCREEN_WIDTH; i++) GraphicsEng::Instance()->DrawPoint(i,0,0,ALPHA_ON,layer);
      // --- draw status text --- //
      GraphicsEng::Instance()->DrawLine(0,0,SCREEN_WIDTH,0,RGB15(31,31,31),ALPHA_ON,layer);
      GraphicsEng::Instance()->DrawLine(0,9,SCREEN_WIDTH,9,RGB15(31,31,31),ALPHA_ON,layer);
      GraphicsEng::Instance()->DrawString(1,2,"Playing",playing ? RGB15(23,31,23) : RGB15(7,7,7),layer);
      GraphicsEng::Instance()->DrawString(50 ,2,"Recording",recording ? RGB15(31,23,23) : RGB15(7,7,7),layer);
      GraphicsEng::Instance()->DrawInt(5,SCREEN_HEIGHT-10,recBufIndex*100/recBufSize,RGB15(31,31,31),layer);
      if(offline) DrawOffline(layer);
    }
    enum Command {
      NO_COMMAND,
      TRIM,
      COPY,
      ADD_TAPE
    };
    Command command;
    void AsyncProcess() {
      switch(command) {
        case TRIM:
          {
            u32 temp;
            pauseAudio = true;
            if(markerA > markerB) { temp = markerA; markerA = markerB; markerB = temp; }
            s16* tempBuf = (s16*)malloc((markerB-markerA) * sizeof(s16));
            s16* freeBuf = recBuf;
            for(int i = markerA; i <= markerB; i++) tempBuf[i-markerA] = recBuf[i];
            recBufSize = markerB-markerA;
            recBuf = tempBuf;
            markerA = 0;
            markerB = recBufSize-1;
            free(freeBuf);
            pauseAudio = false;
          }
          break;
        case COPY:
          {
            u32 temp;
            pauseAudio = true;
            if(markerA > markerB) { temp = markerA; markerA = markerB; markerB = temp; }
            if(clipBoard != NULL) free(clipBoard);
            clipBoard = (s16*)malloc((markerB-markerA) * sizeof(s16));
            clipBoardLength = markerB-markerA;
            for(int i = markerA; i < markerB; i++) clipBoard[i-markerA] = recBuf[i];
            pauseAudio = false;
          }
          break;
        case ADD_TAPE:
          {
            pauseAudio = true;
            s16* temp = (s16*)realloc(recBuf, (recBufSize + sampleRate) * sizeof(s16));
            if(temp != NULL) {
              recBuf = temp;
              recBufSize += sampleRate;
            }
            pauseAudio = false;
          }
          break;
      }
      command = NO_COMMAND;
    }
    void HandleButtons(u32 down, u32 up, u32 held, u32 inputFrames) {
      if(down&KEY_START) playing = !playing;
      if(down&KEY_A) recording = !recording;
      if(down&KEY_LEFT) markerA = (u32)recBufIndex;
      if(down&KEY_RIGHT) markerB = (u32)recBufIndex;
      if(down&KEY_UP) command = TRIM;
      if(down&KEY_DOWN) command = COPY;
      if(down&KEY_Y) command = ADD_TAPE;
    }
    void HandleTouchScreen(u32 down, u32 up, u32 held, touchPosition touch, u32 inputFrames) {
      if(held&KEY_TOUCH) {
        if((inputFrames & 0x000F) == 0) {
          if(lastTouchX!=666) {
            playHeadSpeed = (touch.px - lastTouchX) * -0.5;
            // --- //
            desiredDelta = ((touch.px - lastTouchX) * ((-MAXCLOCK)>>1));
          }
          lastTouchX=touch.px;
        }
      } else {
        lastTouchX=666;
        playHeadSpeed = playing ? 1 : 0;
        // --- //
        desiredDelta = playing ? MAXCLOCK : 0;
      }
    }
    float recBufIndex;
    int recBufSize;
    s16* recBuf;
    float playHeadSpeed;
    bool playing;
    bool recording;
    int lastTouchX;
    u16 tapeColor;
    u32 markerA;
    u32 markerB;
    int sampleRate;
  private:
    float playHeadDelta;
};

// --------------------------- //
// -- MODULE VIEW FUNCTIONS -- //
// --------------------------- //

inline bool TestRect(int x, int y, int width, int height, touchPosition touch) {
  if(touch.px >= x && touch.px <= (x+width) && touch.py >= y && touch.py <= (y+height)) return true;
  return false;
}

inline void DrawOffline(gLayer layer) {
  for(int i = 0; i < SCREEN_HEIGHT; i++)
    for(int j = 0; j < SCREEN_WIDTH; j += 2)
      GraphicsEng::Instance()->DrawPoint(
          j+(i&1),i,RGB15(0,0,0),ALPHA_ON,layer
          );
}

inline void DrawReel(int x, int y, int angle, int radius, gLayer layer) {
  int circleWidth = radius;
  int numSpokes = 3;
  int r1 = 150;
  int r2 = 275;
  GraphicsEng::Instance()->DrawCircle(
      x,y,angle,
      circleWidth,-40,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
  GraphicsEng::Instance()->DrawCircle(
      x,y,angle,
      circleWidth/4,-40,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
  GraphicsEng::Instance()->DrawCircle(
      x,y,angle,
      circleWidth/6,-40,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
  for(int i = 0; i < numSpokes; i++) {
    int x1=sinLerp(angle+(DEGREES_IN_CIRCLE*i/numSpokes))*r1/DEGREES_IN_CIRCLE+x;
    int y1=cosLerp(angle+(DEGREES_IN_CIRCLE*i/numSpokes))*r1/DEGREES_IN_CIRCLE+y;
    int x2=sinLerp(angle+(DEGREES_IN_CIRCLE*i/numSpokes))*r2/DEGREES_IN_CIRCLE+x;
    int y2=cosLerp(angle+(DEGREES_IN_CIRCLE*i/numSpokes))*r2/DEGREES_IN_CIRCLE+y;
    GraphicsEng::Instance()->DrawLine(x1,y1,x2,y2,RGB15(31,31,31),ALPHA_ON,layer);
  }
  GraphicsEng::Instance()->DrawLine(
      SCREEN_WIDTH/4-30,SCREEN_WIDTH/4+30,
      SCREEN_WIDTH/4-4,SCREEN_WIDTH/2+4,
      RGB15(31,31,31), ALPHA_ON, layer
      );
  GraphicsEng::Instance()->DrawLine(
      SCREEN_WIDTH*3/4+28,SCREEN_WIDTH/4+30,
      SCREEN_WIDTH*3/4+3,SCREEN_WIDTH/2+3,
      RGB15(31,31,31), ALPHA_ON, layer
      );
  GraphicsEng::Instance()->DrawLine(
      SCREEN_WIDTH*3/4-3,SCREEN_WIDTH/2+4,
      SCREEN_WIDTH*5/8+5,SCREEN_WIDTH/2-9,
      RGB15(31,31,31), ALPHA_ON, layer
      );
  GraphicsEng::Instance()->DrawLine(
      SCREEN_WIDTH*1/4+3,SCREEN_WIDTH/2+4,
      SCREEN_WIDTH*3/8-5,SCREEN_WIDTH/2-9,
      RGB15(31,31,31), ALPHA_ON, layer
      );
  GraphicsEng::Instance()->DrawLine(
      SCREEN_WIDTH*3/8+5,SCREEN_WIDTH/2-9,
      SCREEN_WIDTH*1/2-5,SCREEN_WIDTH/2+4,
      RGB15(31,31,31), ALPHA_ON, layer
      );
  GraphicsEng::Instance()->DrawLine(
      SCREEN_WIDTH*5/8-6,SCREEN_WIDTH/2-9,
      SCREEN_WIDTH*1/2+5,SCREEN_WIDTH/2+4,
      RGB15(31,31,31), ALPHA_ON, layer
      );
}
inline void DrawReel2Reel(int position, gLayer layer) {
  DrawReel(SCREEN_WIDTH/4, SCREEN_WIDTH/4, (position*DEGREES_IN_CIRCLE)/360, SCREEN_WIDTH/6, layer);
  DrawReel(SCREEN_WIDTH*3/4, SCREEN_WIDTH/4, (position*DEGREES_IN_CIRCLE)/360, SCREEN_WIDTH/6, layer);
  GraphicsEng::Instance()->DrawCircle(
      SCREEN_WIDTH/4,SCREEN_WIDTH/2,(position*DEGREES_IN_CIRCLE)/360,
      5,-40,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
  GraphicsEng::Instance()->DrawCircle(
      SCREEN_WIDTH*3/4,SCREEN_WIDTH/2,(position*DEGREES_IN_CIRCLE)/360,
      5,-40,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
  GraphicsEng::Instance()->DrawCircle(
      SCREEN_WIDTH*3/8,SCREEN_WIDTH/2-3,(position*DEGREES_IN_CIRCLE)/360,
      8,-40,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
  GraphicsEng::Instance()->DrawCircle(
      SCREEN_WIDTH*3/8,SCREEN_WIDTH/2-3,(position*DEGREES_IN_CIRCLE)/360,
      5,-40,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
  GraphicsEng::Instance()->DrawCircle(
      SCREEN_WIDTH*5/8,SCREEN_WIDTH/2-3,(position*DEGREES_IN_CIRCLE)/360,
      8,-40,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
  GraphicsEng::Instance()->DrawCircle(
      SCREEN_WIDTH*5/8,SCREEN_WIDTH/2-3,(position*DEGREES_IN_CIRCLE)/360,
      5,-40,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
  GraphicsEng::Instance()->DrawRect(
      SCREEN_WIDTH/2-6,
      SCREEN_HEIGHT*2/3-4,
      12,8,
      1,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
  GraphicsEng::Instance()->DrawRect(
      SCREEN_WIDTH/2-2,
      SCREEN_HEIGHT*2/3,
      4,4,
      1,
      RGB15(31,31,31),
      ALPHA_ON,
      layer
      );
}
inline void DrawTimeline(u32 sampleIndex, u32 markerA, u32 markerB, u32 bufferSize, int offset, int height, int markerSpacing, u16 tapeColor, gLayer layer) {
  int markerOffset = (((sampleIndex%(1000*markerSpacing))*markerSpacing)/(1000*markerSpacing));
  markerOffset = markerSpacing - markerOffset;
  while(markerOffset < 255) {
    GraphicsEng::Instance()->DrawLine(
        markerOffset,offset-(height/3),
        markerOffset,offset+(height/3),
        RGB15(15,15,15),ALPHA_ON,layer
        );
    markerOffset += markerSpacing;
  }
  GraphicsEng::Instance()->DrawLine(
      SCREEN_WIDTH/2,offset-(height/2),
      SCREEN_WIDTH/2,offset+(height/2),
      RGB15(31,31,31),ALPHA_ON,layer
      );
  GraphicsEng::Instance()->DrawLine(
      SCREEN_WIDTH/2-(sampleIndex/1000),offset,
      SCREEN_WIDTH/2+(bufferSize/1000)-(sampleIndex/1000),offset,
      tapeColor,ALPHA_ON,layer
      );
  u32 temp;
  if(markerA > markerB) { temp = markerA; markerA = markerB; markerB = temp; }
  GraphicsEng::Instance()->DrawChar(
      SCREEN_WIDTH/2-(sampleIndex/1000)+(markerA/1000)-2,
      offset-3,
      'o',
      RGB15(31,31,31),layer
      );
  GraphicsEng::Instance()->DrawChar(
      SCREEN_WIDTH/2-(sampleIndex/1000)+(markerB/1000)-2,
      offset-3,
      'o',
      RGB15(31,31,31),layer
      );
}
#endif
