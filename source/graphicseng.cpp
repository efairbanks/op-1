#include "graphicseng.h"

static u16 defaultCharSet5x5[1700] = {0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,0,0,1,1,0,0,1,0,0,1,0,0,1,1,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,1,1,0,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,0,1,1,1,0,1,0,0,0,1,0,0,1,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,1,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,1,0,0,0,0,1,1,1,1,0,1,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,1,0,0,0,1,0,1,1,1,0,0,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,0,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,0,1,0,0,0,0,1,1,1,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,1,0,1,0,0,0,0,1,0,1,1,1,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,0,0,0,1,0,1,0,0,1,0,0,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,0,1,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,1,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,1,0,0,1,0,0,1,1,0,1,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,1,0,1,0,1,1,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0};

GraphicsEng* gui = NULL;

GraphicsEng::GraphicsEng() {
	videoSetMode(		MODE_5_2D );
	videoSetModeSub(	MODE_5_2D );
	vramSetBankA( VRAM_A_MAIN_BG );
	vramSetBankB( VRAM_B_MAIN_BG_0x06020000 );
	vramSetBankE( VRAM_E_MAIN_BG );
	vramSetBankF( VRAM_F_MAIN_SPRITE );
	vramSetBankC( VRAM_C_SUB_BG );
	vramSetBankD( VRAM_D_SUB_SPRITE );
	MainBGid = bgInit( 2, BgType_Bmp16, BgSize_B16_256x256, 0, 0 );
	memset( bgGetGfxPtr( MainBGid ), NULL, 256*256*4 );
	MainBGbackbuffer = bgGetGfxPtr( MainBGid );
	memset( &MainBGimage, NULL, sizeof( sImage ) );
	bgSetMapBase( MainBGid, 6 );
	MainBGbuffer = bgGetGfxPtr( MainBGid );
	MainBGupdate = false;
	SubBGid = bgInitSub( 2, BgType_Bmp16, BgSize_B16_256x256, 0, 0 );
	memset( bgGetGfxPtr( SubBGid ), NULL, 192*256*2 );
	SubBGbuffer = bgGetGfxPtr( SubBGid );
	memset( &SubBGimage, NULL, sizeof( sImage ) );
	SubBGbackbuffer = (u16*)malloc( 192*256*2 );
	memset( SubBGbackbuffer, NULL, 192*256*2 );
	SubBGupdate = false;
	oamInit( &oamMain, SpriteMapping_1D_32, false );
	oamInit(  &oamSub, SpriteMapping_1D_32, false );
	memset( MainSPRbuffer, NULL, 128 );
	memset( SubSPRbuffer, NULL, 128 );
}
GraphicsEng* GraphicsEng::Instance(){
	if( gui == NULL ) gui = new GraphicsEng();
	return gui;
}
void GraphicsEng::Reset(){
	memset( this->MainBGbuffer,		 0, 256*192*2 );
	memset( this->MainBGbackbuffer,	 0, 256*192*2 );
	memset( this->SubBGbuffer,		 0, 256*192*2 );
	memset( this->SubBGbackbuffer,	 0, 256*192*2 );
}
void GraphicsEng::SetBG( u16 color, pAlpha alpha, gLayer layer){
	u16* vbuf = NULL;
	if( layer == MAIN_LAYER ){ vbuf = MainBGbackbuffer; MainBGupdate = true; }
	if( layer ==  SUB_LAYER ){ vbuf = SubBGbackbuffer; SubBGupdate  = true; }
	for(int i=0; i<SCREEN_WIDTH*SCREEN_HEIGHT; i++) vbuf[i]=color|(alpha<<15);
}
void GraphicsEng::SetBG( u8* bgimage, int priority, u8 alphaindex, gLayer layer ){
	if( layer == MAIN_LAYER ){
		loadPCX( bgimage, &MainBGimage );
		MainBGimageid = bgInit( 3, BgType_Bmp8, BgSize_B8_256x256, 12,0 );
		bgSetPriority( MainBGimageid, priority );
		dmaCopy( MainBGimage.image.data16, bgGetGfxPtr(MainBGimageid), 256*256 );
		dmaCopy( MainBGimage.palette, BG_PALETTE, 256*2 );
		imageDestroy( &MainBGimage );
		memset( MainBGbackbuffer, 0, 192*256*2 );
	}
	if( layer == SUB_LAYER ){
		if( SubBGimage.image.data16 != NULL ){
			imageDestroy( &SubBGimage );
			memset( &SubBGimage, 0, sizeof( sImage ) );
		}
		loadPCX( bgimage, &SubBGimage );
		if( alphaindex == 0 ){
			image8to16( &SubBGimage );
		} else image8to16trans( &SubBGimage, alphaindex );
	}
}
void GraphicsEng::Test() {
	for( int i = 0; i < SCREEN_WIDTH; i++ ){
		int random = rand() & 0x1F;
		for( int j = 0; j < SCREEN_HEIGHT; j++ ){
			MainBGbackbuffer[i+(j*SCREEN_WIDTH)] = RGB15( random, random, random) | ( BIT(15) * (rand() & 1) );
			MainBGbackbuffer[i+(j*SCREEN_WIDTH)] = RGB15( 	32*j/SCREEN_HEIGHT, 32*j/SCREEN_HEIGHT,\
														32*j/SCREEN_HEIGHT ) | BIT(15);
			MainBGupdate = true;
			SubBGbackbuffer[i+(j*SCREEN_WIDTH)] = RGB15( 	32*j/SCREEN_HEIGHT, 32*j/SCREEN_HEIGHT,\
														32*j/SCREEN_HEIGHT ) | BIT(15);
			SubBGupdate = true;
		}
	}
}
void GraphicsEng::DrawPoint( int x, int y, u16 color, pAlpha alpha, gLayer layer ) {
	if( (x >= 0 && x < 256) && (y >= 0 && y < 192) ){
		u16* vbuf = NULL;
		if( layer == MAIN_LAYER ){ vbuf = MainBGbackbuffer; 	MainBGupdate = true; }
		if( layer ==  SUB_LAYER ){ vbuf = SubBGbackbuffer; SubBGupdate  = true; }
		if( vbuf != NULL ) vbuf[x + (y*256)] = color | (alpha << 15);
	}
}
void GraphicsEng::DrawLine( int x1, int y1, int x2, int y2, u16 color, pAlpha alpha, gLayer layer ) {
	int diff, swap;
	if( abs(y2 - y1) <= abs(x2 - x1) ){
		if( x1 > x2 ){
			swap = x1;
			x1 = x2;
			x2 = swap;
			swap = y1;
			y1 = y2;
			y2 = swap;
		}
		diff = x2 - x1;
		for( int i = 0; i <= diff; i++ ) DrawPoint( i + x1, ((y1*(diff-i))/diff) + ((y2*i)/diff), color, alpha, layer );
	} else{
		if( y1 > y2 ){
			swap = y1;
			y1 = y2;
			y2 = swap;
			swap = x1;
			x1 = x2;
			x2 = swap;
		}
		diff = y2 - y1;
		for( int i = 0; i <= diff; i++ ) DrawPoint( ((x1*(diff-i)) + (x2*i))/diff, i + y1, color, alpha, layer );
	}
}
void GraphicsEng::DrawBezQuad( int x0, int x1, int x2, int y0, int y1, int y2, int segments, u16 color, pAlpha alpha, gLayer layer ){
	int x = x0;
	int y = y0;
	int lastx = x0;
	int lasty = y0;
	for( int i = 0; i <= segments; i++ ){
		lastx = x;
		lasty = y;
		x = ((((segments-i)*(segments-i))*x0)/(segments*segments)) + 2*((((segments-i)*(i))*x1)/(segments*segments)) + ((((i)*(i))*x2)/(segments*segments));
		y = ((((segments-i)*(segments-i))*y0)/(segments*segments)) + 2*((((segments-i)*(i))*y1)/(segments*segments)) + ((((i)*(i))*y2)/(segments*segments));
		DrawLine( lastx, lasty, x, y, color, alpha, layer );
	}
}
void GraphicsEng::DrawRect( int x, int y, int width, int height, int borderwidth, u16 color, pAlpha alpha, gLayer layer ) {
	if( borderwidth > 1 ){
		for( int i = 0; i <= width; i++ ) for( int j = 0; j <= height; j++ )
			if( (width>>1) - borderwidth < abs( i - (width>>1) ) || (height>>1) - borderwidth < abs( j - (height>>1) ) )
				DrawPoint( i + x, j + y, color, alpha, layer );
	} else {
		for( int i = 0; i < width; i++ ){
			DrawPoint( x + i, y, color, alpha, layer );
			DrawPoint( x + i, y + height, color, alpha, layer );
		}
		for( int i = 0; i < height; i++ ){
			DrawPoint( x, y + i, color, alpha, layer );
			DrawPoint( x + width, y + i, color, alpha, layer );
		}
	}
}
void GraphicsEng::DrawCircle( int x, int y, int radius, int borderwidth, u16 color, pAlpha alpha, gLayer layer ) {
	if( borderwidth > 0 ){
		borderwidth++;
		for( int i = 0; i <= radius<<1; i++ ) for( int j = 0; j <= radius<<1; j++ ){
			int dist = sqrt((abs(j-radius)*abs(j-radius)) + (abs(i-radius)*abs(i-radius)));
			if( dist < radius && radius - borderwidth < dist )
				DrawPoint( i + x, j + y, color, alpha, layer );
		}
	} else{
		int newx = 0;
		int newy = 0;
		int oldx = 0;
		int oldy = radius;
		int divs = abs( borderwidth ) - 1;
		for( int i = 0; i < divs; i++ ){
			newy = (cosLerp( 32768*i/divs )*radius)>>12;
			newx = (sinLerp( 32768*i/divs )*radius)>>12;
			DrawLine( oldx + x, oldy + y, newx + x, newy + y, color, alpha, layer );
			oldx = newx;
			oldy = newy;
		}
		DrawLine( oldx + x, oldy + y, x, radius + y, color, alpha, layer );
	}
}
void GraphicsEng::DrawCircle( int x, int y, int angle, int radius, int borderwidth, u16 color, pAlpha alpha, gLayer layer ) {
	if( borderwidth > 0 ){
		borderwidth++;
		for( int i = 0; i <= radius<<1; i++ ) for( int j = 0; j <= radius<<1; j++ ){
			int dist = sqrt((abs(j-radius)*abs(j-radius)) + (abs(i-radius)*abs(i-radius)));
			if( dist < radius && radius - borderwidth < dist )
				DrawPoint( i + x - radius, j + y - radius, color, alpha, layer );
		}
	} else{
		int newx = 0;
		int newy = 0;
		int oldx = (sinLerp(angle)*radius)>>12;
		int oldy = (cosLerp(angle)*radius)>>12;
		int divs = abs( borderwidth ) - 1;
		for( int i = 0; i < divs + 1; i++ ){
			newy = (cosLerp( 32768*i/divs + angle)*radius)>>12;
			newx = (sinLerp( 32768*i/divs + angle)*radius)>>12;
			DrawLine( oldx + x, oldy + y, newx + x, newy + y, color, alpha, layer );
			oldx = newx;
			oldy = newy;
		}
	}
}

