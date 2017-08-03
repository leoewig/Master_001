; Initialise C static data by reading the Cosmic intialisation table
;
; The Cosmic supplied code is too broken to use in a large application:
; see the notes below for details.
;
; This code is executed before we have a functioning stack so it is 'called'
; by branching to the label static_init in this file.  This code 'returns'
; by branching to the label static_init_complete in the calling code.
;
; The calling code can only assume that D is preserved.  THE STACK POINTER
; AND ANYTHING ON THE STACK WILL BE TRASHED.
; 
; This code has been broken out from crtsi.s so that it can be shared by
; the bootloader and applications.  Bear this in mind if you need to edit it.
;
; Assumptions
; -----------
; + Application code is compiled without .bss segments (because we don't
;   bother to zero them).  Code for .bss (but not .gbss, .xbss etc.) is only
;   compiled in if FBL is defined
; + There must be at least 8 bytes of stack reserved in near memory.
;   It uses this area to create a direct page for efficient addressing. 
; + The code is loaded on the same bank as its caller so that they can get
;   from one to the other with LBRA instructions.
;
;  Copyright (c) Linkedriving Tech 2012
;
;  $Id: crtsi2.s 5192 2012-11-21 13:36:14Z gvs $
;
   xref  __idesc__, __stack

   xref  _GPAGE, _PPAGE, _DIRECT



; Entrypoint
   xdef  static_init

; Point to return to
   xref  static_init_complete


static_init:
;------------------------------------------------------------------------------
; 'C' Initialisation - Initialise linker-defined (-id) areas.
;------------------------------------------------------------------------------
;
; Cosmic initialisation table layout:
;
; __idesc__:            local ROM data start for first segment
; __idesc__+2:          array of segment entries
;
; Paged memory segment: (the usual case)
;       0:    flag byte with bit 7 set
;       1:    zero byte
;       2:    RAM GPAGE (i.e always $0f!)
;       3,4:  global RAM address
;       5,6:  local ROM end address
; Unpaged memory segment: 
;       0:    flag byte with bit 7 clear
;       1,2:  RAM address
;       3,4:  ROM end address
;
; If the flag byte has neither bit 5 nor bit 6 set, it is for a relocatable
; code segment and not data
;
; The table is terminated with a flag byte of zero.
;
; Problems with the initialisation table
; --------------------------------------
; The table contains only a local ROM addresses but we frequently have 
; more initialisation data than fits into a single ROM PPAGE.
; Rather than attempt to infer the correct PPAGE, we convert the ROM addresses
; to a global address.
;
; N.B.  This code requires all initialisation data to be contained in a single
; Global ROM page.


; Direct Page layout
; ------------------
; The direct page is 256 bytes of RAM which can be addressed particularly
; efficiently by loading its base address into the _DIRECT register.
; Set up a direct page where the stack will live for efficient addressing
; (and more readable assembler!)
; stack-1 is used because the top of the stack may well be the first byte of
; another (256 byte) direct page.
; After this, we will have a direct page sitting just below __stack

; The DIRECT register can only be written once, so if it is non-zero it has
; already been written and cannot be written again.
; Writing it a second time will cause the code to behave differently if a
; Debugger is attached, so avoid doing so.

; A DIRECT page set to 0 is not valid for this code, as it would clobber the
; I/O registers. The check below will be ineffective if DIRECT as previously
; been set to 0.

        tst     _DIRECT         ; Inspect the contents of the DIRECT register
                                ; The Z flag is set as a side-effect
        bne     skip_direct_set ; If the value read was non-zero then skip
                                ; the setting of DIRECT again
        ; If the DIRECT register was 0, set it to point to the stack
        movb    #high(__stack-1),_DIRECT
skip_direct_set:

; Define the memory layout of the direct page
        switch .ubsct           ; tell assembler these are uninitialised
                                ; directly-addressed symbols
preserved_d:            ds.w    1       ; Place to save the Accumulators while
                                        ; we do our work
next_entry:             ds.w    1       ; Pointer to start next initialisation
                                        ; structure (i.e. to the flag byte)
global_ramstart:        ds.w    1       ; Global address of first byte of RAM
                                        ; in a segment to be initialised
local_romstart:         ds.w    1       ; Local address of segment's first byte
                                        ; of ROM initialisation data
local_romend:           ds.w    1       ; Local address following last byte
                                        ; of segment's ROM data
romstart_ppage_bits:    ds.w    1       ; Temporary used in PPAGE to GPAGE 
                                        ; address conversion
odd_byte_to_write:      ds.b    1       ; Flag showing that after writing
                                        ; words, there is a final byte on the
                                        ; end of the segment to intialise


