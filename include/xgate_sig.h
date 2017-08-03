/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    xgate_sig.h
 *
 * description:  
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   xgate
 *                       
 *
 *****************************************************************************/
#if !defined(ABC_H)
#define      ABC_H
/**
 *
 * Summary of purpose of this file
 *
 * Additional file documentation if necessary
 *
 * @file
 * @ingroup ABC
 * $Id: xgate_sig.h 2920 2012-07-18 15:55:00Z pr4 $
 */

//Feature header files (which this probably is) only:
/**
 *
 * <h2> [Feature] Overview </h2>
 *
 * <h2> Approach used </h2>
 * An overview of the design approach.
 *
 * <h2> Something </h2>
 * Details of the design should go here, with headings as required.
 *
 * <h2> Limitations </h2>
 * Describe any limitations of the feature, if there are any.
 *
 * <h2> How to use </h2>
 * A description of exactly what the application and other features must do to 
 * use this feature.
 *
 * <h2> Configuration </h2>
 * If the feature has configuration items then point to where they are.
 *
 * <h2> Requirement Satisfaction </h2>
 * A link to the requirements document, and optionally a list of where in the  
 * design the requirements are satisfied.
 *
 * @defgroup ABC Long Name for Feature ABC
 * @ingroup DEF [parent feature name, if any]
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/**
 * Maximum of x and y
 */
#define MAX(x,y) (((x)>(y))?(x):(y))


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

typedef unsigned char foo; /**< Type used to XXX */

/**
 * Data structure for XXX
 */
struct abc_my_s abc_bar;
{
        int x;  /**< XXX */
        int y;  /**< XXX */
}

/**
 * Options for XXX
 */
typedef enum abc_options {
        Abc_this,   /**< XXX */
        Abc_that    /**< XXX */
};


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern unsigned char abc_is_x; /**< Flag used to XXX */

extern struct abc_my_s abc_bar; /**< XXX */
extern enum abc_options_e abc_options; /**< XXX */

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 *
 *  XXX (brief summary of purpose of function)
 *
 *  @param[in]      abc     - XXX (purpose, range, special values, etc.)
 *  @param[out]     rst     - XXX (purpose, range, special values, etc.)
 *  @param[in,out]  xyz     - XXX (purpose, range, special values, etc.)
 *
 Either:
 *  @return         XXX (description of what values or quantities the function
 *                  returns, possible range, special values, etc.)
 Or:
 *  @retval value-2	condition for returning value-2
 *  @retval value-2	condition for returning value-2
 *
 *  Longer description...
 */
int abc_funct( foo abc, int *rst, int *xyz );


#endif      /* ABC_H */

