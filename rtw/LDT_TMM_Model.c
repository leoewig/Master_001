/*
 * File: LDT_TMM_Model.c
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

#include "LDT_TMM_Model.h"
#include "LDT_TMM_Model_private.h"

/* Exported data definition */

/* Definition for custom storage class: Default */
real_T TMM_Matrix_1[30];
uint8_T mdm_in_TMM_Matrix_Num;
uint16_T mdm_in_TMM_Run_Cnt;
boolean_T mdm_out_Flag_TMM_Control;
int16_T mdm_out_TMM_Angle_Tar;
uint16_T mdm_out_TMM_Speed_Tar;

/* Block states (auto storage) */
DW_LDT_TMM_Model_T LDT_TMM_Model_DW;

/* Real-time model */
RT_MODEL_LDT_TMM_Model_T LDT_TMM_Model_M_;
RT_MODEL_LDT_TMM_Model_T *const LDT_TMM_Model_M = &LDT_TMM_Model_M_;

/* Model step function */
void LDT_TMM_Model_step(void)
{
  boolean_T rtb_RelationalOperator;
  boolean_T rtb_RelationalOperator_ju;
  boolean_T rtb_RelationalOperator_a;
  boolean_T rtb_RelationalOperator_m;
  boolean_T rtb_RelationalOperator_j;
  boolean_T rtb_RelationalOperator_k;
  boolean_T rtb_RelationalOperator_fq;
  boolean_T rtb_RelationalOperator_p;
  boolean_T rtb_RelationalOperator_g2;
  boolean_T rtb_RelationalOperator_n;
  real_T tmp;
  real_T tmp_0;
  real_T tmp_1;
  real_T tmp_2;
  real_T tmp_3;
  real_T tmp_4;
  real_T tmp_5;
  real_T tmp_6;
  real_T tmp_7;
  real_T tmp_8;

  /* SwitchCase: '<Root>/Switch Case' incorporates:
   *  Inport: '<Root>/In2'
   */
  if (mdm_in_TMM_Matrix_Num == 1) {
    /* Outputs for IfAction SubSystem: '<Root>/Switch Case Action Subsystem' incorporates:
     *  ActionPort: '<S1>/Action Port'
     */
    /* RelationalOperator: '<S4>/Relational Operator' incorporates:
     *  Inport: '<Root>/In1'
     */
    rtb_RelationalOperator = (mdm_in_TMM_Run_Cnt ==
      LDT_TMM_Model_ConstB.Product2);

    /* RelationalOperator: '<S5>/Relational Operator' incorporates:
     *  Inport: '<Root>/In1'
     */
    rtb_RelationalOperator_ju = (mdm_in_TMM_Run_Cnt ==
      LDT_TMM_Model_ConstB.Product2_k);

    /* RelationalOperator: '<S6>/Relational Operator' incorporates:
     *  Inport: '<Root>/In1'
     */
    rtb_RelationalOperator_a = (mdm_in_TMM_Run_Cnt ==
      LDT_TMM_Model_ConstB.Product2_b);

    /* RelationalOperator: '<S7>/Relational Operator' incorporates:
     *  Inport: '<Root>/In1'
     */
    rtb_RelationalOperator_m = (mdm_in_TMM_Run_Cnt ==
      LDT_TMM_Model_ConstB.Product2_d);

    /* RelationalOperator: '<S8>/Relational Operator' incorporates:
     *  Inport: '<Root>/In1'
     */
    rtb_RelationalOperator_j = (mdm_in_TMM_Run_Cnt ==
      LDT_TMM_Model_ConstB.Product2_o);

    /* RelationalOperator: '<S9>/Relational Operator' incorporates:
     *  Inport: '<Root>/In1'
     */
    rtb_RelationalOperator_k = (mdm_in_TMM_Run_Cnt ==
      LDT_TMM_Model_ConstB.Product2_e);

    /* RelationalOperator: '<S10>/Relational Operator' incorporates:
     *  Inport: '<Root>/In1'
     */
    rtb_RelationalOperator_fq = (mdm_in_TMM_Run_Cnt ==
      LDT_TMM_Model_ConstB.Product2_g);

    /* RelationalOperator: '<S11>/Relational Operator' incorporates:
     *  Inport: '<Root>/In1'
     */
    rtb_RelationalOperator_p = (mdm_in_TMM_Run_Cnt ==
      LDT_TMM_Model_ConstB.Product2_c);

    /* RelationalOperator: '<S12>/Relational Operator' incorporates:
     *  Inport: '<Root>/In1'
     */
    rtb_RelationalOperator_g2 = (mdm_in_TMM_Run_Cnt ==
      LDT_TMM_Model_ConstB.Product2_l);

    /* RelationalOperator: '<S13>/Relational Operator' incorporates:
     *  Inport: '<Root>/In1'
     */
    rtb_RelationalOperator_n = (mdm_in_TMM_Run_Cnt ==
      LDT_TMM_Model_ConstB.Product2_i);

    /* Product: '<S4>/Product1' */
    tmp = floor((real_T)rtb_RelationalOperator *
                LDT_TMM_Model_ConstB.TMM_Matrix_10[2]);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 65536.0);
    }

    /* Product: '<S5>/Product1' */
    tmp_0 = floor((real_T)rtb_RelationalOperator_ju *
                  LDT_TMM_Model_ConstB.TMM_Matrix_11[2]);
    if (rtIsNaN(tmp_0) || rtIsInf(tmp_0)) {
      tmp_0 = 0.0;
    } else {
      tmp_0 = fmod(tmp_0, 65536.0);
    }

    /* Product: '<S6>/Product1' */
    tmp_1 = floor((real_T)rtb_RelationalOperator_a *
                  LDT_TMM_Model_ConstB.TMM_Matrix_12[2]);
    if (rtIsNaN(tmp_1) || rtIsInf(tmp_1)) {
      tmp_1 = 0.0;
    } else {
      tmp_1 = fmod(tmp_1, 65536.0);
    }

    /* Product: '<S7>/Product1' */
    tmp_2 = floor((real_T)rtb_RelationalOperator_m *
                  LDT_TMM_Model_ConstB.TMM_Matrix_13[2]);
    if (rtIsNaN(tmp_2) || rtIsInf(tmp_2)) {
      tmp_2 = 0.0;
    } else {
      tmp_2 = fmod(tmp_2, 65536.0);
    }

    /* Product: '<S8>/Product1' */
    tmp_3 = floor((real_T)rtb_RelationalOperator_j *
                  LDT_TMM_Model_ConstB.TMM_Matrix_14[2]);
    if (rtIsNaN(tmp_3) || rtIsInf(tmp_3)) {
      tmp_3 = 0.0;
    } else {
      tmp_3 = fmod(tmp_3, 65536.0);
    }

    /* Product: '<S9>/Product1' */
    tmp_4 = floor((real_T)rtb_RelationalOperator_k *
                  LDT_TMM_Model_ConstB.TMM_Matrix_15[2]);
    if (rtIsNaN(tmp_4) || rtIsInf(tmp_4)) {
      tmp_4 = 0.0;
    } else {
      tmp_4 = fmod(tmp_4, 65536.0);
    }

    /* Product: '<S10>/Product1' */
    tmp_5 = floor((real_T)rtb_RelationalOperator_fq *
                  LDT_TMM_Model_ConstB.TMM_Matrix_16[2]);
    if (rtIsNaN(tmp_5) || rtIsInf(tmp_5)) {
      tmp_5 = 0.0;
    } else {
      tmp_5 = fmod(tmp_5, 65536.0);
    }

    /* Product: '<S11>/Product1' */
    tmp_6 = floor((real_T)rtb_RelationalOperator_p *
                  LDT_TMM_Model_ConstB.TMM_Matrix_17[2]);
    if (rtIsNaN(tmp_6) || rtIsInf(tmp_6)) {
      tmp_6 = 0.0;
    } else {
      tmp_6 = fmod(tmp_6, 65536.0);
    }

    /* Product: '<S12>/Product1' */
    tmp_7 = floor((real_T)rtb_RelationalOperator_g2 *
                  LDT_TMM_Model_ConstB.TMM_Matrix_18[2]);
    if (rtIsNaN(tmp_7) || rtIsInf(tmp_7)) {
      tmp_7 = 0.0;
    } else {
      tmp_7 = fmod(tmp_7, 65536.0);
    }

    /* Product: '<S13>/Product1' */
    tmp_8 = floor((real_T)rtb_RelationalOperator_n *
                  LDT_TMM_Model_ConstB.TMM_Matrix_19[2]);
    if (rtIsNaN(tmp_8) || rtIsInf(tmp_8)) {
      tmp_8 = 0.0;
    } else {
      tmp_8 = fmod(tmp_8, 65536.0);
    }

    /* Sum: '<S3>/Sum1' incorporates:
     *  Product: '<S10>/Product1'
     *  Product: '<S11>/Product1'
     *  Product: '<S12>/Product1'
     *  Product: '<S13>/Product1'
     *  Product: '<S4>/Product1'
     *  Product: '<S5>/Product1'
     *  Product: '<S6>/Product1'
     *  Product: '<S7>/Product1'
     *  Product: '<S8>/Product1'
     *  Product: '<S9>/Product1'
     */
    mdm_out_TMM_Speed_Tar = (((((((((tmp < 0.0 ? (uint16_T)-(int16_T)(uint16_T)
      -tmp : (uint16_T)tmp) + (tmp_0 < 0.0 ? (uint16_T)-(int16_T)(uint16_T)
      -tmp_0 : (uint16_T)tmp_0)) + (tmp_1 < 0.0 ? (uint16_T)-(int16_T)(uint16_T)
      -tmp_1 : (uint16_T)tmp_1)) + (tmp_2 < 0.0 ? (uint16_T)-(int16_T)(uint16_T)
      -tmp_2 : (uint16_T)tmp_2)) + (tmp_3 < 0.0 ? (uint16_T)-(int16_T)(uint16_T)
      -tmp_3 : (uint16_T)tmp_3)) + (tmp_4 < 0.0 ? (uint16_T)-(int16_T)(uint16_T)
      -tmp_4 : (uint16_T)tmp_4)) + (tmp_5 < 0.0 ? (uint16_T)-(int16_T)(uint16_T)
      -tmp_5 : (uint16_T)tmp_5)) + (tmp_6 < 0.0 ? (uint16_T)-(int16_T)(uint16_T)
      -tmp_6 : (uint16_T)tmp_6)) + (tmp_7 < 0.0 ? (uint16_T)-(int16_T)(uint16_T)
      -tmp_7 : (uint16_T)tmp_7)) + (tmp_8 < 0.0 ? (uint16_T)-(int16_T)(uint16_T)
      -tmp_8 : (uint16_T)tmp_8);

    /* Switch: '<S1>/Switch' incorporates:
     *  DataStoreWrite: '<S1>/Data Store Write'
     */
    LDT_TMM_Model_DW.mdm_out_Flag_TMM_Control_b = (mdm_out_TMM_Speed_Tar > 0U);

    /* Product: '<S4>/Product' */
    tmp = floor((real_T)rtb_RelationalOperator *
                LDT_TMM_Model_ConstB.TMM_Matrix_10[1]);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 65536.0);
    }

    /* Product: '<S5>/Product' */
    tmp_0 = floor((real_T)rtb_RelationalOperator_ju *
                  LDT_TMM_Model_ConstB.TMM_Matrix_11[1]);
    if (rtIsNaN(tmp_0) || rtIsInf(tmp_0)) {
      tmp_0 = 0.0;
    } else {
      tmp_0 = fmod(tmp_0, 65536.0);
    }

    /* Product: '<S6>/Product' */
    tmp_1 = floor((real_T)rtb_RelationalOperator_a *
                  LDT_TMM_Model_ConstB.TMM_Matrix_12[1]);
    if (rtIsNaN(tmp_1) || rtIsInf(tmp_1)) {
      tmp_1 = 0.0;
    } else {
      tmp_1 = fmod(tmp_1, 65536.0);
    }

    /* Product: '<S7>/Product' */
    tmp_2 = floor((real_T)rtb_RelationalOperator_m *
                  LDT_TMM_Model_ConstB.TMM_Matrix_13[1]);
    if (rtIsNaN(tmp_2) || rtIsInf(tmp_2)) {
      tmp_2 = 0.0;
    } else {
      tmp_2 = fmod(tmp_2, 65536.0);
    }

    /* Product: '<S8>/Product' */
    tmp_3 = floor((real_T)rtb_RelationalOperator_j *
                  LDT_TMM_Model_ConstB.TMM_Matrix_14[1]);
    if (rtIsNaN(tmp_3) || rtIsInf(tmp_3)) {
      tmp_3 = 0.0;
    } else {
      tmp_3 = fmod(tmp_3, 65536.0);
    }

    /* Product: '<S9>/Product' */
    tmp_4 = floor((real_T)rtb_RelationalOperator_k *
                  LDT_TMM_Model_ConstB.TMM_Matrix_15[1]);
    if (rtIsNaN(tmp_4) || rtIsInf(tmp_4)) {
      tmp_4 = 0.0;
    } else {
      tmp_4 = fmod(tmp_4, 65536.0);
    }

    /* Product: '<S10>/Product' */
    tmp_5 = floor((real_T)rtb_RelationalOperator_fq *
                  LDT_TMM_Model_ConstB.TMM_Matrix_16[1]);
    if (rtIsNaN(tmp_5) || rtIsInf(tmp_5)) {
      tmp_5 = 0.0;
    } else {
      tmp_5 = fmod(tmp_5, 65536.0);
    }

    /* Product: '<S11>/Product' */
    tmp_6 = floor((real_T)rtb_RelationalOperator_p *
                  LDT_TMM_Model_ConstB.TMM_Matrix_17[1]);
    if (rtIsNaN(tmp_6) || rtIsInf(tmp_6)) {
      tmp_6 = 0.0;
    } else {
      tmp_6 = fmod(tmp_6, 65536.0);
    }

    /* Product: '<S12>/Product' */
    tmp_7 = floor((real_T)rtb_RelationalOperator_g2 *
                  LDT_TMM_Model_ConstB.TMM_Matrix_18[1]);
    if (rtIsNaN(tmp_7) || rtIsInf(tmp_7)) {
      tmp_7 = 0.0;
    } else {
      tmp_7 = fmod(tmp_7, 65536.0);
    }

    /* Product: '<S13>/Product' */
    tmp_8 = floor((real_T)rtb_RelationalOperator_n *
                  LDT_TMM_Model_ConstB.TMM_Matrix_19[1]);
    if (rtIsNaN(tmp_8) || rtIsInf(tmp_8)) {
      tmp_8 = 0.0;
    } else {
      tmp_8 = fmod(tmp_8, 65536.0);
    }

    /* Sum: '<S3>/Sum' incorporates:
     *  Product: '<S10>/Product'
     *  Product: '<S11>/Product'
     *  Product: '<S12>/Product'
     *  Product: '<S13>/Product'
     *  Product: '<S4>/Product'
     *  Product: '<S5>/Product'
     *  Product: '<S6>/Product'
     *  Product: '<S7>/Product'
     *  Product: '<S8>/Product'
     *  Product: '<S9>/Product'
     */
    mdm_out_TMM_Angle_Tar = (((((((((tmp < 0.0 ? -(int16_T)(uint16_T)-tmp :
      (int16_T)(uint16_T)tmp) + (tmp_0 < 0.0 ? -(int16_T)(uint16_T)-tmp_0 :
      (int16_T)(uint16_T)tmp_0)) + (tmp_1 < 0.0 ? -(int16_T)(uint16_T)-tmp_1 :
      (int16_T)(uint16_T)tmp_1)) + (tmp_2 < 0.0 ? -(int16_T)(uint16_T)-tmp_2 :
      (int16_T)(uint16_T)tmp_2)) + (tmp_3 < 0.0 ? -(int16_T)(uint16_T)-tmp_3 :
      (int16_T)(uint16_T)tmp_3)) + (tmp_4 < 0.0 ? -(int16_T)(uint16_T)-tmp_4 :
      (int16_T)(uint16_T)tmp_4)) + (tmp_5 < 0.0 ? -(int16_T)(uint16_T)-tmp_5 :
      (int16_T)(uint16_T)tmp_5)) + (tmp_6 < 0.0 ? -(int16_T)(uint16_T)-tmp_6 :
      (int16_T)(uint16_T)tmp_6)) + (tmp_7 < 0.0 ? -(int16_T)(uint16_T)-tmp_7 :
      (int16_T)(uint16_T)tmp_7)) + (tmp_8 < 0.0 ? -(int16_T)(uint16_T)-tmp_8 :
      (int16_T)(uint16_T)tmp_8);

    /* End of Outputs for SubSystem: '<Root>/Switch Case Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<Root>/Switch Case Action Subsystem1' incorporates:
     *  ActionPort: '<S2>/Action Port'
     */
    /* DataStoreWrite: '<S2>/Data Store Write' incorporates:
     *  Constant: '<S2>/Constant'
     */
    LDT_TMM_Model_DW.mdm_out_Flag_TMM_Control_b = false;

    /* End of Outputs for SubSystem: '<Root>/Switch Case Action Subsystem1' */
  }

  /* End of SwitchCase: '<Root>/Switch Case' */

  /* DataStoreRead: '<Root>/Data Store Read' */
  mdm_out_Flag_TMM_Control = LDT_TMM_Model_DW.mdm_out_Flag_TMM_Control_b;
}

