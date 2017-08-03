; Magic Word 1 Constants
	xdef	_app_magic1

   appmagic: equ $4D42 ; "MB"


.magic1:	section
	switch	.magic1

	org	$0000

_app_magic1: dc.w appmagic

; Trap to prevent runaway code entering the application code.
    swi
end
