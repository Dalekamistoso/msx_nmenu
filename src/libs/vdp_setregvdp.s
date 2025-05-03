;===============================================================================
; In:  B     Value to store
;      C     Register number
; Out: -
; Chg: A HL DE BC
;
.include "vdp.s"

;void setRegVDP8(char reg, char value) __sdcccall(1);
_setRegVDP8::
		ld      c,a					; A = Param reg
		ld      b,l					; L = Param value
setRegVDP8::
		ld      a,c
		sub     #8
		ld      hl,#RG8SAV
		jp      .setregvdp_cont

;void setRegVDP(char reg, char value) __sdcccall(1);
_setRegVDP::
		ld      c,a					; A = Param reg
		ld      b,l					; L = Param value
setRegVDP::
		ld      a,c
		ld      hl,#RG0SAV

.setregvdp_cont:
		ld      e,a
		ld      d,#0
		add     hl,de
		ld      (hl),b				;save copy of vdp value in system var

		ld      a,b
		di
		out     (0x99),a
		ld      a,#128
		add     c
		ei
		out     (0x99),a
		ret
