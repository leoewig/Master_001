; Magic Word 2 Constants
	xdef	_app_magic2

   appmagic: equ $4D42 ; "MB"

.magic2:	section
	switch	.magic2

	org	$0000
_app_magic2:   dc.w appmagic

end
