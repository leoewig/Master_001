/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   shift_helpers.c
 *
 * description:  shifts
 * 
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
/**
 * @file
 * @ingroup fixedpoint
 */

#include "hardware.h"
#include "fixedpoint.h"		/* public header */
#include "fixedpoint_p.h"	/* private header */

/* Global variables */
/* (none) */

/* Private variables */

/* lookup table of ~((1<<(16-n))-1); we do this to improve on the worst-case execution
   time of our left shift routines.

   fixedpoint_u16_overflow[n] is a mask of bits which will be lost when we
   shift a 16-bit word left by n bits, but it's used by the 8- and 32-bit shift
   routines too.
*/

/**
 * @lint Deviation Required : Operators '~' and '<<' require recasting to underlying type for sub-integers, this is trusted code so suppress
 * lint -esym(960, 10.5)
 */
/*lint -esym(960, 10.5) */
const uint16 fixedpoint_u16_overflow[FP_U16_OVERFLOW_SIZE] = {
    0U,                       (uint16)~((1U << 15)-1U),
    (uint16)~((1U << 14)-1U), (uint16)~((1U << 13)-1U),
    (uint16)~((1U << 12)-1U), (uint16)~((1U << 11)-1U),
    (uint16)~((1U << 10)-1U), (uint16)~((1U <<  9)-1U),
    (uint16)~((1U <<  8)-1U), (uint16)~((1U <<  7)-1U),
    (uint16)~((1U <<  6)-1U), (uint16)~((1U <<  5)-1U),
    (uint16)~((1U <<  4)-1U), (uint16)~((1U <<  3)-1U),
    (uint16)~((1U <<  2)-1U), (uint16)~((1U <<  1)-1U),
    (uint16)~0U
};
/*lint +esym(960, 10.5) */

/******************************************************************************
 *
 * lsr_uxx: logical right shift of unsigned integers
 *
 *****************************************************************************/
FCODE uint32 lsr_u32( uint32 input, binpt_t shift )
{
    uint32 result;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    if( shift >= 32 ) {
	result = 0UL;
    }

    /* by handling the 16 <= shift < 32 case separately, we improve the
       worst-case execution time (which then happens when shift = 15, instead
       of when shift = 31) */

    else if ( shift >= 16 ) {
	uint16 shrt = (uint16)(input >> 16);
	shift -= 16;
        /**
         * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
         * lint -esym(960, 10.3)
         */
        /*lint -esym(960, 10.3) */
        /** @polyspace<MISRA-C:10.3:Not a defect:No action planned> Trusted code, so suppress */
	result = (uint32)(shrt >> shift);
        /*lint +esym(960, 10.3) */
    }

    else {
	/* shift < 16 */
	result = input >> shift;
    }

    return result;
}


FCODE uint16 lsr_u16( uint16 input, binpt_t shift )
{
    uint16 result;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    if( shift >= 16 ) {
	result = 0U;
    } else {
	result = input >> shift;
    }

    return result;
}

FCODE uint8  lsr_u8(  uint8 input, binpt_t shift )
{
    uint8 result;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    if( shift >= 8 ) {
	result = 0U;
    } else {
	result = input >> shift;
    }

    return result;
}

/******************************************************************************
 *
 * asr_sxx: arithmetic right shift of signed integers
 *
 *****************************************************************************/

/* we implement an arithmetic shift of a negative number by flipping the bits,
   doing a logical shift, then flipping back again.

   This works because:
     - it is intuitively clear that it doesn't matter if we shift a 1, or flip
       to 0, shift, and flip back

     - the flipping ensures that the extra (zero) bits shifted in on the
       left-hand-side by the logical shift are correctly replaced with ones.

   eg -128 ASR 2
      = 0x80 ASR 2 = 0xE0
      = ~(0x7F) ASR 2
      = ~(0x7F ASR 2)
      = ~(0x1F) = 0xE0

   eg -1 ASR 2
      = 0xFF ASR 2 = 0xFF
      = ~(0x00) ASR 2
      = ~(0x00 ASR 2)
      = ~(0x00) = 0xFF


   Notes: ISO C90 specifies that right shifts of negative numbers are
   implementation-defined. The Cosmic compiler doesn't actually seem to define
   it, though testing shows that it seems to use asr when shifting signed
   numbers. Still, we don't take advantage of that because:
      - it isn't portable (and thus inhibits hosted testing).
      - it violates MISRA 37.
      - we would have to try to pin down Cosmic to a definition.
      - it's fairly low-cost to do the bit-flipping to make it portable and
        compliant.
*/

