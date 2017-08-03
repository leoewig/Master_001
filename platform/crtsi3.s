;  C STARTUP FOR S12X
;
;  $Id: crtsi2.s 5192 2012-11-21 13:36:14Z gvs $
;  Copyright (c) Linkedriving Tech 2012
   xdef  _exit
   xref  f_platform_main, __stack

   xref  __idesc__, __sgconst, __sdirect
   xref  _GPAGE, _PPAGE, _DIRECT


; External locations
   xdef  static_init

; Entrypoints - External declarations
   xdef  _app_reset
   xdef  static_init_complete

;------------------------------------------------------------------------------
; Entrypoints
;------------------------------------------------------------------------------

_app_reset:                ; Normal reset vector

;------------------------------------------------------------------------------
; Startup
;------------------------------------------------------------------------------

start:
        
        ; 'Call' static initialisation code.
        ;
        ; ------------------------------------------------------------------
        ; NB Only register D (i.e. A and B) is preserved by static_init code.
        ; The Stack Pointer isn't
        ; ------------------------------------------------------------------
        lbra      static_init

        ; provide a location for static_init to jump back to
static_init_complete:

        ; Data is now initialised

        lds      #__stack               ; Initialize stack pointer
                                        ; Stack monitor feature will deal with
                                        ; inserting a pattern

; Set up GPAGE for use by Cosmic-generated code

#ifdef  GPAGE_MODD
   ; Initialise GPAGE for +modd, will work for +modg, too
   movb #phigh(__sgdata),_GPAGE ; global page register
#else
   ; Initialise GPAGE for +modc, will work for +modg, too
   movb #phigh(__sgconst),_GPAGE ; global page register
#endif

;------------------------------------------------------------------------------
; Star12 Main Entrypoint
;------------------------------------------------------------------------------

   ; Now invoke the C-code.
   call  f_platform_main           ; execute main

;------------------------------------------------------------------------------
; Star12 Invalid Exit Trap
;------------------------------------------------------------------------------

_exit:
   bra   _exit            ; stay here

   end
