/*
	Secret (demo) Project
	by Mandarine
*/

#include "includes.prl"
#include <intuition/intuition.h>
#include <graphics/gfxbase.h>
#include <hardware/dmabits.h>
#include <hardware/intbits.h>
#include <hardware/custom.h>
#include <graphics/gfxmacros.h>

/*
Common
*/
#include "board.h"
#include "ptreplay.h"
#include "ptreplay_protos.h"
#include "ptreplay_pragmas.h"

/*
Routines
*/
#include "bitmap_routines.h"
#include "font_routines.h"
#include "sound_routines.h"	
#include "font_routines.h"
#include "color_routines.h"
#include "voxel_routines.h"
#include "fx_routines.h"
#include "tinfl.c"

/*
Graphic assets
*/
#include "screen_size.h"
#include "copper_lists.h"

/* Music */
struct Library *PTReplayBase;
struct Module *theMod;
UBYTE *mod = NULL;

UBYTE tin_fl_enable_waittof = 0;

struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;
extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;
extern struct Custom far custom;

struct View my_view;
struct View *my_old_view;

/*
	Double buffer and scrolling
*/
short scr1_x_offset = 0, scr1_y_offset = 0;
short scr2_x_offset = 0, scr2_y_offset = 0;
int dbuffer_offset_1 = 0;
int dbuffer_offset_2 = 0;
BOOL enable_dbuffer_1 = FALSE;
BOOL enable_dbuffer_2 = FALSE;

/* ViewPort 1 */
struct ViewPort view_port1;
struct RasInfo ras_info1;
struct BitMap bit_map1;
struct RastPort rast_port1;
struct RasInfo ras_info2;
struct BitMap bit_map2;
struct RastPort rast_port2;


/* Data */
struct BitMap *bitmap_element_city = NULL;

extern UWORD trabant_facing_groundPaletteRGB4[8];
extern UWORD trabant_facing_carPaletteRGB4[8];

extern UWORD trabant_side_PaletteRGB4[8];

UWORD *pal_facing_car_fadein = NULL;
UWORD *pal_facing_car_fadeout = NULL;

UWORD *pal_side_car_fadein = NULL;
UWORD *pal_side_car_fadeout = NULL;

#define PTREPLAY_MUSIC
struct SoundInfo *background = NULL;

UBYTE is_cpu_a_020(void)
{
	printf("Kickstart v%d, ", SysBase->LibNode.lib_Version);

	if (SysBase->AttnFlags && AFF_68020)
	{
		printf("CPU is a 68020 or higher.\n");
		return 1;
	}

	printf("CPU is a 68000 or 68010.\n");
	return 0;
}

void initMusic(void)
{
#ifdef PTREPLAY_MUSIC
	if (SysBase->LibNode.lib_Version >= 36)
		if (!AssignPath("Libs","Libs"))
			exit(0); //FIXME // init_conerr((UBYTE *)"Failed to Assign the local Libs drawer. Please copy ptreplay.library into your Libs: drawer.\n");

	if (!(PTReplayBase = OpenLibrary((UBYTE *)"ptreplay.library", 0)))
	{
		exit(0); //FIXME
	}

	mod = load_zlib_getchipmem((UBYTE *)"assets/jhericurl-med-mandarine.dat", 120930, 232562);
	// mod = NULL;
	// mod = load_getchipmem((UBYTE *)"assets/miami_vice.mod", 7394);
#else
	background = PrepareSound( "assets/music_loop.8svx" );
	if( !background )
	{
		printf( "Could not prepare the sound effect!\n" );
		exit(0); //FIXME
	}
#endif
}

void playMusic(void)
{
#ifdef PTREPLAY_MUSIC
	if (mod != NULL)
	{
		theMod = PTSetupMod((APTR)mod);
		PTPlay(theMod);
	}
#else
	if (background != NULL)
	{
		PlaySound( background, MAXVOLUME, LEFT0, NORMALRATE, NONSTOP );
		PlaySound( background, MAXVOLUME, RIGHT0, NORMALRATE, NONSTOP );
	}
#endif
}

