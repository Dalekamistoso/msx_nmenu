#include <stdint.h>
#include "vdp.h"
#include "msx_const.h"


void bios_copyToVRAM(void *address, uint16_t vram, uint16_t size) __naked __sdcccall(0)
{
	address, vram, size;
/*
	LDIRVM
	Address  : #005C
	Function : Block transfer to VRAM from memory
	Input    : BC - Block length
	           HL - Start address of memory
	           DE - Start address of VRAM
	Registers: All
*/
	__asm
		push ix
		ld   ix, #4
		add  ix, sp
		ld   l, 0(ix)
		ld   h, 1(ix)
		ld   e, 2(ix)
		ld   d, 3(ix)
		ld   c, 4(ix)
		ld   b, 5(ix)

		ld ix, #LDIRVM		; HL: memory address | DE: VRAM address | BC: length
		BIOSCALL 			; (from MSXDOS only)

		pop  ix
		ret
	__endasm;
}