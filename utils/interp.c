/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   interp.c
 *
 * description:  
 *
 * 2D interpolation lookup function
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

#include "basic_types.h"
#include "interp.h"     /* public interface */

/**
 * documentation with prototype in header file
*/
FCODE uint16 interp_1d_u16_u16 (uint16 u,
                               const FPTR uint16 *uaxis,
                               const FPTR uint16 *yaxis,
                               uint8 n_pts) {

    uint16 y = yaxis[0];

    if (u < uaxis[0]) {
        /* u off bottom of table */
        y = yaxis[0];
    } else if (u >= uaxis[n_pts-1u]) {
        /* u off top of table */
        y = yaxis[n_pts-1u];
    } else {
        /* u within table */
        uint8 i;
        bool done = FALSE;
        /* Note that this i is off by one from the one in the design */
        /**
         * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
         * lint -esym(960, 10.1)
         */
        /*lint -esym(960, 10.1) */
        /** @polyspace<MISRA-C:12.5:Not a defect:No action planned> Trusted existing code, no change planned */
        /** @polyspace<MISRA-C:12.6:Not a defect:No action planned> Construct is Misra compliant */
        /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
        for (i=1; !done && (i < n_pts); i++) {
        /*lint +esym(960, 10.1) */
            /* Valid indices into uaxis[] and yaxis[]: i-1 and i only */
            if ((u == uaxis[i]) &&
                (uaxis[i-1u] == uaxis[i])) {
                /*
                 * Handle steps in table. The result given is the y-value
                 * with the smallest possible index.
                 */
                y = yaxis[i-1u];
                done = TRUE;
            } else if (u < uaxis[i]) {
                /*                                 (   u - u     )
                 *                                 (        i-1  )
                 * Calculate y = y    + (y  - y   )(-------------)
                 *                i-1     i    i-1 (  u  - u     )
                 *                                 (   i    i-1  )
                 *
                 * or y = y_(i-1) + (Dy)(du/Du)
                 *
                 * Division performed last in order to minimise error in
                 * calculation: y_(i-1) + ((Dy)(du))/Du
                 *
                 * Dy stored as separate sign and value so that later
                 * arithmetic can be unsigned and have defined rounding
                 * characteristics.
                 */
                uint16 ydiff;   /* "|Dy|" */
                bool ydneg;     /* "sign(Dy)" */
                uint32 numer;   /* numerator "(|Dy|*du)" */
                uint16 frac;    /* product term "(|Dy|*du)/Du" */
                if (yaxis[i] >= yaxis[i-1u]) {
                    ydneg = FALSE;
                    ydiff = yaxis[i] - yaxis[i-1u];
                } else {
                    ydneg = TRUE;
                    ydiff = yaxis[i-1u] - yaxis[i];
                }
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
                /** @polyspace<MISRA-C:10.3:Low:Investigate> Could code be changed to fix this? */
                /* This generates EMUL (u16*u16->u32) instruction */
                numer = (uint32) ydiff * (uint32)(u - uaxis[i-1u]);
                /*lint +esym(960, 10.3) */

                /* u32/u16 division. Cast to uint16 causes compiler to
                 * generate EDIV instruction. */
                /* (We know that frac will fit in a uint16) */
                /**
                 * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
                 * lint -esym(960, 10.1)
                 */
                /*lint -esym(960, 10.1) */
                frac = (uint16)(numer / (uaxis[i] - uaxis[i-1u]));
                /*lint +esym(960, 10.1) */
                /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
                if (ydneg) {
                    y = yaxis[i-1u] - frac;
                } else {
                    y = yaxis[i-1u] + frac;
                }
                done = TRUE;
            } else {
		 /* still stepping through table. */
                ;
            }
        }
    }

    return y;
}

