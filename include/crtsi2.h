/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  crtsi2.h 
 *
 * description:  
 *  Declarations for entry points in crtsi2.s
 * 
 *  
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined (CRTSI2_H)
#define       CRTSI2_H
/**
 * @file
 * Declarations for entry points in crtsi2.s
 *
 * The compiler can't check for consistency with the assembly language so extra
 * care is needed to get the types right.
 *
 * (We tell a porky about their being INTERRUPT because the functions reset the
 * stack and interrupt flags, making it irrelevent)
 */

#include "crtsi2_types.h"

/* entry points following errors */
extern INTERRUPT void app_fatal(void);
extern INTERRUPT void app_reset(void);
extern INTERRUPT void app_copreset(void);

#endif /* CRTSI2_H */
