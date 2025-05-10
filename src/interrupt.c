/*
	Thanks all people at MSX.ORG thread:
		https://www.msx.org/forum/msx-talk/development/getting-htimi-interrupt
*/
#include "msx_const.h"


extern void _interrupt_hook(void);


void interrupt_init() __naked
{
	__asm
		di

		ld	hl, #H_TIMI
		ld	de, #oldHook$
		ld	bc, #5
		ldir

		ld	a, #0xF7				; RST #30
		ld	(H_TIMI), a				; In H_TIMI hook
		call getSlot$				; Our Slot in A
		ld	(H_TIMI+1),a			; Next H_TIMI hook byte
		ld	hl, #interruption$		; Our H_TIMI routine address
		ld	(H_TIMI+2), hl

		ei
		ret

	getSlot$:
		in	a,(#0xA8)
		rrca
		rrca
		and	#3
		ld	c,a						;C = Slot
		ld	b,#0
		ld	hl,#EXPTBL
		add	hl,bc
		ld	a,(hl)
		and	#0x80
		or	c
		ld	c,a
		inc	hl
		inc	hl
		inc	hl
		inc	hl
		ld	a,(hl)
		and	#0x0C
		or	c
		bit	7,a
		ret	nz
		and	#3
		ret

	interruption$:
		exx

		call _interrupt_hook		; Call our hook

		exx

	oldHook$:
		.db 0xC9, 0xC9, 0xC9, 0xC9, 0xC9

	__endasm;
}


void interrupt_end() __naked
{
	__asm
		di

		ld	hl, #oldHook$
		ld	de, #H_TIMI
		ld	bc, #5
		ldir

		ei
		ret
	__endasm;
}
