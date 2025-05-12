#include "vdp.h"


// Reset palette to default values
void resetPalette() __naked
{
	__asm
		// INIPLT: Initialises the palette (current palet is saved in VRAM)
		ld   ix,#0x0141
		call CALSUB
	__endasm;
}