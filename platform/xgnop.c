/**
 * @file
 * @ingroup xgate
 *
 * Define a word containing the XGATE NOP op-code.
 *
 * The pipelining of the XGATE means that an illegal-instruciton error can
 * occur if the word after a branch or return instruction is an illegal opcode.
 * Therefore we must take precautions to ensure that the last word of XGATE
 * program code is followed by a valid XGATE instruction.
 *
 * We do this by defining a variable containing the the NOP instruction and
 * getting the linker file to locate it in the .xnop section after all code
 * (.xtext).
 *
 * $Id: xgnop.c 3644 2012-09-10 12:52:29Z gvs $
 */
/* Copyright (c) Linkedriving Tech 2012 */

#include "basic_types.h"

#define	XGATE_NOP		0x0100u  /**< machine code for NOP
					   instruction */

#pragma section const {xnop}
/**
 * @lint Investigate : XNOP Symbol not referenced\n
 * -esym(528,xnop)
 */
/*lint -esym(528,xnop) */
/**
 * dummy constant defined to insert a NOP instruction after the last bit of
 * XGATE code
 */
static const uint16_t xnop = XGATE_NOP;

#pragma section const {}
