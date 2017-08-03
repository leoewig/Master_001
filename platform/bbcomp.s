	; Fixed-location constants
	xdef  _app_bbcompat

.bbcomp:	section
	switch	.bbcomp

	; Boot block compatibility byte. This is checked for suitability
	; by the boot block, which will refuse to attempt to jump to
	; application code if it's not right. Therefore, this _must_
	; remain at a fixed location

	org	$0000
_app_bbcompat: dc.b $81

end