/* Returns all allocated resources: */
void close_demo(STRPTR message)
{
	int loop;

	WaitBlit();
	free_allocated_bitmap(bitmap_element_city);
	freeTrabantFacingGround();
	freeTrabantFacingCar();
	freeTrabantLight();

	if (pal_facing_car_fadein != NULL)
		FreeMem(pal_facing_car_fadein, sizeof(UWORD) * 16 * 16);
	if (pal_facing_car_fadeout != NULL)
		FreeMem(pal_facing_car_fadeout, sizeof(UWORD) * 16 * 16);

	freeTrabantSideGround();
	freeTrabantSideCar();

	if (pal_side_car_fadein != NULL)
		FreeMem(pal_side_car_fadein, sizeof(UWORD) * 16 * 16);
	if (pal_side_car_fadeout != NULL)
		FreeMem(pal_side_car_fadeout, sizeof(UWORD) * 16 * 16);	

	/*	Free the voxel structures */
	deleteMatrix();
	deletePointList();

	/* Free automatically allocated display structures: */
	FreeVPortCopLists( &view_port1 );
	FreeCprList( my_view.LOFCprList );

	/* Deallocate the display memory, BitPlane for BitPlane: */
	for( loop = 0; loop < DEPTH1; loop++ )
		if( bit_map1.Planes[ loop ] )
			FreeRaster( bit_map1.Planes[ loop ], WIDTH1, HEIGHT1 + 8);

	for( loop = 0; loop < DEPTH1; loop++ )
		if( bit_map2.Planes[ loop ] )
			FreeRaster( bit_map2.Planes[ loop ], WIDTH1, HEIGHT1 + 8);		

#ifdef PTREPLAY_MUSIC
	/*	Stop music */
	if (mod != NULL)
	{
		PTStop(theMod);
		PTFreeMod(theMod);
		FreeMem(mod, 232562);
	}

	if (PTReplayBase) CloseLibrary(PTReplayBase);
#else
	if (background != NULL)
	{
		StopSound( LEFT0 );
		StopSound( RIGHT0 );
		RemoveSound( background );
	}
#endif
	/* Close the Graphics library: */
	if(GfxBase)
		CloseLibrary((struct Library *)GfxBase);

	/* C Close the Intuition library:  */
	if(IntuitionBase)
		CloseLibrary((struct Library *)IntuitionBase);

	/* Restore the old View: */
	LoadView( my_old_view );

	/* Print the message and leave: */
	printf( "%s\n", message );

	exit(0);
}

void __inline swapDoubleBuffer1(void)
{
	if (dbuffer_offset_1 == 0)
		dbuffer_offset_1 = DISPL_WIDTH1;
	else
		dbuffer_offset_1 = 0;
}

void __inline swapDoubleBuffer2(void)
{		
	if (dbuffer_offset_2 == 0)
		dbuffer_offset_2 = DISPL_WIDTH1;
	else
		dbuffer_offset_2 = 0;
}

void __inline resetViewportOffset(void)
{
	dbuffer_offset_1 = 0;
	dbuffer_offset_2 = 0;

	(&view_port1)->RasInfo->RxOffset = 0;
	(&view_port1)->RasInfo->RyOffset = 0;
	(&view_port1)->RasInfo->Next->RxOffset = 0;
	(&view_port1)->RasInfo->Next->RyOffset = 0;
}

void setPaletteToBlack(void);
void setPaletteFacingCar(void);
void precalculateFacingCarFades(void);
BOOL fxFacingCar(unsigned int);
void precalculateSideCarFades(void);
BOOL fxSideCar(unsigned int);