/* Model initialize function */
void LDT_TMM_Model_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize error status */
  rtmSetErrorStatus(LDT_TMM_Model_M, (NULL));

  /* block I/O */

  /* custom signals */
  TMM_Matrix_1[0] = 0.0;
  TMM_Matrix_1[1] = 0.5;
  TMM_Matrix_1[2] = 2.5;
  TMM_Matrix_1[3] = 6.0;
  TMM_Matrix_1[4] = 7.0;
  TMM_Matrix_1[5] = 8.0;
  TMM_Matrix_1[6] = 9.0;
  TMM_Matrix_1[7] = 10.0;
  TMM_Matrix_1[8] = 11.0;
  TMM_Matrix_1[9] = 15.0;
  TMM_Matrix_1[10] = 85.0;
  TMM_Matrix_1[11] = 0.0;
  TMM_Matrix_1[12] = 255.0;
  TMM_Matrix_1[13] = 235.0;
  TMM_Matrix_1[14] = 215.0;
  TMM_Matrix_1[15] = 195.0;
  TMM_Matrix_1[16] = 175.0;
  TMM_Matrix_1[17] = 160.0;
  TMM_Matrix_1[18] = 85.0;
  TMM_Matrix_1[19] = 85.0;
  TMM_Matrix_1[20] = 100.0;
  TMM_Matrix_1[21] = 100.0;
  TMM_Matrix_1[22] = 180.0;
  TMM_Matrix_1[23] = 100.0;
  TMM_Matrix_1[24] = 100.0;
  TMM_Matrix_1[25] = 100.0;
  TMM_Matrix_1[26] = 100.0;
  TMM_Matrix_1[27] = 100.0;
  TMM_Matrix_1[28] = 100.0;
  TMM_Matrix_1[29] = 100.0;
  mdm_out_TMM_Angle_Tar = 0;
  mdm_out_TMM_Speed_Tar = 0U;
  mdm_out_Flag_TMM_Control = false;

  /* states (dwork) */
  (void) memset((void *)&LDT_TMM_Model_DW, 0,
                sizeof(DW_LDT_TMM_Model_T));

  /* external inputs */
  mdm_in_TMM_Matrix_Num = 0U;
  mdm_in_TMM_Run_Cnt = 0U;

  {
    int16_T i;

    /* ConstCode for IfAction SubSystem: '<Root>/Switch Case Action Subsystem' */
    /* ConstCode for Constant: '<S3>/Constant' */
    for (i = 0; i < 30; i++) {
      TMM_Matrix_1[i] = LDT_TMM_Model_ConstP.Constant_Value[i];
    }

    /* End of ConstCode for Constant: '<S3>/Constant' */
    /* End of ConstCode for SubSystem: '<Root>/Switch Case Action Subsystem' */
  }
}

/* Model terminate function */
void LDT_TMM_Model_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
