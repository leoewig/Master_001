/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   eq_types.h
 *
 * description:  
 *  External CAN: an interface to CANbedded
 * 
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
/**
 * @file
 * @defgroup eq_tests Equizzer Equivalent Type Definitions
 * @ingroup utils
 */

#include "basic_types.h"

#if !defined (EQ_TYPES_H)
#define       EQ_TYPES_H
typedef int8_t   sint8;               /** P12 - Redefinition of typedef to allow minimal code changes */
typedef int16_t  sint16;              /** P12 - Redefinition of typedef to allow minimal code changes */
typedef int32_t  sint32;              /** P12 - Redefinition of typedef to allow minimal code changes */

typedef uint8_t  uint8;               /** P12 - Redefinition of typedef to allow minimal code changes */
typedef uint16_t uint16;              /** P12 - Redefinition of typedef to allow minimal code changes */
typedef uint32_t uint32;              /** P12 - Redefinition of typedef to allow minimal code changes */

#if !defined(__BOOL_DEFINED)
typedef bool_t   bool; /** Visual Studio C++ doesn't like this at all - disable for unit testing */
#endif

#define SINT32_MAX INT32_MAX          /** P12 - Redefinition of typedef to allow minimal code changes */
#define SINT32_MIN (-(INT32_MAX) - 1) /** P12 - Redefinition of typedef to allow minimal code changes */
#define SINT16_MAX    INT16_MAX       /** P12 - Redefinition of typedef to allow minimal code changes */
#define SINT16_MIN (-(INT16_MAX) - 1) /** P12 - Redefinition of typedef to allow minimal code changes */
#define SINT8_MAX     INT8_MAX        /** P12 - Redefinition of typedef to allow minimal code changes */
#define SINT8_MIN  (-(INT8_MAX)  - 1) /** P12 - Redefinition of typedef to allow minimal code changes */
#endif /* EQ_TYPES_H */