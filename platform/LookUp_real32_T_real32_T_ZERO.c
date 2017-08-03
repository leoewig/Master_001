/**
 * Model Name: em_ref
 *
 * Model Description:
 *
 * $Id: LookUp_real32_T_real32_T_ZERO.c 11176 2013-11-05 16:31:07Z dc17 $
 *
 * @file
 * @ingroup em_ref
 */
/**
 * @defgroup em_ref em_ref
 * @ingroup rtw
 */
/*********************************
 *         Includes              *
 *********************************/
/*********************************
 *         Defines               *
 *********************************/

/*  Data Types */

/**************************** GLOBAL DATA *************************************/
/*********************************
 *         Definitions           *
 *********************************/

/*********************************
 *         Declarations          *
 *********************************/

/***************************** FILE SCOPE DATA ********************************/

/*************************** FUNCTIONS ****************************************/
//#include "rtwtypes.h"
//#include "rtw_shared_utils.h"
#include "LookUp_real32_T_real32_T_ZERO.h"

/*********************************************************************
 * Lookup Binary Search Utility BINARYSEARCH_real32_T
 */
void BINARYSEARCH_real32_T(FPTR  uint16_t *piLeft,FPTR  uint16_t *piRght, uint32_t u,FPTR 
  const uint32_t *pData, uint16_t iHi) /* FPTR inserted */
{
  /* Find the location of current input value in the data table. */
  *piLeft = 0U;
  *piRght = iHi;
  if (u <= pData[0] ) {
    /* Less than or equal to the smallest point in the table. */
    *piRght = 0U;
  } else if (u >= pData[iHi] ) {
    /* Greater than or equal to the largest point in the table. */
    *piLeft = iHi;
  } else {
    uint16_t i;

    /* Do a binary search. */
    while (( *piRght - *piLeft ) > 1U ) {
      /* Get the average of the left and right indices using to Floor rounding. */
      i = (*piLeft + *piRght) >> 1;

      /* Move either the right index or the left index so that */
      /*  LeftDataPoint <= CurrentValue < RightDataPoint */
      if (u < pData[i] ) {
        *piRght = i;
      } else {
        *piLeft = i;
      }
    }
  }
}

/* end macro BINARYSEARCH_real32_T
 *********************************************************************/

/*********************************************************************
 * Lookup Utility LookUp_real32_T_real32_T_ZERO
 */
void LookUp_real32_T_real32_T_ZERO(FPTR  int32_t *pY,FPTR  const int32_t *pYData,
  uint32_t u,FPTR  const uint32_t *pUData, uint16_t iHi) /* FPTR inserted */
{
  uint16_t iLeft;
  uint16_t iRght;
  BINARYSEARCH_real32_T( &(iLeft), &(iRght), u, pUData, iHi);

  {
    uint32_t lambda;
    if (pUData[iRght] > pUData[iLeft] ) {
      uint32_t num;
      uint32_t den;
      den = pUData[iRght];
      den = den - pUData[iLeft];
      num = u;
      num = num - pUData[iLeft];
      lambda = num * 100u / den;
    } else {
      lambda = 0u;
    }

    {
      int32_t yLeftCast;
      int32_t yRghtCast;
      yLeftCast = pYData[iLeft];
      yRghtCast = pYData[iRght];
      yLeftCast += lambda * ( yRghtCast - yLeftCast )/100u;
      (*pY) = yLeftCast;
    }
  }
}

/* end function LookUp_real32_T_real32_T_ZERO
 *********************************************************************/

/*********************************
 *         End of File           *
 *********************************/
