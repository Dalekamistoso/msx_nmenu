;===============================================================================
; In:  A: page view to be activated [0..3]
; Out: -
; Chg: A BC HL DE
;
.include "vdp.s"

;void setVPage(uint8_t page) __sdcccall(1);
_setVPage::
							; A = Param page
setVPage::
		sla  a
		sla  a
		sla  a
		sla  a
		sla  a
		or   #0b00011111
		ld   b,a
		ld   c,#2
		jp   setRegVDP
