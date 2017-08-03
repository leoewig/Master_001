/**
 * @file
 * @ingroup regs
 *
 * File to locate registers in memory by including COSMIC header file
 * ioxep100.h or hostedxep100.h on the host environment.
 *
 * @par Definitions:
 * \#if !defined(_lint) && !defined(POLYSPACE)
 * @par
 * \#if !defined(HOSTED)
 *
 * Lint and Polyspace builds will not include ioxep100.h to avoid issues
 * with the absolute location (\@) syntax. The host environment will include
 * hostedxep100.h since the register definitions are still needed to build the
 * unit test solution.
 */
/* Copyright (c) Linkedriving Tech 2012 */
#if (!defined(_lint)) && (!defined(POLYSPACE))
#include "basic_types.h"
#include "regs.h"

#if !defined(HOSTED)
#include "ioxep100.h"
#else
#include "hostedxep100.h"
#endif

#endif



