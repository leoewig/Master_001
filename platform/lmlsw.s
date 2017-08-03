;	LONG MULTIPLICATION BY SIGNED INT
;	Copyright (c) 2013 by COSMIC Software
;	- 1st operand in X:D
;	- 2nd operand in Y
;
	xdef	c_lmlsw
	.dcall	"6,0,c_lmlsw"
;
c_lmlsw:
	pshy			; save word
	exg	d,x		; MSW in D, LSW in X
	emul			; H * W
	exg	d,x		; LSW in D
	ldy	0,s		; get word
	bpl	pos		; if negative
	pshd			; stack LSW
#ifdef MCX
	subx	2,s+		; update MSW
#else
	exg	d,x
	subd	2,s+		; update MSW
	exg	d,x
#endif
pos:
	emul			; L * W
	exg	d,y		; MSW in D
	leax	d,x		; update result MSW
	tfr	y,d		; LSW in place
	puly			; restore word
	rts			; and return
;
	end