void GraphicsEng::DrawChar(int x, int y, char c, u16 color, gLayer layer) {
  int offset = -1;
  if(c >= 33 && c <= 96) {
    offset = (c-33)*25;
  }
  if(c >= 97 && c <= 122) {
    offset = ((c-65)*25);
  }
  if(c >= 123 && c <= 126) {
    offset = ((c-122)+63)*25;
  }
  if(offset > -1) {
    for(int i = 0; i < 5; i++) {
      for(int j = 0; j < 5; j++) {
        if(defaultCharSet5x5[offset+i+(j*5)] > 0) {
          GraphicsEng::Instance()->DrawPoint(
            i+x,j+y,
            color,
            ALPHA_ON,layer
          );
        }
      }
    }
  }
}

void GraphicsEng::DrawInt(int x, int y, int i, u16 color, gLayer layer) {
  int digits[10];
  int n = 0;
  while(i>0) {
    digits[n] = i%10;
    i /= 10;
    n++;
  }
  i = 0;
  while(n > 0) {
    n--;
    DrawChar(x+(i*6), y, 48+digits[n], color, layer);
    i++;
  }
}

void GraphicsEng::DrawString(int x, int y, char* s, u16 color, gLayer layer) {
  for(int i = 0; i < strlen(s); i++)
    DrawChar(x+(i*6), y, s[i], color, layer);
}

