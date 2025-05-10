;===============================================================================
; In:  HL    Palette RAM address
;
.include "vdp.s"

;void setPalette(char *paletteAddress) __sdcccall(1);
_setPalette::
								; HL = Param paletteAddress
setPalette::
		xor     a				; set pointer to zero.
		di
		out     (0x99), a
		ld      a, #(16+128)
		ei
		out     (0x99), a
		ld      bc, #0x1f9a		; out 32 bytes ($20) to port $9A
		otir
		ret