FCODE sint32 asr_s32( sint32 input, binpt_t shift )
{
    uint32 temp;
    bool negative;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    /* flip the bits if need be */
    if( input < 0L ) {
	temp = ~(uint32)(input);
	negative = TRUE;
    } else {
	temp = (uint32)(input);
	negative = FALSE;
    }

    /* now do a logical right shift */
    if( shift >= 32 ) {
	temp = 0UL;
    }

    /* by handling the 16 <= shift < 32 case separately, we improve the
       worst-case execution time (which then happens when shift = 15, instead
       of when shift = 31) */

    else if ( shift >= 16 ) {
	uint16 shrt = (uint16)(temp >> 16);
	shift -= 16;
        /**
         * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
         * lint -esym(960, 10.3)
         */
        /*lint -esym(960, 10.3) */
        /** @polyspace<MISRA-C:10.3:Not a defect:No action planned> Trusted code, so suppress */
	temp = (uint32)(shrt >> shift);
        /*lint +esym(960, 10.3) */
    }

    else {
	temp >>= shift;
    }

    /* flip back if required */
    /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
    if( negative ) {
	temp = ~temp;
    }

    return (sint32)temp;
}

FCODE sint16 asr_s16( sint16 input, binpt_t shift )
{
    uint16 temp;
    bool negative;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    /* flip the bits if need be */
    if( input < 0 ) {
	temp = ~(uint16)(input);
	negative = TRUE;
    } else {
	temp = (uint16)(input);
	negative = FALSE;
    }

    /* now do a logical right shift */
    if( shift >= 16 ) {
	temp = 0U;
    } else {
	temp >>= shift;
    }

    /* flip back if required */
    /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
    if( negative ) {
	temp = ~temp;
    }

    return (sint16)temp;
}


FCODE sint8 asr_s8( sint8 input, binpt_t shift )
{
    uint8 temp;
    bool negative;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    /* flip the bits if need be */
    if( input < 0 ) {
        /**
         * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
         * Operators '~' and '<<' require recasting to underlying type for sub-integers, this is trusted code so suppress
         * lint -esym(960, 10.3)
         * lint -esym(960, 10.5)
         */
        /*lint -esym(960, 10.3) */
        /*lint -esym(960, 10.5) */
	temp = ~(uint8)(input);
        /*lint +esym(960, 10.3) */
        /*lint +esym(960, 10.5) */
	negative = TRUE;
    } else {
	temp = (uint8)(input);
	negative = FALSE;
    }

    /* now do a logical right shift */
    if( shift >= 8 ) {
	temp = 0U;
    } else {
	temp >>= shift;
    }

    /* flip back if required */
    /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
    if( negative ) {
        /**
         * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
         * Operators '~' and '<<' require recasting to underlying type for sub-integers, this is trusted code so suppress
         * lint -esym(960, 10.3)
         * lint -esym(960, 10.5)
         */
        /*lint -esym(960, 10.3) */
        /*lint -esym(960, 10.5) */
	temp = ~temp;
        /*lint +esym(960, 10.3) */
        /*lint +esym(960, 10.5) */
    }

    return (sint8)temp;
}


/******************************************************************************
 *
 * shl_xxx: saturating left shift
 *
 *****************************************************************************/


/* The following is a (rather desperate) attempt to get the maximum clip logic on
 * lsl_u32 to compile to something which doesn't have either:
 *         - a loop which shifts the constant '1' (giving a bad worst-case
 *           execution time)
 *         - or function calls to things like c_lcmp, c_land.
 *
 * There are three cases when we clip to UINT32_MAX:
 *         - shift >= 32, and input > 0
 *         - 16 >= shift >= 0, and anything in fixedpoint_u16_overflow[shift] in
 *           the top word is set
 *         - 32 > shift > 16, and any of the top word is set, or anything in
 *             fixedpoint_u16_overflow[shift-16] in the bottom word is set
 * We set overflow=TRUE in any of those three cases.
 *
 * Obviously, this is optimised for a processor with 16-bit words and limited
 * 32-bit support; however, it will still give correct results on other
 * processors.
 */