void main()
{
	int loop;
	unsigned int fx_clock = 0;
	unsigned short demo_phase = 0;
	unsigned int palette_fade;

	UWORD angle;

	/* Open the Intuition library: */
	IntuitionBase = (struct IntuitionBase *)
	OpenLibrary( "intuition.library", 0 );
	if( !IntuitionBase )
		close_demo( "Could NOT open the Intuition library!" );

	/* Open the Graphics library: */
	GfxBase = (struct GfxBase *)
	OpenLibrary( "graphics.library", 0 );
	if( !GfxBase )
		close_demo( "Could NOT open the Graphics library!" );

	tinflDiag();

	printf("DEMO IS LOADING!\n");

	is_cpu_a_020();

	/* Save the current View, so we can restore it later: */
	my_old_view = GfxBase->ActiView;

	/* Prepare the View structure, and give it a pointer to */
	/* the first ViewPort:                                  */
	InitView( &my_view );
	my_view.ViewPort = &view_port1;

	/* 2. Prepare the ViewPort structures, and set some important values: */

	/* ViewPort 1 */
	InitVPort( &view_port1 );
	view_port1.DWidth = DISPL_WIDTH1;      /* Set the width.                */
	view_port1.DHeight = DISPL_HEIGHT1;    /* Set the height.               */
	view_port1.DxOffset = 0;         /* X position.                   */
	view_port1.DyOffset = 0;         /* Y position.                   */
	view_port1.RasInfo = &ras_info1; /* Give it a pointer to RasInfo. */
	view_port1.Modes = DUALPF|PFBA;         /* Low resolution.               */
	view_port1.Next = NULL;   /* Pointer to next ViewPort.     */

	/* 3. Get a colour map, link it to the ViewPort, and prepare it: */

	/* ViewPort 1 */
	view_port1.ColorMap = (struct ColorMap *) GetColorMap(COLOURS1 * 4);
	if( view_port1.ColorMap == NULL )
		close_demo( "Could NOT get a ColorMap!" );

	/* Prepare the BitMaps */
	/* ViewPort 1, Bitmap 1 */
	InitBitMap( &bit_map1, DEPTH1, WIDTH1, HEIGHT1 + 8);
	/* Allocate memory for the Raster: */ 
	for( loop = 0; loop < DEPTH1; loop++ )
	{
		bit_map1.Planes[ loop ] = (PLANEPTR) AllocRaster( WIDTH1, HEIGHT1 + 8);
		if( bit_map1.Planes[ loop ] == NULL )
			close_demo( "Could NOT allocate enough memory for the raster!" );
	/* Clear the display memory with help of the Blitter: */
		BltClear( bit_map1.Planes[ loop ], RASSIZE( WIDTH1, HEIGHT1 + 8), 0 );
	}

	/* ViewPort 1, Bitmap 2 */
	InitBitMap( &bit_map2, DEPTH1, WIDTH1, HEIGHT1 + 8);
	/* Allocate memory for the Raster: */ 
	for( loop = 0; loop < DEPTH1; loop++ )
	{
		bit_map2.Planes[ loop ] = (PLANEPTR) AllocRaster( WIDTH1, HEIGHT1 + 8);
		if( bit_map2.Planes[ loop ] == NULL )
			close_demo( "Could NOT allocate enough memory for the raster!" );
	/* Clear the display memory with help of the Blitter: */
		BltClear( bit_map2.Planes[ loop ], RASSIZE( WIDTH1, HEIGHT1 + 8), 0 );
	}

	/* Prepare the RasInfo structure */

	/* ViewPort 1, Raster 1 */
	ras_info1.BitMap = &bit_map1; /* Pointer to the BitMap structure.  */
	ras_info1.RxOffset = 0;       /* The top left corner of the Raster */
	ras_info1.RyOffset = 0;       /* should be at the top left corner  */
	              /* of the display.                   */
	ras_info1.Next = &ras_info2;        /* Dual playfield */

	/* ViewPort 1, Raster 2 */
	ras_info2.BitMap = &bit_map2; /* Pointer to the BitMap structure.  */
	ras_info2.RxOffset = 0;       /* The top left corner of the Raster */
	ras_info2.RyOffset = 0;       /* should be at the top left corner  */
	              					/* of the display.                   */
	ras_info2.Next = NULL;        /* Single playfield - only one       */
	              				/* RasInfo structure is necessary.   */

	/* Prepare the RastPort, and give it a pointer to the BitMap. */
	/* ViewPort 1 */
	InitRastPort( &rast_port1 );
	rast_port1.BitMap = &bit_map1;
	InitRastPort( &rast_port2 );
	rast_port2.BitMap = &bit_map2;

	/* Create the display */
	MakeVPort(&my_view, &view_port1); /* Prepare ViewPort 1 */

	printf("Tinfl decrunches while waiting...\n");
	initMusic();

	WaitTOF();

	// setCityCopperList(&view_port1);

	MrgCop(&my_view);

	/*	Wait until the DF0: motor stops */
	// for(loop = 0; loop < 150; loop++)
	// 	WaitTOF();

	/* 8. Show the new View: */
	LoadView( &my_view );

	WaitTOF();
	WaitTOF();

	Forbid();
	Disable();
	WaitBlit();
	
	OFF_SPRITE;
	// OFF_VBLANK;

	// setupMatrix();
	// fillMatrixWithRandomData();
	// buildPointListFromMatrix();
	// angle = 0;

	// setPaletteFacingCar();
	setPaletteToBlack();

	precalculateFacingCarFades();
	loadTrabantFacingGround();
	loadTrabantFacingCar();
	loadTrabantLight();

	precalculateSideCarFades();
	loadTrabantSideGround();
	loadTrabantSideCar();

	tin_fl_enable_waittof = 1;

	playMusic();
	// drawElementCity(&bit_map1);
	// drawElementCity(&bit_map2);
	// LoadRGB4(&view_port1, trabant_facing_groundPaletteRGB4, 8);

	#define DMPHASE_FACING_CAR 0
	#define DMPHASE_SIDE_CAR 1 << 4
	demo_phase = 0;
	fx_clock = 0;

	while((*(UBYTE *)0xBFE001) & 0x40)
	{
		// SetAPen(&rast_port1, 0);
		// // RectFill(&rast_port1, (DISPL_WIDTH1 >> 1) - 64, (DISPL_HEIGHT1 >> 1) - 64 + dbuffer_offset, (DISPL_WIDTH1 >> 1) + 64, (DISPL_HEIGHT1 >> 1) + 64 + dbuffer_offset);
		// drawPointListToViewport(&rast_port1);
		// angle++;
		// angle &= 0x1FF;

		// rotatePointsOnAxisY(angle);
		// angle++;

		// printf("demo_phase = %d\n", demo_phase);
		// printf("Songpos = %d, Patpos = %d\n", PTSongPos(theMod), PTPatternPos(theMod));

		switch(demo_phase)
		{
			/*	
				Screen/FX :
				Facing car 
			***********************/
			case DMPHASE_FACING_CAR:
				if (PTSongPos(theMod) == 1 && PTPatternPos(theMod) > 4)
					demo_phase++;
				break;

			case DMPHASE_FACING_CAR | 1:
				palette_fade = 0;
				resetViewportOffset();
				drawTrabantFacingGround(&bit_map1);
				drawTrabantFacingCar(&bit_map2);
				demo_phase++;
				break;

			case DMPHASE_FACING_CAR | 2:
				/* Fade in */
				LoadRGB4(&view_port1, pal_facing_car_fadein + (palette_fade << 4), 16);
				palette_fade++;

				/*	Parallax fx */
				scr2_x_offset = (16 - palette_fade) >> 1;
				scr1_x_offset = scr2_x_offset >> 1;				

				if (palette_fade >= 16)
				{
					palette_fade = 0;
					demo_phase++;
				}
				break;

			case DMPHASE_FACING_CAR | 3:
				if (fxFacingCar(fx_clock))
					fx_clock++;
				else
					demo_phase++;
				break;

			case DMPHASE_FACING_CAR | 4:
				/* Fade out */
				LoadRGB4(&view_port1, pal_facing_car_fadeout + (palette_fade << 4), 16);
				palette_fade++;
				if (palette_fade >= 16)
				{
					palette_fade = 0;
					demo_phase++;
				}
				break;

			/*	Clear the screen */
			case DMPHASE_FACING_CAR | 5:
				SetRast(&rast_port1, 0);
				demo_phase++;
				break;

			case DMPHASE_FACING_CAR | 6:
				SetRast(&rast_port2, 0);
				demo_phase++;
				break;						

			/*	Next fx!!! */
			case DMPHASE_FACING_CAR | 7:
				resetViewportOffset();
				demo_phase = DMPHASE_SIDE_CAR;
				break;			

			/*	
				Screen/FX :
				Side car 
			***********************/
			case DMPHASE_SIDE_CAR:
				if (PTSongPos(theMod) == 2 && PTPatternPos(theMod) > 4)
				demo_phase++;
				break;

			case DMPHASE_SIDE_CAR | 1:
				setPaletteToBlack();
				drawTrabantSideGround(&bit_map1);
				demo_phase++;
				break;

			case DMPHASE_SIDE_CAR | 2:
				drawTrabantSideCar(&bit_map2, 0);
				palette_fade = 0;
				demo_phase++;
				break;				

			case DMPHASE_SIDE_CAR | 3:
				/* Fade in */
				LoadRGB4(&view_port1, pal_side_car_fadein + (palette_fade << 4), 16);
				palette_fade++;

				/*	Parallax fx */
				scr2_x_offset = palette_fade >> 1;
				scr1_x_offset = scr2_x_offset >> 1;	

				if (palette_fade >= 16)
				{
					palette_fade = 0;
					demo_phase++;
					fx_clock = 0;
				}
				break;

			case DMPHASE_SIDE_CAR | 4:
				if (fxSideCar(fx_clock))
					fx_clock++;
				else
					demo_phase++;
				break;

			case DMPHASE_SIDE_CAR | 5:
				/* Fade out */
				LoadRGB4(&view_port1, pal_side_car_fadeout + (palette_fade << 4), 16);
				palette_fade++;
				if (palette_fade >= 16)
				{
					palette_fade = 0;
					demo_phase++;
				}
				break;		
		}

		if (enable_dbuffer_1)
			swapDoubleBuffer1();
		if (enable_dbuffer_2)
			swapDoubleBuffer2();

		(&view_port1)->RasInfo->RxOffset = scr1_x_offset + dbuffer_offset_1;
		(&view_port1)->RasInfo->RyOffset = scr1_y_offset;

		(&view_port1)->RasInfo->Next->RxOffset = scr2_x_offset + dbuffer_offset_2;
		(&view_port1)->RasInfo->Next->RyOffset = scr2_y_offset;

		ScrollVPort(&view_port1);	

		WaitTOF();
	}

	// ON_VBLANK;

	WaitTOF();	

	/*	Wait for mouse up
		to prevent the mouse up event
		to mess with the OS gui
	*/
	while(!((*(UBYTE *)0xBFE001) & 0x40))
		WaitTOF();

	ON_SPRITE;

	Enable();
	Permit();

	close_demo("Stay 16/32!");
}

