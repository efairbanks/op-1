#ifndef GRAPHICSENGINE
#define GRAPHICSENGINE
#define SCREEN_AREA SCREEN_WIDTH*SCREEN_HEIGHT
#define REDMASK 0x001F
#define BLUMASK 0x03E0
#define GRNMASK 0x7C00
#include <nds.h>
#include <math.h>

typedef int sGraphicsIndex;
typedef int SpriteIndex;

typedef enum {
	MAIN_LAYER,
	SUB_LAYER
} gLayer;
typedef enum {
	ALPHA_ON  = 1,
	ALPHA_OFF = 0
} pAlpha;
typedef u16* SpriteGFX;
class GraphicsEng {
	private:
	GraphicsEng();
	int		MainBGid;
	int		MainBGimageid;
	u16*	MainBGbuffer;
	u16*	MainBGbackbuffer;
	sImage	MainBGimage;
	bool	MainBGupdate;
	int		SubBGid;
	int		SubBGimageid;
	u16*	SubBGbuffer;
	u16*	SubBGbackbuffer;
	sImage	SubBGimage;
	bool	SubBGupdate;
	SpriteGFX MainSPRbuffer[64];
	SpriteGFX SubSPRbuffer[64];
	bool UpdateMain(); // If returns non-zero, error.
	bool UpdateSub();  // If returns non-zero, error.
	public:
	static GraphicsEng* Instance();
	void Test();
	void Reset();
	void SetBG( u16 color, pAlpha alpha, gLayer layer);
	void SetBG( u8* bgimage, int priority, u8 alphaindex, gLayer layer );
	void DrawPoint( int x, int y, u16 color, pAlpha alpha, gLayer layer );
	void DrawLine( int x1, int y1, int x2, int y2, u16 color, pAlpha alpha, gLayer layer );
	void DrawBezQuad( int x0, int x1, int x2, int y0, int y1, int y2, int segments, u16 color, pAlpha alpha, gLayer layer );
	void DrawRect( int x, int y, int width, int height, int borderwidth, u16 color, pAlpha alpha, gLayer layer );
	void DrawCircle( int x, int y, int radius, int borderwidth, u16 color, pAlpha alpha, gLayer layer );
	void DrawCircle( int x, int y, int angle, int radius, int borderwidth, u16 color, pAlpha alpha, gLayer layer );
  void DrawChar(int x, int y, char c, u16 color, gLayer layer);
  void DrawInt(int x, int y, int i, u16 color, gLayer layer);
  void DrawString(int x, int y, char* s, u16 color, gLayer layer);
	// ---- //
	sGraphicsIndex AddSpriteGraphics( u8* spriteimage, int graphicsoffset, SpriteSize size, gLayer layer );
	bool DrawSprite( int x, int y, bool hide, int priority, sGraphicsIndex index, int oamindex, SpriteSize sprsize, gLayer layer );
	// ---- //
	bool Update();  // If returns non-zero, error.
	// ---- //
	~GraphicsEng();
};
#endif
