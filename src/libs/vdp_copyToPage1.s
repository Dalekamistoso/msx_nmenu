;===============================================================================
; Copy the page 0 to page 1
; In:  -
; Out: -
; Chg: A C HL
;
.include "vdp.s"


;void copyToPage1();
_copyToPage1::
copyToPage1::
		ld      hl, #.copyToPage1Data
		ld      de, #BLT_SX
		ld      bc, #15
		ldir
		ld      hl,#BLT_SX
		call    fastVCopy
		ret

.copyToPage1Data:
		.dw    0				; SX (not used)
		.dw    0				; SY (not used)
		.dw    0				; DX
		.dw    256				; DY
		.dw    512				; NX
		.dw    212				; NY
		.db    0				; COL
		.db    0				; ARG
		.db    CMD_YMMM			; CMD