void setPaletteToBlack(void)
{
	short loop;
	for(loop = 1; loop < 16; loop++)
		SetRGB4(&view_port1, loop, 0x0, 0x0, 0x0);
}

/*
	Screen with a facing trabant
	Set the palette with both dual playfield layers
*/
void setPaletteFacingCar(void)
{
	UBYTE loop;

	for(loop = 1; loop < 8; loop++)
	{
		UWORD tmp_col = trabant_facing_groundPaletteRGB4[loop];
		SetRGB4(&view_port1, loop, (tmp_col & 0x0f00) >> 8, (tmp_col & 0x00f0) >> 4, tmp_col & 0x000f);
	}

	for(loop = 1; loop < 8; loop++)
	{
		UWORD tmp_col = trabant_facing_carPaletteRGB4[loop];
		SetRGB4(&view_port1, loop + 8, (tmp_col & 0x0f00) >> 8, (tmp_col & 0x00f0) >> 4, tmp_col & 0x000f);
	}
}


/*
	Prepare the fade in/fade out palette
	for the "facing trabant" scene
*/
void precalculateFacingCarFades(void)
{
	short palette_fade, palette_idx;
	UWORD tmp_col;

	/* Precalc the fade in/out */
	pal_facing_car_fadein = AllocMem(sizeof(UWORD) * 16 * 16, MEMF_CLEAR);
	for(palette_fade = 0; palette_fade < 16; palette_fade++)
		for(palette_idx = 0; palette_idx < 16; palette_idx++)
		{
			if (palette_idx < 8)
				tmp_col = trabant_facing_groundPaletteRGB4[palette_idx];
			else
				tmp_col = trabant_facing_carPaletteRGB4[palette_idx - 8];

			pal_facing_car_fadein[palette_idx + (palette_fade << 4)] = mixRGB4Colors(0x000, tmp_col, palette_fade);
		}

	pal_facing_car_fadeout = AllocMem(sizeof(UWORD) * 16 * 16, MEMF_CLEAR);
	for(palette_fade = 0; palette_fade < 16; palette_fade++)
		for(palette_idx = 0; palette_idx < 16; palette_idx++)
		{
			if (palette_idx < 8)
				tmp_col = trabant_facing_groundPaletteRGB4[palette_idx];
			else
				tmp_col = trabant_facing_carPaletteRGB4[palette_idx - 8];

			pal_facing_car_fadeout[palette_idx + (palette_fade << 4)] = mixRGB4Colors(tmp_col, 0x000, palette_fade);
		}
}