/**
 * documentation with prototype in header file
*/
FCODE sint16 interp_1d_s16_s16 (sint16 u,
                               const FPTR sint16 *uaxis,
                               const FPTR sint16 *yaxis,
                               uint8 n_pts) {

    sint16 y = yaxis[0];

    if (u < uaxis[0]) {
        /* u off bottom of table */
        y = yaxis[0];
    } else if (u >= uaxis[n_pts-1u]) {
        /* u off top of table */
        y = yaxis[n_pts-1u];
    } else {
        /* u within table */
        uint8 i;
        bool done = FALSE;

        /* Note that this i is off by one from the one in the design */
        /**
         * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
         * lint -esym(960, 10.1)
         */
        /*lint -esym(960, 10.1) */
        /** @polyspace<MISRA-C:12.5:Not a defect:No action planned> Trusted existing code, no change planned */
        /** @polyspace<MISRA-C:12.6:Not a defect:No action planned> Construct is Misra compliant */
        /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
        for (i=1; !done && (i < n_pts); i++) {
        /*lint +esym(960, 10.1) */
            /* Valid indices into uaxis[] and yaxis[]: i-1 and i only */
            if ((u == uaxis[i]) &&
                (uaxis[i-1u] == uaxis[i])) {
                /*
                 * Handle steps in table. The result given is the y-value
                 * with the smallest possible index.
                 */
                y = yaxis[i-1u];
                done = TRUE;
            } else if (u < uaxis[i]) {
                /*                                 (   u - u     )
                 *                                 (        i-1  )
                 * Calculate y = y    + (y  - y   )(-------------)
                 *                i-1     i    i-1 (  u  - u     )
                 *                                 (   i    i-1  )
                 *
                 * or y = y_(i-1) + (Dy)(du/Du)
                 *
                 * Division performed last in order to minimise error in
                 * calculation: y_(i-1) + ((Dy)(du))/Du
                 *
                 * Dy stored as separate sign because the range of Dy exceeds
                 * that of a sint16. However, the _absolute_ difference between
                 * two sint16s can be represented in a uint16.
                 */
                uint16 ydiff;   /* "|Dy|" */
                bool ydneg;     /* "sign(Dy)" */
                uint32 numer;   /* numerator "(|Dy|*du)" */
                uint16 frac;    /* product term "(|Dy|*du)/Du" */
		uint16 du, Du;

		if (yaxis[i] >= yaxis[i-1u]) {
                    ydneg = FALSE;
                    /**
                     * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                     * lint -esym(960, 10.3)
                     */
                    /*lint -esym(960, 10.3) */
                    /** @polyspace<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted existing code. no change planned */
                    ydiff = (uint16)(yaxis[i] - yaxis[i-1u]);
                    /*lint +esym(960, 10.3) */
                } else {
                    ydneg = TRUE;
                    /**
                     * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                     * lint -esym(960, 10.3)
                     */
                    /*lint -esym(960, 10.3) */
                    /** @polyspace<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted existing code. no change planned */
                    ydiff = (uint16)(yaxis[i-1u] - yaxis[i]);
                    /*lint +esym(960, 10.3) */
                }

		/* We know that uaxis[i] > u > uaxis[i-1u] so there's no problem
		 * with casting these to unsigned. */
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
                /** @polyspace<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted existing code. no change planned */
		du = (uint16)(u-uaxis[i-1u]);
                /*lint +esym(960, 10.3) */
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
                /** @polyspace<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted existing code. no change planned */
		Du = (uint16)(uaxis[i] - uaxis[i-1u]);
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
                /** @polyspace<MISRA-C:10.3:Low:Investigate> Could code be changed to fix this? */
                /* This generates EMUL (u16*u16->u32) instruction */
                numer = (uint32) ydiff * (uint32)du;
                /*lint +esym(960, 10.3) */

                /* u32/u16 division. Cast to uint16 causes compiler to
                 * generate EDIV instruction. */
                /* (We know that frac will fit in a uint16) */
                /**
                 * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
                 * lint -esym(960, 10.1)
                 */
                /*lint -esym(960, 10.1) */
                frac = (uint16)(numer / Du);

                /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
		if (ydneg) {
		    /* If Dy was negative, we know that
		       yaxis[i] < yaxis[i-1u]-frac < yaxis[i-1u], so
		       yaxis[i-1u]-frac will fit into a sint16. */
                    y = yaxis[i-1u] - (sint16)frac;
                } else {
		    /* If Dy was positive, we know that
		       yaxis[i] > yaxis[i-1u]+frac > yaxis[i-1u], so
		       yaxis[i-1u]+frac will fit into a sint16. */
                    y = yaxis[i-1u] + (sint16)frac;
                }
                done = TRUE;
            } else {
                /* else clause can never be reached; no appropriate
                 * action to take */
                ;
            }
        }
    }

    return y;
}