FCODE uint32 shl_u32( uint32 input, binpt_t shift )
{
    uint32 result;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    if( input == 0UL ) {
	result = input;
    } else {
	if( shift >= 32 ) {
            /** @polyspace<MISRA-C:10.6:Not a defect:No action planned> Trusted code, so suppress */
	    result=UINT32_MAX;
	} else {
	    /* split the input into top and bottom words for the overflow
	     * check. This also helps improve the worst-case execution time by
	     * limiting the maximum shift we do to 16 bits */
	    uint16 top    = (uint16)(input>>16);
	    uint16 bottom = (uint16)input;

	    if( shift >= 16 ) {
		shift -= 16;
		if ((top != 0U) || ((bottom & fixedpoint_u16_overflow[shift]) != 0U)) {
                    /** @polyspace<MISRA-C:10.6:Not a defect:No action planned> Trusted code, so suppress */
		    result=UINT32_MAX;
		} else {
		    /* for timing, note shift < 16 */
		    bottom <<= shift;
		    result = ((uint32)bottom) << 16;
		}
	    } else { /* shift < 16 */
		if((top & fixedpoint_u16_overflow[shift]) != 0U) {
                    /** @polyspace<MISRA-C:10.6:Not a defect:No action planned> Trusted code, so suppress */
		    result=UINT32_MAX;
		} else {
		    /* for timing, note shift < 16 */
		    result = input << shift;
		}
	    }
	}
    }
    return result;
}


FCODE sint32 shl_s32( sint32 input, binpt_t shift )
{
    sint32 result;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    if( input == 0L ) {
	result = input;
    } else {
	bool overflow;

	if( shift >= 31 ) {
	    overflow = TRUE;
	} else {
	    /* split the input into top and bottom words for the overflow
	     * check. */
	    uint16 top    = (uint16)(((uint32)input)>>16);
	    uint16 bottom = (uint16)input;

	    /* if the input is negative, we are more interested in zero bits
	     * than ones.
	     */
	    if ( input < 0L ) {
		top = ~top;
		bottom = ~bottom;
	    }

	    if( shift > 15 ) {
                /**
                 * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
                 * lint -esym(960, 10.1)
                 */
                /*lint -esym(960, 10.1) */
		overflow = ((top != 0U) || ((bottom & fixedpoint_u16_overflow[shift-15]) != 0U ));
                /*lint +esym(960, 10.1) */
	    } else { /* shift <= 15 */
                /**
                 * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
                 * lint -esym(960, 10.1)
                 */
                /*lint -esym(960, 10.1) */
		overflow = ((top & fixedpoint_u16_overflow[shift+1]) != 0U );
                /*lint +esym(960, 10.1) */
	    }
	}

        /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
	if( overflow ) {
	    /* overflow will occur; clip to relevant limit */
	    if( input < 0L ) {
                /**
                 * @lint Deviation Required : Prohibited operator applied to unsigned underlying type, this is trusted code so suppress
                 * lint -esym(960, 12.9)
                 */
                /*lint -esym(960, 12.9) */
		result=SINT32_MIN;
                /*lint +esym(960, 12.9) */
	    } else {
		result=SINT32_MAX;
	    }
	} else {
	    /* shift is safe; just cast to uint32 before shifting to respect
	     * MISRA 37. */

	    /* by handling the 16 <= shift < 32 case separately, we improve the
	       worst-case execution time (which then happens when shift = 15,
	       instead of when shift = 31) */

	    if ( shift >= 16 ) {
		uint16 shrt = (uint16)input;
		uint32 lng;
		shift -= 16;
		shrt <<= shift;
		lng = ((uint32)shrt) << 16;
		result = (sint32)lng;
	    } else {
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
                /** @polyspace<MISRA-C:10.3:Not a defect:No action planned> Trusted code, so suppress */
		result = (sint32)(((uint32)input) << shift );
                /*lint +esym(960, 10.3) */
	    }
	}
    }
    return result;
}


FCODE uint16 shl_u16( uint16 input, binpt_t shift )
{
    uint16 result;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    if( input == 0U ) {
	result = input;
    } else {
        /**
         * @lint Deviation Required : dependence placed on C's operator precedence, this is trusted code so suppress
         * lint -esym(961, 12.1)
         */
        /*lint -esym(961, 12.1) */
        /** @polyspace<MISRA-C:12.5:Not a defect:No action planned> Trusted code, so suppress */
	if( shift >= 16 || ((input & fixedpoint_u16_overflow[shift]) != 0U )) {
        /*lint +esym(961, 12.1) */
    	    /* overflow will occur; clip to max */
            /** @polyspace<MISRA-C:10.6:Not a defect:No action planned> Trusted code, so suppress */
	    result=UINT16_MAX;
	} else {
	    /* shift is safe */
	    result = input << shift;
	}
    }
    return result;
}