/*
	Prepare the fade in/fade out palette
	for the "side trabant" scene
*/
void precalculateSideCarFades(void)
{
	short palette_fade, palette_idx;
	UWORD tmp_col;

	/* Precalc the fade in/out */
	pal_side_car_fadein = AllocMem(sizeof(UWORD) * 16 * 16, MEMF_CLEAR);
	for(palette_fade = 0; palette_fade < 16; palette_fade++)
		for(palette_idx = 0; palette_idx < 16; palette_idx++)
		{
			if (palette_idx < 8)
				tmp_col = trabant_facing_groundPaletteRGB4[palette_idx];
			else
				tmp_col = trabant_side_PaletteRGB4[palette_idx - 8];

			pal_side_car_fadein[palette_idx + (palette_fade << 4)] = mixRGB4Colors(0x000, tmp_col, palette_fade);
		}

	pal_side_car_fadeout = AllocMem(sizeof(UWORD) * 16 * 16, MEMF_CLEAR);
	for(palette_fade = 0; palette_fade < 16; palette_fade++)
		for(palette_idx = 0; palette_idx < 16; palette_idx++)
		{
			if (palette_idx < 8)
				tmp_col = trabant_facing_groundPaletteRGB4[palette_idx];
			else
				tmp_col = trabant_side_PaletteRGB4[palette_idx - 8];

			pal_side_car_fadeout[palette_idx + (palette_fade << 4)] = mixRGB4Colors(tmp_col, 0x000, palette_fade);
		}
}

