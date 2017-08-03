/*
 * File: LDT_TMM_Model.h
 *
 * Code generated for Simulink model 'LDT_TMM_Model'.
 *
 * Model version                  : 1.12
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Wed Jun 29 19:07:38 2016
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->16-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_LDT_TMM_Model_h_
#define RTW_HEADER_LDT_TMM_Model_h_
#include <math.h>
#include <stddef.h>
#include <string.h>
#ifndef LDT_TMM_Model_COMMON_INCLUDES_
# define LDT_TMM_Model_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* LDT_TMM_Model_COMMON_INCLUDES_ */

#include "LDT_TMM_Model_types.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  boolean_T mdm_out_Flag_TMM_Control_b;/* '<Root>/Data Store Memory' */
} DW_LDT_TMM_Model_T;

/* Invariant block signals (auto storage) */
typedef struct {
  const real_T TMM_Matrix_10[3];       /* '<S3>/TMM_Matrix_1[0]' */
  const real_T TMM_Matrix_11[3];       /* '<S3>/TMM_Matrix_1[1]' */
  const real_T TMM_Matrix_12[3];       /* '<S3>/TMM_Matrix_1[2]' */
  const real_T TMM_Matrix_13[3];       /* '<S3>/TMM_Matrix_1[3]' */
  const real_T TMM_Matrix_14[3];       /* '<S3>/TMM_Matrix_1[4]' */
  const real_T TMM_Matrix_15[3];       /* '<S3>/TMM_Matrix_1[5]' */
  const real_T TMM_Matrix_16[3];       /* '<S3>/TMM_Matrix_1[6]' */
  const real_T TMM_Matrix_17[3];       /* '<S3>/TMM_Matrix_1[7]' */
  const real_T TMM_Matrix_18[3];       /* '<S3>/TMM_Matrix_1[8]' */
  const real_T TMM_Matrix_19[3];       /* '<S3>/TMM_Matrix_1[9]' */
  const uint16_T Product2;             /* '<S4>/Product2' */
  const uint16_T Product2_k;           /* '<S5>/Product2' */
  const uint16_T Product2_b;           /* '<S6>/Product2' */
  const uint16_T Product2_d;           /* '<S7>/Product2' */
  const uint16_T Product2_o;           /* '<S8>/Product2' */
  const uint16_T Product2_e;           /* '<S9>/Product2' */
  const uint16_T Product2_g;           /* '<S10>/Product2' */
  const uint16_T Product2_c;           /* '<S11>/Product2' */
  const uint16_T Product2_l;           /* '<S12>/Product2' */
  const uint16_T Product2_i;           /* '<S13>/Product2' */
} ConstB_LDT_TMM_Model_T;

/* Constant parameters (auto storage) */
typedef struct {
  /* Expression: [0.0,85,100;0.5,0,100;2.5,255,180;6.0,235,100;7.0,215,100;8.0,195,100;9.0,175,100;10.0,160,100;11.0,85,100;15.0,85,100]
   * Referenced by: '<S3>/Constant'
   */
  real_T Constant_Value[30];
} ConstP_LDT_TMM_Model_T;

/* Real-time Model Data Structure */
struct tag_RTM_LDT_TMM_Model_T {
  const char_T * volatile errorStatus;
};

/* Block states (auto storage) */
extern DW_LDT_TMM_Model_T LDT_TMM_Model_DW;
extern const ConstB_LDT_TMM_Model_T LDT_TMM_Model_ConstB;/* constant block i/o */

/* Constant parameters (auto storage) */
extern const ConstP_LDT_TMM_Model_T LDT_TMM_Model_ConstP;

/* Model entry point functions */
extern void LDT_TMM_Model_initialize(void);
extern void LDT_TMM_Model_step(void);
extern void LDT_TMM_Model_terminate(void);

/* Exported data declaration */

/* Declaration for custom storage class: Default */
extern real_T TMM_Matrix_1[30];
extern uint8_T mdm_in_TMM_Matrix_Num;
extern uint16_T mdm_in_TMM_Run_Cnt;
extern boolean_T mdm_out_Flag_TMM_Control;
extern int16_T mdm_out_TMM_Angle_Tar;
extern uint16_T mdm_out_TMM_Speed_Tar;

/* Real-time Model object */
extern RT_MODEL_LDT_TMM_Model_T *const LDT_TMM_Model_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'LDT_TMM_Model'
 * '<S1>'   : 'LDT_TMM_Model/Switch Case Action Subsystem'
 * '<S2>'   : 'LDT_TMM_Model/Switch Case Action Subsystem1'
 * '<S3>'   : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1'
 * '<S4>'   : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1/Subsystem'
 * '<S5>'   : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1/Subsystem1'
 * '<S6>'   : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1/Subsystem2'
 * '<S7>'   : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1/Subsystem3'
 * '<S8>'   : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1/Subsystem4'
 * '<S9>'   : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1/Subsystem5'
 * '<S10>'  : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1/Subsystem6'
 * '<S11>'  : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1/Subsystem7'
 * '<S12>'  : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1/Subsystem8'
 * '<S13>'  : 'LDT_TMM_Model/Switch Case Action Subsystem/Matrix 1/Subsystem9'
 */
#endif                                 /* RTW_HEADER_LDT_TMM_Model_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
