#ifndef GENERAL_H
#define GENERAL_H
#include <nds.h>
#include <stdio.h>
#include <fat.h>
#include <maxmod9.h>
#include "blackbox.h"

typedef struct {
  long         RIFF;      //RIFF header
  char         NI1 [18];  //not important
  unsigned int Channels;  //channels 1 = mono; 2 = stereo
  long         Frequency; //sample frequency
  char         NI2 [6];   //not important
  char         BitRes;    //bit resolution 8/16 bit
  char         NI3 [8];  //not important
  u32          datasize;
  union {
	u16* data16;
	u8*  data8;  
  } data;
} WAVheader;

WAVheader* loadWAV( char* filename );
void SoundSetup( BlackBox* sndeng, int srate, int bufsize );
void SoundRestart(int srate, int bufsize);
int ipower( int var, int power );
bool recttest( int x, int y, int rx, int ry, int rwidth, int rheight );

#endif