/*
	Screen with a facing trabant
	Draws the car & carlights
*/
#define FX_TRAB_CARLIGHT_DELAY	150
#define FX_TRAB_CARLIGHT_INTERVAL	4
BOOL fxFacingCar(unsigned int fx_clock)
{
	scr2_y_offset = 0;

	switch(fx_clock)
	{
		case 0:
			swapDoubleBuffer2();
			drawTrabantFacingGround(&bit_map1);
			drawTrabantFacingCar(&bit_map2);
			break;

		case 1:
			swapDoubleBuffer2();
			drawTrabantFacingCar(&bit_map2);
			break;			

		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 0):
			swapDoubleBuffer2();
			drawTrabantLight(&bit_map2, &rast_port2, 0);
			break;

		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 1):
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 3):
			swapDoubleBuffer2();
			drawTrabantLight(&bit_map2, &rast_port2, 1);
			break;

		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2):
			swapDoubleBuffer2();
			drawTrabantLight(&bit_map2, &rast_port2, 2);
			break;

		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 1:	
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 6:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 10:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 14:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 19:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 24:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 32:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 38:
			scr2_y_offset = 1;
			break;

		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) - 1:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 2:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 5:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 8:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 12:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 16:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 22:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 28:
		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 35:		
			scr2_y_offset = -1;
			break;

		case FX_TRAB_CARLIGHT_DELAY + (FX_TRAB_CARLIGHT_INTERVAL * 2) + 50:
			// freeTrabantFacingGround();
			// freeTrabantFacingCar();
			// freeTrabantLight();
			// dbuffer_offset_2 = 0;
			return FALSE;
			break;
	}

	return TRUE;
}

/*
	Screen with trabant seen from side
	Draws the car & shutting door
*/
#define FX_TRAB_SIDE_DELAY	25
BOOL fxSideCar(unsigned int fx_clock)
{
	scr2_y_offset = 0;

	switch(fx_clock)
	{
		case FX_TRAB_SIDE_DELAY:		
			swapDoubleBuffer2();
			drawTrabantSideCar(&bit_map2, 0);
		break;

		case FX_TRAB_SIDE_DELAY + 5:
			swapDoubleBuffer2();
			drawTrabantSideCar(&bit_map2, 1);
		break;

		case FX_TRAB_SIDE_DELAY + 10:
			swapDoubleBuffer2();
			drawTrabantSideCar(&bit_map2, 2);
		break;

		case FX_TRAB_SIDE_DELAY + 10 + 2:		
		case FX_TRAB_SIDE_DELAY + 10 + 6:		
		case FX_TRAB_SIDE_DELAY + 10 + 10:
		case FX_TRAB_SIDE_DELAY + 10 + 15:
			scr2_y_offset = 1;		
		break;

		case FX_TRAB_SIDE_DELAY + 10 + 4:		
		case FX_TRAB_SIDE_DELAY + 10 + 8:		
		case FX_TRAB_SIDE_DELAY + 10 + 12:
		case FX_TRAB_SIDE_DELAY + 10 + 18:
		case FX_TRAB_SIDE_DELAY + 10 + 21:		
			scr2_y_offset = -1;		
		break;

		case FX_TRAB_SIDE_DELAY + 100:
			// dbuffer_offset_2 = 0;
			return FALSE;
		break;		
	}
	return TRUE;
}