sGraphicsIndex GraphicsEng::AddSpriteGraphics( u8* spriteimage, int graphicsoffset, SpriteSize size, gLayer layer ){
	sImage image;
	SpriteGFX* gfx = NULL;
	OamState* engine = NULL;
	u16* palette = NULL;
	sGraphicsIndex index = 0;
	if( layer == MAIN_LAYER ){ gfx = MainSPRbuffer; engine = &oamMain; palette = SPRITE_PALETTE; }
	if( layer == SUB_LAYER  ){ gfx = SubSPRbuffer; engine = &oamSub; palette = SPRITE_PALETTE_SUB; }
	while( gfx[index] != NULL ) index++;
	gfx[index] = oamAllocateGfx( engine, size, SpriteColorFormat_256Color );
	loadPCX((u8*)spriteimage, &image);
	imageTileData(&image);
	for( int i = 0; i < 256; i++ ) palette[i] = image.palette[i];
	for( int i = 0; i < SPRITE_SIZE_PIXELS(size)/2; i++ ) gfx[index][i] = image.image.data16[i + (graphicsoffset>>1)];
	imageDestroy( &image );
	return index;
}

bool GraphicsEng::DrawSprite( int x, int y, bool hide, int priority, sGraphicsIndex index, int oamindex, SpriteSize sprsize, gLayer layer ){
	if( layer == MAIN_LAYER ){
		oamSet(
			&oamMain, 							//main graphics engine context
			oamindex,          					//oam index (0 to 127)  
			x, 191-y,  							//x and y pixel location of the sprite
			priority & 0x03,					//priority, lower renders last (on top)
			0,					  				//this is the palette index if multiple palettes or the alpha value if bmp sprite	
			sprsize,     
			SpriteColorFormat_256Color, 
			MainSPRbuffer[index & 0x3F],		//pointer to the loaded graphics
			0,                  				//sprite rotation/scale matrix index 
			false,              				//double the size when rotating?
			hide,								//hide the sprite?
			false, false, 						//vflip, hflip
			false								//apply mosaic
		);
		MainBGupdate = true;
	}
	if( layer == SUB_LAYER ){
		oamSet(
			&oamSub, 							//main graphics engine context
			oamindex,          					//oam index (0 to 127)  
			x, 191-y,  							//x and y pixel location of the sprite
			priority & 0x03,					//priority, lower renders last (on top)
			0,					  				//this is the palette index if multiple palettes or the alpha value if bmp sprite	
			sprsize,     
			SpriteColorFormat_256Color, 
			SubSPRbuffer[index & 0x3F],			//pointer to the loaded graphics
			0,                  				//sprite rotation/scale matrix index 
			false,              				//double the size when rotating?
			hide,								//hide the sprite?
			false, false, 						//vflip, hflip
			false								//apply mosaic
		);
		SubBGupdate = true;
	}
	return 0;
}
bool GraphicsEng::UpdateMain(){
	if( MainBGupdate == false ) return 0;
	MainBGupdate = false;
	oamUpdate( &oamMain );
	if( bgGetMapBase( MainBGid ) == 6 ){
		MainBGbackbuffer = bgGetGfxPtr( MainBGid );
		bgSetMapBase( MainBGid, 0 );
		MainBGbuffer = bgGetGfxPtr( MainBGid );
	} else if ( bgGetMapBase( MainBGid ) == 0 ){
		MainBGbackbuffer = bgGetGfxPtr( MainBGid );
		bgSetMapBase( MainBGid, 6 );
		MainBGbuffer = bgGetGfxPtr( MainBGid );
	} else return 1;
	memset( MainBGbackbuffer, 0, 192*256*2 );
	return 0;
}
bool GraphicsEng::UpdateSub(){
	if( SubBGupdate == false ) return 0;
	SubBGupdate = false;
	oamUpdate( &oamSub );
	memcpy( SubBGbuffer, SubBGbackbuffer, 192*256*2 );
	if( SubBGimage.image.data16 == NULL ){
		memset( SubBGbackbuffer, 0, 192*256*2 );
	} else{
		DC_FlushRange( SubBGimage.image.data16, sizeof(256*192*2) );
		dmaCopyWords( 0, SubBGimage.image.data16, SubBGbackbuffer, 256*192*2 );
	}
	return 0;
}
bool GraphicsEng::Update(){
	swiWaitForVBlank();
	return UpdateMain() | UpdateSub();
}
GraphicsEng::~GraphicsEng() {	
}