; Page constants
RAM_GPAGE:              equ     $0f     ; all 64k of RAM lives on GPAGE 0xf

        switch .text

        ; Save the double accumulator
        std     preserved_d

        ; Set up paging for initialisation
        movb    #page(__idesc__),_PPAGE ; initialisation table can now be
                                        ; addressed via PPAGE
        ; Read the ROM start address from the table
        ldx     #__idesc__              ; load address of __idesc__ into X
        leay    2,x                     ; load address of first flag byte in Y
        ldx     0,x                     ; Replace X with the actual address
                                        ; stored at __idesc__
        stx     local_romstart          ; and save it to local_romstart

        ; Convert the ROM start address to a GPAGE address.  We can get the
        ; GPAGE using the phigh() 'operator' but we need to massage the
        ; local address into the lower 16bits of a global address by
        ; replacing the top two bits of the local address with the bottom
        ; two bits of the PPAGE
        andx    #$3fff                  ; clear the top two bits of the local
                                        ; ROM start address

        ; get lower 2 bits of ppage by shifting it left 6 times!
        ldaa    #page(__idesc__)
        asla                            ; 1
        asla                            ; 2
        asla                            ; 3
        asla                            ; 4
        asla                            ; 5
        asla                            ; 6
        clrb
        ; D (i.e. A:B) now contains the bottom two bits of the initialisation
        ; PPAGE in its top two bits, so it is ready to OR into the bottom
        ; 14 bits stored in X.
        ; Can't do it directly from D so save to direct memory and read back in
        std     romstart_ppage_bits
        orx     romstart_ppage_bits     ; X now contains lower 16 bits of the
                                        ; global address of the first byte
                                        ; of initialisation data

copy_next_segment:
        ; At this point we have:
        ;       X       - global pointer to next byte of ROM data
        ;       Y       - local pointer to flag byte of next entry in the
        ;                 initialisation table

        ldaa    1,y+                    ; load flag into A and move Y on to
                                        ; point to RAM page info (or RAM address
                                        ; if unpaged)
        beq     copy_complete           ; flag is zero, so we are all done
        bpl     after_page_skip         ; 'sign' bit of flag is clear so this
                                        ; is an unpaged entry and Y already
                                        ; points to the address.
        leay    2,y                     ; skip (useless) RAM page info.
                                        ; NB leay leaves CCR flags alone
after_page_skip:
        bita    #$60                    ; test for relocatable code segment
        bne copy_data                   ; and jump if it isn't

        ; Entry is a relocatable code segment so:
        ; jump over RAM start
        ; read and save ROM end for next iteration
        ; add length of code segment to X to jump over the code data

        ldd     2,+y                    ; skip RAM start and load ROM end
        std     local_romend            ; and save
        subd    local_romstart          ; D now contains length of code segment
        leax    d,x                     ; so add it to y
        leay    2,y                     ; move y on to next flag byte
        sty     next_entry              ; and save
        bra     copy_loop_complete

copy_data:
        ldd     2,y+                    ; load RAM start address into D
                                        ; (and move Y on to ROM address)
        std     global_ramstart         ; and save to the direct page

        ldd     2,y+                    ; load ROM end address into D
                                        ; (and move Y on to next flag byte)
        sty     next_entry              ; save for later
        std     local_romend            ; save for later
        subd    local_romstart          ; D = ROM end - ROM start (i.e. length)
        tfr     d,s                     ; put length into SP for safe keeping
        
        ; We will be copying words so we need to know whether there will be
        ; a final byte to write.  The byte length is in D so the LSB is
        ; the LSB of B.
        andb    #1                      ; see whether number of bytes is odd
        stab    odd_byte_to_write       ; and save result

        tfr     s,d                     ; Put byte count back into D
        lsrd                            ; Right shift to turn it into a word
                                        ; count
        tfr     d,s                     ; And store it back in SP, where it can
                                        ; be used as the loop counter

        ldy     global_ramstart         ; put the address of the first RAM byte
                                        ; of this segment into Y

copy_loop:
        ; Time to actually copy the data
        ; We have
        ;       X:      global pointer to ROM initialisation data
        ;       Y:      global pointer to RAM needing initialisation
        ;       SP:     remaining number of words to write

        movb    #phigh(__idesc__),_GPAGE        ; access ROM first
        gldd    2,x+                    ; load word into D
        movb    #RAM_GPAGE,_GPAGE               ; and now RAM
        gstd    2,y+
        dbne    s,copy_loop             ; decrement counter and branch if
                                        ; more writes required
        ; now see about writing final byte if any
        ldaa    odd_byte_to_write
        beq     copy_loop_complete      ; branch if byte count was even

        ; write final byte
        movb    #phigh(__idesc__),_GPAGE        ; access ROM first
        gldaa   1,x+                    ; load word into D
        movb    #RAM_GPAGE,_GPAGE               ; and now RAM
        gstaa   1,y+
        
copy_loop_complete:
        ; put a pointer to the next flag byte into Y
        ldy     next_entry
        ; and update next loop's start address with this loop's end
        ldd     local_romend
        std     local_romstart
        bra     copy_next_segment

copy_complete:

#ifdef FBL
;------------------------------------------------------------------------------
; Initialise the BSS region to 0
; This is only done for the FBL and not the application because the number of
; .bss segments was proliferating: (.bss, .gbss, .gbss_s12_only, .xbss).
;------------------------------------------------------------------------------
    xref __sbss, __ebss
    ldx   #__sbss          ; start address
    ldd   #0

bss_init:
    std   2,x+
    cpx   #__ebss          ; end address
    blt   bss_init
#endif
        ; finally, restore D to its former value
        ldd     preserved_d

        ; Data is now initialised so jump back to the caller
        lbra static_init_complete