/**
 * documentation with prototype in header file
*/
FCODE sint16 interp_1d_u16_s16 (uint16 u,
                               const FPTR uint16 *uaxis,
                               const FPTR sint16 *yaxis,
                               uint8 n_pts) {

    sint16 y = yaxis[0];

    if (u < uaxis[0]) {
        /* u off bottom of table */
        y = yaxis[0];
    } else if (u >= uaxis[n_pts-1u]) {
        /* u off top of table */
        y = yaxis[n_pts-1u];
    } else {
        /* u within table */
        uint8 i;
        bool done = FALSE;

        /* Note that this i is off by one from the one in the design */
        /**
         * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
         * lint -esym(960, 10.1)
         */
        /*lint -esym(960, 10.1) */
        /** @polyspace<MISRA-C:12.5:Not a defect:No action planned> Trusted existing code, no change planned */
        /** @polyspace<MISRA-C:12.6:Not a defect:No action planned> Construct is Misra compliant */
        /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
        for (i=1; !done && (i < n_pts); i++) {
        /*lint +esym(960, 10.1) */
            /* Valid indices into uaxis[] and yaxis[]: i-1 and i only */
            if ((u == uaxis[i]) &&
                (uaxis[i-1u] == uaxis[i])) {
                /*
                 * Handle steps in table. The result given is the y-value
                 * with the smallest possible index.
                 */
                y = yaxis[i-1u];
                done = TRUE;
            } else if (u < uaxis[i]) {
                /*                                 (   u - u     )
                 *                                 (        i-1  )
                 * Calculate y = y    + (y  - y   )(-------------)
                 *                i-1     i    i-1 (  u  - u     )
                 *                                 (   i    i-1  )
                 *
                 * or y = y_(i-1) + (Dy)(du/Du)
                 *
                 * Division performed last in order to minimise error in
                 * calculation: y_(i-1) + ((Dy)(du))/Du
                 *
                 * Dy stored as separate sign because the range of Dy exceeds
                 * that of a sint16. However, the _absolute_ difference between
                 * two sint16s can be represented in a uint16.
                 */
                uint16 ydiff;   /* "|Dy|" */
                bool ydneg;     /* "sign(Dy)" */
                uint32 numer;   /* numerator "(|Dy|*du)" */
                uint16 frac;    /* product term "(|Dy|*du)/Du" */
		uint16 du, Du;

		if (yaxis[i] >= yaxis[i-1u]) {
                    ydneg = FALSE;
                    /**
                     * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                     * lint -esym(960, 10.3)
                     */
                    /*lint -esym(960, 10.3) */
                    /** @polyspace<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted existing code. no change planned */
                    ydiff = (uint16)(yaxis[i] - yaxis[i-1u]);
                    /*lint +esym(960, 10.3) */
                } else {
                    ydneg = TRUE;
                    /**
                     * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                     * lint -esym(960, 10.3)
                     */
                    /*lint -esym(960, 10.3) */
                    /** @polyspace<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted existing code. no change planned */
                    ydiff = (uint16)(yaxis[i-1u] - yaxis[i]);
                    /*lint +esym(960, 10.3) */
                }

		/* We know that uaxis[i] > u > uaxis[i-1u] so there's no problem
		 * with casting these to unsigned. */
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
		du = (uint16)(u-uaxis[i-1u]);
                /*lint +esym(960, 10.3) */
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
		Du = (uint16)(uaxis[i] - uaxis[i-1u]);
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
                /** @polyspace<MISRA-C:10.3:Low:Investigate> Could code be changed to fix this? */
                /* This generates EMUL (u16*u16->u32) instruction */
                numer = (uint32) ydiff * (uint32)du;
                /*lint +esym(960, 10.3) */

                /* u32/u16 division. Cast to uint16 causes compiler to
                 * generate EDIV instruction. */
                /* (We know that frac will fit in a uint16) */
                /**
                 * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
                 * lint -esym(960, 10.1)
                 */
                /*lint -esym(960, 10.1) */
                frac = (uint16)(numer / Du);

                /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
		if (ydneg) {
		    /* If Dy was negative, we know that
		       yaxis[i] < yaxis[i-1u]-frac < yaxis[i-1u], so
		       yaxis[i-1u]-frac will fit into a sint16. */
                    y = yaxis[i-1u] - (sint16)frac;
                } else {
		    /* If Dy was positive, we know that
		       yaxis[i] > yaxis[i-1u]+frac > yaxis[i-1u], so
		       yaxis[i-1u]+frac will fit into a sint16. */
                    y = yaxis[i-1u] + (sint16)frac;
                }
                done = TRUE;
            } else {
                /* else clause can never be reached; no appropriate
                 * action to take */
                ;
            }
        }
    }

    return y;
}