FCODE sint16 shl_s16( sint16 input, binpt_t shift )
{
    sint16 result;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    if( input == 0 ) {
	result = input;
    } else {
	bool overflow;

	/* we set overflow up to be non-zero if any bits that we are going
	 * to discard are the 'wrong' sign: ie 1 for a positive number, or
	 * 0 for a negative number.
	 */
	if( shift >= 15 ) {
	    overflow = TRUE;
	}
	else {
	    uint16 bits;
	    bits = (uint16) input;
	    if( input < 0 ) {
		bits = ~bits;
	    }
            /**
             * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
             * lint -esym(960, 10.1)
             */
            /*lint -esym(960, 10.1) */
	    overflow = ((bits & fixedpoint_u16_overflow[shift+1]) != 0U);
            /*lint -esym(960, 10.1) */
	}

        /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
	if( overflow ) {
	    /* overflow will occur; clip to relevant limit */
	    if( input < 0 ) {
		result=SINT16_MIN;
	    } else {
		result=SINT16_MAX;
	    }
	} else {
	    /* shift is safe; just cast to uint16 before shifting to respect
	     * MISRA 37. */
            /**
             * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
             * lint -esym(960, 10.3)
             */
            /*lint -esym(960, 10.3) */
            /** @polyspace<MISRA-C:10.3:Not a defect:No action planned> Trusted code, so suppress */
	    result = (sint16)(((uint16)input) << shift );
            /*lint +esym(960, 10.3) */
	}
    }
    return result;
}


FCODE uint8 shl_u8( uint8 input, binpt_t shift )
{
    uint8 result;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    if( input == 0u ) {
	result = input;
    } else {
	/* we have to cast to uint16, else input will be promoted to _signed_
	   int */
        /**
         * @lint Deviation Required : dependence placed on C's operator precedence, this is trusted code so suppress
         * lint -esym(961, 12.1)
         */
        /*lint -esym(961, 12.1) */
        /** @polyspace<MISRA-C:12.5:Not a defect:No action planned> Trusted code, so suppress */
	if( shift >= 8 || ((((uint16)input) & (fixedpoint_u16_overflow[shift+8])) != 0U )) {
        /*lint +esym(961, 12.1) */
    	    /* overflow will occur; clip to max */
	    result=UINT8_MAX;
	} else {
	    /* shift is safe */
            /**
             * @lint Deviation Required : Operators '~' and '<<' require recasting to underlying type for sub-integers, this is trusted code so suppress
             * lint -esym(960, 10.5)
             */
            /*lint -esym(960, 10.5) */
	    result = (uint8) (input << shift);
            /*lint +esym(960, 10.5) */
	}
    }
    return result;
}


FCODE sint8 shl_s8( sint8 input, binpt_t shift )
{
    sint8 result;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    if( input == 0 ) {
	result = input;
    } else {
	bool overflow;

	/* we set overflow up to be non-zero if any bits that we are going
	 * to discard are the 'wrong' sign: ie 1 for a positive number, or
	 * 0 for a negative number.
	 */
	if( shift >= 7 ) {
	    overflow = TRUE;
	}
	else { /* shift < 7 */
	    uint8 bits;
	    bits = (uint8) input;
	    if( input < 0 ) {
                /**
                 * @lint Deviation Required : Operators '~' and '<<' require recasting to underlying type for sub-integers, this is trusted code so suppress
                 * lint -esym(960, 10.5)
                 */
                /*lint -esym(960, 10.5) */
		bits = ~bits;
                /*lint +esym(960, 10.5) */
	    }
	    /* we have to cast to uint16, else bits will be promoted to _signed_
	       int */
            /**
             * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
             * lint -esym(960, 10.1)
             */
            /*lint -esym(960, 10.1) */
	    overflow = ((((uint16)bits) & (fixedpoint_u16_overflow[shift+9])) != 0U);
            /*lint +esym(960, 10.1) */
	}

        /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
	if( overflow ) {
	    /* overflow will occur; clip to relevant limit */
	    if( input < 0 ) {
		result=SINT8_MIN;
	    } else {
		result=SINT8_MAX;
	    }
	} else {
	    /* shift is safe; just cast to uint8 before shifting to respect
	     * MISRA 37. */
            /**
             * @lint Deviation Required : Operators '~' and '<<' require recasting to underlying type for sub-integers, this is trusted code so suppress
             * Prohibited cast of complex integer expression, this is trusted code so suppress
             * lint -esym(960, 10.3)
             * lint -esym(960, 10.5)
             */
            /*lint -esym(960, 10.3) */
            /*lint -esym(960, 10.5) */
            /** @polyspace<MISRA-C:10.3:Not a defect:No action planned> Trusted code, so suppress */
	    result = (sint8)(((uint8)input) << shift );
            /*lint +esym(960, 10.3) */
            /*lint +esym(960, 10.5) */
	}
    }
    return result;
}
