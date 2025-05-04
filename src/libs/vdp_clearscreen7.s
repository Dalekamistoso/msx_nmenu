;===============================================================================
; Clear screen7 lines
; In:  HL  Start line
;      DE  Num. lines to clear
; Out: -
; Chg: A C HL
;
.include "vdp.s"

;void clearSC7lines(uint16_t startline, uint16_t numLines) __sdcccall(1);
_clearSC7lines::
								; HL = Param startline
								; DE = Param numlines
clearSC7lines::
		push    hl
		push    de
		call    .clearsc7_ldir
		pop     de
		pop     hl
		ld      (BLT_DY), hl
		ld      (BLT_NY), de
		jr      .clearsc7_call

;void clearSC7();
_clearSC7::
clearSC7::
		call    .clearsc7_ldir
.clearsc7_call:
		ld      hl,#BLT_SX
		call    fastVCopy
		ret
.clearsc7_ldir:
		ld      hl, #.clearData7
		ld      de, #BLT_SX
		ld      bc, #15
		ldir
		ret

.clearData7:
		.dw    0				; SX (not used)
		.dw    0				; SY (not used)
		.dw    0				; DX
		.dw    0				; DY
		.dw    512				; NX
		.dw    212				; NY
		.db    0				; COL
		.db    0				; ARG
		.db    CMD_HMMV			; CMD
