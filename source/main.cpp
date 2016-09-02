/*---------------------------------------------------------------------------------
	Basic template code for starting a DS app
---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include "general.h"
#include "blackbox.h"
#include "graphicseng.h"
#include "soundengine.h"
#define SAMPLERATE	32768
#define CIRCLEDIVS 360
#define BUFSIZE 512

s16* clipBoard = NULL;
int clipBoardLength = 0;

bool pauseAudio = false;

int numModules = 5;
Module** modules;

int topModuleIndex = 0;
int bottomModuleIndex = 0;
bool swapReels = false;
u32 inputFrames = 0;

bool drawTopModule = false;
bool drawBottomModule = false;

void HandleInput() {
		touchPosition touch; touchRead( &touch );
		scanKeys();
		u32 down=keysDown(), up=keysUp(), held=keysHeld();
    
    if(held&KEY_SELECT) {
      if(down&KEY_START)
        modules[bottomModuleIndex]->offline = !modules[bottomModuleIndex]->offline;
      if(down&KEY_R) { topModuleIndex++; drawTopModule = true; }
      if(down&KEY_L) { topModuleIndex--; drawTopModule = true; }
      while(topModuleIndex<0) topModuleIndex += numModules;
      topModuleIndex %= numModules;
    } else {
      if(down&KEY_R) bottomModuleIndex++; { drawBottomModule = true; }
      if(down&KEY_L) bottomModuleIndex--; { drawBottomModule = true; }
      while(bottomModuleIndex<0) bottomModuleIndex += numModules;
      bottomModuleIndex %= numModules;
    }

    if(!(held&KEY_SELECT)) {
      modules[bottomModuleIndex]->HandleButtons(down, up, held, inputFrames);
      modules[bottomModuleIndex]->HandleTouchScreen(down, up, held, touch, inputFrames);
    }

    //if((held&KEY_L) && (held&KEY_R) && (down&KEY_START)) SoundRestart(SAMPLERATE, BUFSIZE);

    modules[topModuleIndex]->AsyncProcess();
    modules[bottomModuleIndex]->AsyncProcess();
    
    inputFrames++;
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	SoundEngine* soundengine = new SoundEngine( SAMPLERATE );
	SoundSetup( soundengine, SAMPLERATE, BUFSIZE );

  modules = (Module**)malloc(numModules * sizeof(Module*));
  modules[0] = soundengine->r2rA;
  modules[1] = soundengine->r2rB;
  modules[2] = soundengine->simpleSynth;
  modules[3] = soundengine->polySeq;
  modules[4] = soundengine->sampleEditor;

	while(1) {

   	// -------------------- //
		// -- INPUT HANDLING -- //
   	// -------------------- //

    timerStart(1,ClockDivider_1024,TIMER_FREQ_1024(100),HandleInput);

		// -------------- //
		// -- GRAPHICS -- //
		// -------------- //

    if(!modules[bottomModuleIndex]->offline || drawBottomModule) {
      modules[bottomModuleIndex]->Draw(SUB_LAYER);
      drawBottomModule = false;
    }
    if(!modules[topModuleIndex]->offline || drawTopModule) {
      modules[topModuleIndex]->Draw(MAIN_LAYER);
      drawTopModule = false;
    }
  
    GraphicsEng::Instance()->Update();
	}
}


