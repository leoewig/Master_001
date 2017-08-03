/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    ut_canbedded_stubs.h
 *
 * description:   CANbedded stub file
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li    CANbedded stub file
 *                       
 *
 *****************************************************************************/

extern bool_t ut_cbd_override;

/* Rx signals header start */
extern uint8_t ut_NSBCPTStatusChecksum_get;
extern uint32_t ut_NSBCPTStatusChecksum_get_ctr;
uint8_t ut_IlGetRxNSBCPTStatusChecksum(void);

extern uint16_t ut_PID43_AbsoluteLoad_get;
extern uint32_t ut_PID43_AbsoluteLoad_get_ctr;
uint16_t ut_IlGetRxPID43_AbsoluteLoad(void);

extern uint8_t ut_PID49_AbsolutePedalPosn_get;
extern uint32_t ut_PID49_AbsolutePedalPosn_get_ctr;
uint8_t ut_IlGetRxPID49_AbsolutePedalPosn(void);

extern uint8_t ut_NBatteryType_get;
extern uint32_t ut_NBatteryType_get_ctr;
uint8_t ut_IlGetRxNBatteryType(void);

extern uint16_t ut_PID0C_nEngine_get;
extern uint32_t ut_PID0C_nEngine_get_ctr;
uint16_t ut_IlGetRxPID0C_nEngine(void);

extern uint8_t ut_PID05_TEngineCoolant_get;
extern uint32_t ut_PID05_TEngineCoolant_get_ctr;
uint8_t ut_IlGetRxPID05_TEngineCoolant(void);

extern uint8_t ut_tSecond_get;
extern uint32_t ut_tSecond_get_ctr;
uint8_t ut_IlGetRxtSecond(void);

extern uint8_t ut_NECM_BMS_00Checksum_get;
extern uint32_t ut_NECM_BMS_00Checksum_get_ctr;
uint8_t ut_IlGetRxNECM_BMS_00Checksum(void);

extern uint16_t ut_nPowertrain_get;
extern uint32_t ut_nPowertrain_get_ctr;
uint16_t ut_IlGetRxnPowertrain(void);

extern uint8_t ut_NCarConfigMultiplexor_get;
extern uint32_t ut_NCarConfigMultiplexor_get_ctr;
uint8_t ut_IlGetRxNCarConfigMultiplexor(void);

extern uint8_t ut_NIgnitionStatus_get;
extern uint32_t ut_NIgnitionStatus_get_ctr;
uint8_t ut_IlGetRxNIgnitionStatus(void);

extern uint8_t ut_NCCU_1Checksum_get;
extern uint32_t ut_NCCU_1Checksum_get_ctr;
uint8_t ut_IlGetRxNCCU_1Checksum(void);

extern uint8_t ut_NMCUStatus_get;
extern uint32_t ut_NMCUStatus_get_ctr;
uint8_t ut_IlGetRxNMCUStatus(void);

extern uint8_t ut_tDay_get;
extern uint32_t ut_tDay_get_ctr;
uint8_t ut_IlGetRxtDay(void);

extern uint16_t ut_vVehicleIC_get;
extern uint32_t ut_vVehicleIC_get_ctr;
uint16_t ut_IlGetRxvVehicleIC(void);

extern uint8_t ut_qWaterPumpHybridCooling_get;
extern uint32_t ut_qWaterPumpHybridCooling_get_ctr;
uint8_t ut_IlGetRxqWaterPumpHybridCooling(void);

extern uint8_t ut_NContactorRequested_get;
extern uint32_t ut_NContactorRequested_get_ctr;
uint8_t ut_IlGetRxNContactorRequested(void);

extern uint8_t ut_NPowertrainStatus_get;
extern uint32_t ut_NPowertrainStatus_get_ctr;
uint8_t ut_IlGetRxNPowertrainStatus(void);

extern uint8_t ut_NEngineStatus_get;
extern uint32_t ut_NEngineStatus_get_ctr;
uint8_t ut_IlGetRxNEngineStatus(void);

extern uint8_t ut_NECM_BMS_00Counter_get;
extern uint32_t ut_NECM_BMS_00Counter_get_ctr;
uint8_t ut_IlGetRxNECM_BMS_00Counter(void);

extern uint8_t ut_NMCU_3Checksum_get;
extern uint32_t ut_NMCU_3Checksum_get_ctr;
uint8_t ut_IlGetRxNMCU_3Checksum(void);

extern uint8_t ut_tMonth_get;
extern uint32_t ut_tMonth_get_ctr;
uint8_t ut_IlGetRxtMonth(void);

extern uint8_t ut_tYear_get;
extern uint32_t ut_tYear_get_ctr;
uint8_t ut_IlGetRxtYear(void);

extern uint8_t ut_NVINDataBytesIndex_get;
extern uint32_t ut_NVINDataBytesIndex_get_ctr;
uint8_t ut_IlGetRxNVINDataBytesIndex(void);

extern uint8_t ut_NVINPart2_get[7];
extern uint32_t ut_NVINPart2_get_ctr;
void ut_IlGetRxNVINPart2(uint8_t * out);

extern uint8_t ut_NVINPart1_get[7];
extern uint32_t ut_NVINPart1_get_ctr;
void ut_IlGetRxNVINPart1(uint8_t * out);

extern uint8_t ut_tMinute_get;
extern uint32_t ut_tMinute_get_ctr;
uint8_t ut_IlGetRxtMinute(void);

extern uint8_t ut_NSBCPTStatusCounter_get;
extern uint32_t ut_NSBCPTStatusCounter_get_ctr;
uint8_t ut_IlGetRxNSBCPTStatusCounter(void);

extern uint32_t ut_lPART1Totalizer_get;
extern uint32_t ut_lPART1Totalizer_get_ctr;
uint32_t ut_IlGetRxlPART1Totalizer(void);

extern uint32_t ut_NVINPart3_get;
extern uint32_t ut_NVINPart3_get_ctr;
uint32_t ut_IlGetRxNVINPart3(void);

extern uint16_t ut_PID0D_vVehicleOBD_get;
extern uint32_t ut_PID0D_vVehicleOBD_get_ctr;
uint16_t ut_IlGetRxPID0D_vVehicleOBD(void);

extern uint8_t ut_PID04_CalculatedLoad_get;
extern uint32_t ut_PID04_CalculatedLoad_get_ctr;
uint8_t ut_IlGetRxPID04_CalculatedLoad(void);

extern uint8_t ut_BTBatteryHighLimitEnable_get;
extern uint32_t ut_BTBatteryHighLimitEnable_get_ctr;
uint8_t ut_IlGetRxBTBatteryHighLimitEnable(void);

extern uint8_t ut_NMCU_3Counter_get;
extern uint32_t ut_NMCU_3Counter_get_ctr;
uint8_t ut_IlGetRxNMCU_3Counter(void);

extern uint8_t ut_VBattery_get;
extern uint32_t ut_VBattery_get_ctr;
uint8_t ut_IlGetRxVBattery(void);

extern uint8_t ut_IDCDCOutput_get;
extern uint32_t ut_IDCDCOutput_get_ctr;
uint8_t ut_IlGetRxIDCDCOutput(void);

extern uint8_t ut_PID33_BarometricPressure_get;
extern uint32_t ut_PID33_BarometricPressure_get_ctr;
uint8_t ut_IlGetRxPID33_BarometricPressure(void);

extern uint8_t ut_NGearEngaged_get;
extern uint32_t ut_NGearEngaged_get_ctr;
uint8_t ut_IlGetRxNGearEngaged(void);

extern uint8_t ut_PID4A_AbsolutePedalPosn_get;
extern uint32_t ut_PID4A_AbsolutePedalPosn_get_ctr;
uint8_t ut_IlGetRxPID4A_AbsolutePedalPosn(void);

extern uint8_t ut_tHour_get;
extern uint32_t ut_tHour_get_ctr;
uint8_t ut_IlGetRxtHour(void);

extern uint8_t ut_NMCU_4Checksum_get;
extern uint32_t ut_NMCU_4Checksum_get_ctr;
uint8_t ut_IlGetRxNMCU_4Checksum(void);

extern uint8_t ut_NEPBEngaged_get;
extern uint32_t ut_NEPBEngaged_get_ctr;
uint8_t ut_IlGetRxNEPBEngaged(void);

extern uint8_t ut_NCCU_1Counter_get;
extern uint32_t ut_NCCU_1Counter_get_ctr;
uint8_t ut_IlGetRxNCCU_1Counter(void);

extern uint8_t ut_NCrashSeverity_get;
extern uint32_t ut_NCrashSeverity_get_ctr;
uint8_t ut_IlGetRxNCrashSeverity(void);

extern uint8_t ut_NMCU_4Counter_get;
extern uint32_t ut_NMCU_4Counter_get_ctr;
uint8_t ut_IlGetRxNMCU_4Counter(void);

extern uint8_t ut_TAmbientRaw_get;
extern uint32_t ut_TAmbientRaw_get_ctr;
uint8_t ut_IlGetRxTAmbientRaw(void);

extern uint8_t ut_IMCUDCLink_get;
extern uint32_t ut_IMCUDCLink_get_ctr;
uint8_t ut_IlGetRxIMCUDCLink(void);

/* Tx signals header start */
extern uint8_t ut_Additional_BMS_Errors_1_put;
extern uint32_t ut_Additional_BMS_Errors_1_put_ctr;
void ut_IlPutTxAdditional_BMS_Errors_1(uint8_t val);

extern uint8_t ut_Additional_BMS_Errors_2_put;
extern uint32_t ut_Additional_BMS_Errors_2_put_ctr;
void ut_IlPutTxAdditional_BMS_Errors_2(uint8_t val);

extern uint8_t ut_Additional_BMS_Errors_3_put;
extern uint32_t ut_Additional_BMS_Errors_3_put_ctr;
void ut_IlPutTxAdditional_BMS_Errors_3(uint8_t val);

extern uint8_t ut_Additional_BMS_Errors_SubType_3_put;
extern uint32_t ut_Additional_BMS_Errors_SubType_3_put_ctr;
void ut_IlPutTxAdditional_BMS_Errors_SubType_3(uint8_t val);

extern uint8_t ut_Additional_BMS_Errors_SubType_2_put;
extern uint32_t ut_Additional_BMS_Errors_SubType_2_put_ctr;
void ut_IlPutTxAdditional_BMS_Errors_SubType_2(uint8_t val);

extern uint8_t ut_Additional_BMS_Errors_SubType_1_put;
extern uint32_t ut_Additional_BMS_Errors_SubType_1_put_ctr;
void ut_IlPutTxAdditional_BMS_Errors_SubType_1(uint8_t val);

extern uint8_t ut_NTCellMin_put;
extern uint32_t ut_NTCellMin_put_ctr;
void ut_IlPutTxNTCellMin(uint8_t val);

extern uint8_t ut_NVCellMin_put;
extern uint32_t ut_NVCellMin_put_ctr;
void ut_IlPutTxNVCellMin(uint8_t val);

extern uint8_t ut_NVCellMax_put;
extern uint32_t ut_NVCellMax_put_ctr;
void ut_IlPutTxNVCellMax(uint8_t val);

extern uint16_t ut_VCellSOCMin_put;
extern uint32_t ut_VCellSOCMin_put_ctr;
void ut_IlPutTxVCellSOCMin(uint16_t val);

extern uint16_t ut_VCellSOCMax_put;
extern uint32_t ut_VCellSOCMax_put_ctr;
void ut_IlPutTxVCellSOCMax(uint16_t val);

extern uint8_t ut_NTCellMax_put;
extern uint32_t ut_NTCellMax_put_ctr;
void ut_IlPutTxNTCellMax(uint8_t val);

extern uint8_t ut_ctc_marker_put;
extern uint32_t ut_ctc_marker_put_ctr;
void ut_IlPutTxctc_marker(uint8_t val);

extern uint16_t ut_ctc_overall_voltage_put;
extern uint32_t ut_ctc_overall_voltage_put_ctr;
void ut_IlPutTxctc_overall_voltage(uint16_t val);

extern uint16_t ut_ctc_pos_voltage_put;
extern uint32_t ut_ctc_pos_voltage_put_ctr;
void ut_IlPutTxctc_pos_voltage(uint16_t val);

extern uint8_t ut_ctc_save_seq_put;
extern uint32_t ut_ctc_save_seq_put_ctr;
void ut_IlPutTxctc_save_seq(uint8_t val);

extern uint16_t ut_ctc_latest_imi_put;
extern uint32_t ut_ctc_latest_imi_put_ctr;
void ut_IlPutTxctc_latest_imi(uint16_t val);

extern uint8_t ut_NBMS_3AChecksum_put;
extern uint32_t ut_NBMS_3AChecksum_put_ctr;
void ut_IlPutTxNBMS_3AChecksum(uint8_t val);

extern uint16_t ut_NBMS_FirstFaultCode_put;
extern uint32_t ut_NBMS_FirstFaultCode_put_ctr;
void ut_IlPutTxNBMS_FirstFaultCode(uint16_t val);

extern uint16_t ut_NBMS_SecondFaultCode_put;
extern uint32_t ut_NBMS_SecondFaultCode_put_ctr;
void ut_IlPutTxNBMS_SecondFaultCode(uint16_t val);

extern uint16_t ut_NBMS_RecentFaultCode_put;
extern uint32_t ut_NBMS_RecentFaultCode_put_ctr;
void ut_IlPutTxNBMS_RecentFaultCode(uint16_t val);

extern uint8_t ut_NBMS_3ACounter_put;
extern uint32_t ut_NBMS_3ACounter_put_ctr;
void ut_IlPutTxNBMS_3ACounter(uint8_t val);

extern uint8_t ut_bms_dbg_39_0_put;
extern uint32_t ut_bms_dbg_39_0_put_ctr;
void ut_IlPutTxbms_dbg_39_0(uint8_t val);

extern uint8_t ut_bms_dbg_39_1_put;
extern uint32_t ut_bms_dbg_39_1_put_ctr;
void ut_IlPutTxbms_dbg_39_1(uint8_t val);

extern uint8_t ut_bms_dbg_39_4_put;
extern uint32_t ut_bms_dbg_39_4_put_ctr;
void ut_IlPutTxbms_dbg_39_4(uint8_t val);

extern uint8_t ut_bms_dbg_39_5_put;
extern uint32_t ut_bms_dbg_39_5_put_ctr;
void ut_IlPutTxbms_dbg_39_5(uint8_t val);

extern uint8_t ut_bms_dbg_39_6_put;
extern uint32_t ut_bms_dbg_39_6_put_ctr;
void ut_IlPutTxbms_dbg_39_6(uint8_t val);

extern uint8_t ut_bms_dbg_39_7_put;
extern uint32_t ut_bms_dbg_39_7_put_ctr;
void ut_IlPutTxbms_dbg_39_7(uint8_t val);

extern uint8_t ut_SOCcLow_put;
extern uint32_t ut_SOCcLow_put_ctr;
void ut_IlPutTxSOCcLow(uint8_t val);

extern uint8_t ut_SOCcHigh_put;
extern uint32_t ut_SOCcHigh_put_ctr;
void ut_IlPutTxSOCcHigh(uint8_t val);

extern uint8_t ut_ctc_no_close_reason_put;
extern uint32_t ut_ctc_no_close_reason_put_ctr;
void ut_IlPutTxctc_no_close_reason(uint8_t val);

extern uint8_t ut_bms_dbg_38_1_put;
extern uint32_t ut_bms_dbg_38_1_put_ctr;
void ut_IlPutTxbms_dbg_38_1(uint8_t val);

extern uint8_t ut_bms_dbg_38_0_put;
extern uint32_t ut_bms_dbg_38_0_put_ctr;
void ut_IlPutTxbms_dbg_38_0(uint8_t val);

extern int32_t ut_IRaw_put;
extern uint32_t ut_IRaw_put_ctr;
void ut_IlPutTxIRaw(int32_t val);

extern uint8_t ut_bms_dbg_38_7_put;
extern uint32_t ut_bms_dbg_38_7_put_ctr;
void ut_IlPutTxbms_dbg_38_7(uint8_t val);

extern uint8_t ut_bms_dbg_38_6_put;
extern uint32_t ut_bms_dbg_38_6_put_ctr;
void ut_IlPutTxbms_dbg_38_6(uint8_t val);

extern uint8_t ut_NBMS_05Checksum_put;
extern uint32_t ut_NBMS_05Checksum_put_ctr;
void ut_IlPutTxNBMS_05Checksum(uint8_t val);

extern uint8_t ut_rLastChargeSOCDelta_put;
extern uint32_t ut_rLastChargeSOCDelta_put_ctr;
void ut_IlPutTxrLastChargeSOCDelta(uint8_t val);

extern uint8_t ut_rBatterySOHCPB_put;
extern uint32_t ut_rBatterySOHCPB_put_ctr;
void ut_IlPutTxrBatterySOHCPB(uint8_t val);

extern uint8_t ut_rBatterySOC_put;
extern uint32_t ut_rBatterySOC_put_ctr;
void ut_IlPutTxrBatterySOC(uint8_t val);

extern uint8_t ut_NBatterySOCDesired_put;
extern uint32_t ut_NBatterySOCDesired_put_ctr;
void ut_IlPutTxNBatterySOCDesired(uint8_t val);

extern uint8_t ut_NBMSClusterMessage_put;
extern uint32_t ut_NBMSClusterMessage_put_ctr;
void ut_IlPutTxNBMSClusterMessage(uint8_t val);
extern bool_t ut_NBMSClusterMessageConfirmation;
extern uint32_t ut_NBMSClusterMessageConfirmation_ctr;
uint8_t ut_IlGetNBMSClusterMessageConfirmation(void);
extern uint32_t ut_NBMSClusterMessageConfirmation_clr_ctr;
void ut_IlClrNBMSClusterMessageConfirmation(void);

extern uint8_t ut_NBMS_05Counter_put;
extern uint32_t ut_NBMS_05Counter_put_ctr;
void ut_IlPutTxNBMS_05Counter(uint8_t val);

extern uint8_t ut_BBatteryWarrantyVoid_put;
extern uint32_t ut_BBatteryWarrantyVoid_put_ctr;
void ut_IlPutTxBBatteryWarrantyVoid(uint8_t val);

extern uint16_t ut_TCellMax_put;
extern uint32_t ut_TCellMax_put_ctr;
void ut_IlPutTxTCellMax(uint16_t val);

extern uint8_t ut_rBatterySOHCPC_put;
extern uint32_t ut_rBatterySOHCPC_put_ctr;
void ut_IlPutTxrBatterySOHCPC(uint8_t val);

extern uint16_t ut_TCellMin_put;
extern uint32_t ut_TCellMin_put_ctr;
void ut_IlPutTxTCellMin(uint16_t val);

extern uint8_t ut_TBMSOutlet_put;
extern uint32_t ut_TBMSOutlet_put_ctr;
void ut_IlPutTxTBMSOutlet(uint8_t val);

extern uint8_t ut_NBMS_04Checksum_put;
extern uint32_t ut_NBMS_04Checksum_put_ctr;
void ut_IlPutTxNBMS_04Checksum(uint8_t val);

extern uint8_t ut_TBMSInlet_put;
extern uint32_t ut_TBMSInlet_put_ctr;
void ut_IlPutTxTBMSInlet(uint8_t val);

extern uint8_t ut_BBMSChargerTetheredStatus_put;
extern uint32_t ut_BBMSChargerTetheredStatus_put_ctr;
void ut_IlPutTxBBMSChargerTetheredStatus(uint8_t val);

extern uint8_t ut_NBMS_04Counter_put;
extern uint32_t ut_NBMS_04Counter_put_ctr;
void ut_IlPutTxNBMS_04Counter(uint8_t val);

extern int8_t ut_mad_A_packMovAvgNet_A_put;
extern uint32_t ut_mad_A_packMovAvgNet_A_put_ctr;
void ut_IlPutTxmad_A_packMovAvgNet_A(int8_t val);

extern uint16_t ut_hrd_cellSerResEst_Ohm_put;
extern uint32_t ut_hrd_cellSerResEst_Ohm_put_ctr;
void ut_IlPutTxhrd_cellSerResEst_Ohm(uint16_t val);

extern uint8_t ut_mad_A_packMovAvgChg_A_put;
extern uint32_t ut_mad_A_packMovAvgChg_A_put_ctr;
void ut_IlPutTxmad_A_packMovAvgChg_A(uint8_t val);

extern uint8_t ut_mad_A_packMovAvgDis_A_put;
extern uint32_t ut_mad_A_packMovAvgDis_A_put_ctr;
void ut_IlPutTxmad_A_packMovAvgDis_A(uint8_t val);

extern uint16_t ut_dcd_tm_ceRunTime0_put;
extern uint32_t ut_dcd_tm_ceRunTime0_put_ctr;
void ut_IlPutTxdcd_tm_ceRunTime0(uint16_t val);

extern int16_t ut_VPackDCLink_put;
extern uint32_t ut_VPackDCLink_put_ctr;
void ut_IlPutTxVPackDCLink(int16_t val);

extern uint16_t ut_dcd_tm_socRunTime0_put;
extern uint32_t ut_dcd_tm_socRunTime0_put_ctr;
void ut_IlPutTxdcd_tm_socRunTime0(uint16_t val);

extern uint8_t ut_acv_enum_packDisLimReasonFF_Med_put;
extern uint32_t ut_acv_enum_packDisLimReasonFF_Med_put_ctr;
void ut_IlPutTxacv_enum_packDisLimReasonFF_Med(uint8_t val);

extern uint8_t ut_acv_enum_packChgLimReasonFF_Med_put;
extern uint32_t ut_acv_enum_packChgLimReasonFF_Med_put_ctr;
void ut_IlPutTxacv_enum_packChgLimReasonFF_Med(uint8_t val);

extern uint8_t ut_acv_enum_packDisLimReasonFF_Long_put;
extern uint32_t ut_acv_enum_packDisLimReasonFF_Long_put_ctr;
void ut_IlPutTxacv_enum_packDisLimReasonFF_Long(uint8_t val);

extern uint8_t ut_acv_enum_packChgLimReasonFF_Shrt_put;
extern uint32_t ut_acv_enum_packChgLimReasonFF_Shrt_put_ctr;
void ut_IlPutTxacv_enum_packChgLimReasonFF_Shrt(uint8_t val);

extern uint8_t ut_acv_enum_packChgLimReasonFF_Long_put;
extern uint32_t ut_acv_enum_packChgLimReasonFF_Long_put_ctr;
void ut_IlPutTxacv_enum_packChgLimReasonFF_Long(uint8_t val);

extern uint8_t ut_acv_enum_packDisLimReasonFF_Shrt_put;
extern uint32_t ut_acv_enum_packDisLimReasonFF_Shrt_put_ctr;
void ut_IlPutTxacv_enum_packDisLimReasonFF_Shrt(uint8_t val);

extern uint8_t ut_scv_pct_socObvEst_min_put;
extern uint32_t ut_scv_pct_socObvEst_min_put_ctr;
void ut_IlPutTxscv_pct_socObvEst_min(uint8_t val);

extern uint8_t ut_cev_pct_cellVSOCEst_min_put;
extern uint32_t ut_cev_pct_cellVSOCEst_min_put_ctr;
void ut_IlPutTxcev_pct_cellVSOCEst_min(uint8_t val);

extern uint8_t ut_cev_pct_cellVSOCEst_max_put;
extern uint32_t ut_cev_pct_cellVSOCEst_max_put_ctr;
void ut_IlPutTxcev_pct_cellVSOCEst_max(uint8_t val);

extern uint8_t ut_scv_pct_socObvEst_max_put;
extern uint32_t ut_scv_pct_socObvEst_max_put_ctr;
void ut_IlPutTxscv_pct_socObvEst_max(uint8_t val);

extern uint8_t ut_ccv_pct_cellSOC_max_put;
extern uint32_t ut_ccv_pct_cellSOC_max_put_ctr;
void ut_IlPutTxccv_pct_cellSOC_max(uint8_t val);

extern uint8_t ut_ccv_pct_cellSOC_min_put;
extern uint32_t ut_ccv_pct_cellSOC_min_put_ctr;
void ut_IlPutTxccv_pct_cellSOC_min(uint8_t val);

extern int8_t ut_tmv_t_cellCoreModel_dC_min_put;
extern uint32_t ut_tmv_t_cellCoreModel_dC_min_put_ctr;
void ut_IlPutTxtmv_t_cellCoreModel_dC_min(int8_t val);

extern int8_t ut_tmv_t_cellCoreModel_dC_max_put;
extern uint32_t ut_tmv_t_cellCoreModel_dC_max_put_ctr;
void ut_IlPutTxtmv_t_cellCoreModel_dC_max(int8_t val);

extern uint16_t ut_PDischargeMaxShortTerm_put;
extern uint32_t ut_PDischargeMaxShortTerm_put_ctr;
void ut_IlPutTxPDischargeMaxShortTerm(uint16_t val);

extern uint8_t ut_NBMS_01Counter_put;
extern uint32_t ut_NBMS_01Counter_put_ctr;
void ut_IlPutTxNBMS_01Counter(uint8_t val);

extern uint16_t ut_IDischargeMaxShortTerm_put;
extern uint32_t ut_IDischargeMaxShortTerm_put_ctr;
void ut_IlPutTxIDischargeMaxShortTerm(uint16_t val);

extern uint8_t ut_NBMS_01Checksum_put;
extern uint32_t ut_NBMS_01Checksum_put_ctr;
void ut_IlPutTxNBMS_01Checksum(uint8_t val);

extern uint16_t ut_tChargingRemainingDisplayed_put;
extern uint32_t ut_tChargingRemainingDisplayed_put_ctr;
void ut_IlPutTxtChargingRemainingDisplayed(uint16_t val);

extern uint16_t ut_VHybridBattery_put;
extern uint32_t ut_VHybridBattery_put_ctr;
void ut_IlPutTxVHybridBattery(uint16_t val);

extern uint8_t ut_NBMS_00Counter_put;
extern uint32_t ut_NBMS_00Counter_put_ctr;
void ut_IlPutTxNBMS_00Counter(uint8_t val);

extern uint16_t ut_IActual_put;
extern uint32_t ut_IActual_put_ctr;
void ut_IlPutTxIActual(uint16_t val);

extern uint8_t ut_NBMS_00Checksum_put;
extern uint32_t ut_NBMS_00Checksum_put_ctr;
void ut_IlPutTxNBMS_00Checksum(uint8_t val);

extern uint16_t ut_PActual_put;
extern uint32_t ut_PActual_put_ctr;
void ut_IlPutTxPActual(uint16_t val);

extern uint16_t ut_PDischargeMaxLongTerm_put;
extern uint32_t ut_PDischargeMaxLongTerm_put_ctr;
void ut_IlPutTxPDischargeMaxLongTerm(uint16_t val);

extern uint8_t ut_NBMS_08Counter_put;
extern uint32_t ut_NBMS_08Counter_put_ctr;
void ut_IlPutTxNBMS_08Counter(uint8_t val);

extern uint8_t ut_NBMS_08Checksum_put;
extern uint32_t ut_NBMS_08Checksum_put_ctr;
void ut_IlPutTxNBMS_08Checksum(uint8_t val);

extern uint16_t ut_IDischargeMaxLongTerm_put;
extern uint32_t ut_IDischargeMaxLongTerm_put_ctr;
void ut_IlPutTxIDischargeMaxLongTerm(uint16_t val);

extern uint16_t ut_PChargeMaxLongTerm_put;
extern uint32_t ut_PChargeMaxLongTerm_put_ctr;
void ut_IlPutTxPChargeMaxLongTerm(uint16_t val);

extern uint8_t ut_NBMS_09Checksum_put;
extern uint32_t ut_NBMS_09Checksum_put_ctr;
void ut_IlPutTxNBMS_09Checksum(uint8_t val);

extern uint8_t ut_NBMS_09Counter_put;
extern uint32_t ut_NBMS_09Counter_put_ctr;
void ut_IlPutTxNBMS_09Counter(uint8_t val);

extern uint16_t ut_IChargeMaxLongTerm_put;
extern uint32_t ut_IChargeMaxLongTerm_put_ctr;
void ut_IlPutTxIChargeMaxLongTerm(uint16_t val);

extern uint16_t ut_PDischargeMaxMediumTerm_put;
extern uint32_t ut_PDischargeMaxMediumTerm_put_ctr;
void ut_IlPutTxPDischargeMaxMediumTerm(uint16_t val);

extern uint8_t ut_NBMS_06Checksum_put;
extern uint32_t ut_NBMS_06Checksum_put_ctr;
void ut_IlPutTxNBMS_06Checksum(uint8_t val);

extern uint16_t ut_IDischargeMaxMediumTerm_put;
extern uint32_t ut_IDischargeMaxMediumTerm_put_ctr;
void ut_IlPutTxIDischargeMaxMediumTerm(uint16_t val);

extern uint8_t ut_NBMS_06Counter_put;
extern uint32_t ut_NBMS_06Counter_put_ctr;
void ut_IlPutTxNBMS_06Counter(uint8_t val);

extern uint8_t ut_NBMS_07Checksum_put;
extern uint32_t ut_NBMS_07Checksum_put_ctr;
void ut_IlPutTxNBMS_07Checksum(uint8_t val);

extern uint16_t ut_PChargeMaxMediumTerm_put;
extern uint32_t ut_PChargeMaxMediumTerm_put_ctr;
void ut_IlPutTxPChargeMaxMediumTerm(uint16_t val);

extern uint16_t ut_IChargeMaxMediumTerm_put;
extern uint32_t ut_IChargeMaxMediumTerm_put_ctr;
void ut_IlPutTxIChargeMaxMediumTerm(uint16_t val);

extern uint8_t ut_NBMS_07Counter_put;
extern uint32_t ut_NBMS_07Counter_put_ctr;
void ut_IlPutTxNBMS_07Counter(uint8_t val);

extern uint8_t ut_NBMSStatus_put;
extern uint32_t ut_NBMSStatus_put_ctr;
void ut_IlPutTxNBMSStatus(uint8_t val);

extern uint8_t ut_BPrechargeContactorState_put;
extern uint32_t ut_BPrechargeContactorState_put_ctr;
void ut_IlPutTxBPrechargeContactorState(uint8_t val);

extern uint8_t ut_NBMSMILRequested_put;
extern uint32_t ut_NBMSMILRequested_put_ctr;
void ut_IlPutTxNBMSMILRequested(uint8_t val);

extern uint8_t ut_BBMSContactorOpenIndicator_put;
extern uint32_t ut_BBMSContactorOpenIndicator_put_ctr;
void ut_IlPutTxBBMSContactorOpenIndicator(uint8_t val);

extern uint8_t ut_BContactorStuckOpen_put;
extern uint32_t ut_BContactorStuckOpen_put_ctr;
void ut_IlPutTxBContactorStuckOpen(uint8_t val);

extern uint8_t ut_NHVBSoCResetState_put;
extern uint32_t ut_NHVBSoCResetState_put_ctr;
void ut_IlPutTxNHVBSoCResetState(uint8_t val);

extern uint8_t ut_BContactorStuckClosed_put;
extern uint32_t ut_BContactorStuckClosed_put_ctr;
void ut_IlPutTxBContactorStuckClosed(uint8_t val);

extern uint8_t ut_BBMSContactorCloseIndicator_put;
extern uint32_t ut_BBMSContactorCloseIndicator_put_ctr;
void ut_IlPutTxBBMSContactorCloseIndicator(uint8_t val);

extern uint8_t ut_NBMS_03Counter_put;
extern uint32_t ut_NBMS_03Counter_put_ctr;
void ut_IlPutTxNBMS_03Counter(uint8_t val);

extern uint8_t ut_NBMS_03Checksum_put;
extern uint32_t ut_NBMS_03Checksum_put_ctr;
void ut_IlPutTxNBMS_03Checksum(uint8_t val);

extern uint8_t ut_BChargerNegContactorState_put;
extern uint32_t ut_BChargerNegContactorState_put_ctr;
void ut_IlPutTxBChargerNegContactorState(uint8_t val);

extern uint8_t ut_NFullDischargeRequested_put;
extern uint32_t ut_NFullDischargeRequested_put_ctr;
void ut_IlPutTxNFullDischargeRequested(uint8_t val);

extern uint8_t ut_BVehicleNegContactorState_put;
extern uint32_t ut_BVehicleNegContactorState_put_ctr;
void ut_IlPutTxBVehicleNegContactorState(uint8_t val);

extern uint8_t ut_BChargerPosContactorState_put;
extern uint32_t ut_BChargerPosContactorState_put_ctr;
void ut_IlPutTxBChargerPosContactorState(uint8_t val);

extern uint8_t ut_BVehiclePosContactorState_put;
extern uint32_t ut_BVehiclePosContactorState_put_ctr;
void ut_IlPutTxBVehiclePosContactorState(uint8_t val);

extern uint16_t ut_IChargeMaxShortTerm_put;
extern uint32_t ut_IChargeMaxShortTerm_put_ctr;
void ut_IlPutTxIChargeMaxShortTerm(uint16_t val);

extern uint8_t ut_NBMS_02Counter_put;
extern uint32_t ut_NBMS_02Counter_put_ctr;
void ut_IlPutTxNBMS_02Counter(uint8_t val);

extern uint16_t ut_PChargeMaxShortTerm_put;
extern uint32_t ut_PChargeMaxShortTerm_put_ctr;
void ut_IlPutTxPChargeMaxShortTerm(uint16_t val);

extern uint8_t ut_NBMS_02Checksum_put;
extern uint32_t ut_NBMS_02Checksum_put_ctr;
void ut_IlPutTxNBMS_02Checksum(uint8_t val);

/* Rx groups header start */
/* Tx groups header start */
/* Rx signals header start */
extern uint8_t ut_ISA_Fourth_status_get;
extern uint32_t ut_ISA_Fourth_status_get_ctr;
uint8_t ut_IlGetRxISA_Fourth_status(void);

extern uint8_t ut_ISA_Response_ID_get;
extern uint32_t ut_ISA_Response_ID_get_ctr;
uint8_t ut_IlGetRxISA_Response_ID(void);

extern uint8_t ut_ISA_Response_DB1_get;
extern uint32_t ut_ISA_Response_DB1_get_ctr;
uint8_t ut_IlGetRxISA_Response_DB1(void);

extern uint8_t ut_ISA_Response_DB2_get;
extern uint32_t ut_ISA_Response_DB2_get_ctr;
uint8_t ut_IlGetRxISA_Response_DB2(void);

extern uint8_t ut_ISA_Response_DB3_get;
extern uint32_t ut_ISA_Response_DB3_get_ctr;
uint8_t ut_IlGetRxISA_Response_DB3(void);

extern uint8_t ut_ISA_Response_DB4_get;
extern uint32_t ut_ISA_Response_DB4_get_ctr;
uint8_t ut_IlGetRxISA_Response_DB4(void);

extern uint8_t ut_ISA_Response_DB5_get;
extern uint32_t ut_ISA_Response_DB5_get_ctr;
uint8_t ut_IlGetRxISA_Response_DB5(void);

extern uint8_t ut_ISA_First_status_get;
extern uint32_t ut_ISA_First_status_get_ctr;
uint8_t ut_IlGetRxISA_First_status(void);

extern uint8_t ut_ISA_Response_DB7_get;
extern uint32_t ut_ISA_Response_DB7_get_ctr;
uint8_t ut_IlGetRxISA_Response_DB7(void);

extern uint8_t ut_ISA_Second_status_get;
extern uint32_t ut_ISA_Second_status_get_ctr;
uint8_t ut_IlGetRxISA_Second_status(void);

extern uint8_t ut_ISA_Third_type_get;
extern uint32_t ut_ISA_Third_type_get_ctr;
uint8_t ut_IlGetRxISA_Third_type(void);

extern uint8_t ut_ISA_Third_status_get;
extern uint32_t ut_ISA_Third_status_get_ctr;
uint8_t ut_IlGetRxISA_Third_status(void);

extern int32_t ut_ISA_Third_result_get;
extern uint32_t ut_ISA_Third_result_get_ctr;
int32_t ut_IlGetRxISA_Third_result(void);

extern uint8_t ut_ISA_Response_DB6_get;
extern uint32_t ut_ISA_Response_DB6_get_ctr;
uint8_t ut_IlGetRxISA_Response_DB6(void);

extern uint8_t ut_ISA_Second_type_get;
extern uint32_t ut_ISA_Second_type_get_ctr;
uint8_t ut_IlGetRxISA_Second_type(void);

extern int32_t ut_ISA_Fifth_result_get;
extern uint32_t ut_ISA_Fifth_result_get_ctr;
int32_t ut_IlGetRxISA_Fifth_result(void);

extern uint8_t ut_ISA_Fifth_type_get;
extern uint32_t ut_ISA_Fifth_type_get_ctr;
uint8_t ut_IlGetRxISA_Fifth_type(void);

extern uint8_t ut_ISA_Sixth_status_get;
extern uint32_t ut_ISA_Sixth_status_get_ctr;
uint8_t ut_IlGetRxISA_Sixth_status(void);

extern uint8_t ut_ISA_Sixth_type_get;
extern uint32_t ut_ISA_Sixth_type_get_ctr;
uint8_t ut_IlGetRxISA_Sixth_type(void);

extern int32_t ut_ISA_Sixth_result_get;
extern uint32_t ut_ISA_Sixth_result_get_ctr;
int32_t ut_IlGetRxISA_Sixth_result(void);

extern uint8_t ut_ISA_Fifth_status_get;
extern uint32_t ut_ISA_Fifth_status_get_ctr;
uint8_t ut_IlGetRxISA_Fifth_status(void);

extern int32_t ut_ISA_Fourth_result_get;
extern uint32_t ut_ISA_Fourth_result_get_ctr;
int32_t ut_IlGetRxISA_Fourth_result(void);

extern uint8_t ut_ISA_First_type_get;
extern uint32_t ut_ISA_First_type_get_ctr;
uint8_t ut_IlGetRxISA_First_type(void);

extern uint8_t ut_ISA_Fourth_type_get;
extern uint32_t ut_ISA_Fourth_type_get_ctr;
uint8_t ut_IlGetRxISA_Fourth_type(void);

extern int32_t ut_ISA_Second_result_get;
extern uint32_t ut_ISA_Second_result_get_ctr;
int32_t ut_IlGetRxISA_Second_result(void);

extern int32_t ut_ISA_First_result_get;
extern uint32_t ut_ISA_First_result_get_ctr;
int32_t ut_IlGetRxISA_First_result(void);

/* Tx signals header start */
extern uint8_t ut_ISA_Cmd_DB4_put;
extern uint32_t ut_ISA_Cmd_DB4_put_ctr;
void ut_IlPutTxISA_Cmd_DB4(uint8_t val);

extern uint8_t ut_ISA_Cmd_DB5_put;
extern uint32_t ut_ISA_Cmd_DB5_put_ctr;
void ut_IlPutTxISA_Cmd_DB5(uint8_t val);

extern uint8_t ut_ISA_Cmd_DB6_put;
extern uint32_t ut_ISA_Cmd_DB6_put_ctr;
void ut_IlPutTxISA_Cmd_DB6(uint8_t val);

extern uint8_t ut_ISA_Cmd_DB7_put;
extern uint32_t ut_ISA_Cmd_DB7_put_ctr;
void ut_IlPutTxISA_Cmd_DB7(uint8_t val);

extern uint8_t ut_ISA_Cmd_DB1_put;
extern uint32_t ut_ISA_Cmd_DB1_put_ctr;
void ut_IlPutTxISA_Cmd_DB1(uint8_t val);

extern uint8_t ut_ISA_Cmd_DB2_put;
extern uint32_t ut_ISA_Cmd_DB2_put_ctr;
void ut_IlPutTxISA_Cmd_DB2(uint8_t val);

extern uint8_t ut_ISA_Cmd_DB3_put;
extern uint32_t ut_ISA_Cmd_DB3_put_ctr;
void ut_IlPutTxISA_Cmd_DB3(uint8_t val);

extern uint8_t ut_ISA_Command_ID_put;
extern uint32_t ut_ISA_Command_ID_put_ctr;
void ut_IlPutTxISA_Command_ID(uint8_t val);

/* Rx groups header start */
extern uint32_t ut_sixth_grp_get_ctr;
void ut_IlGetRxsixth_grp(void);
extern bool_t ut_sixth_grpIndication;
extern uint32_t ut_sixth_grpIndication_ctr;
uint8_t ut_IlGetsixth_grpIndication(void);
extern uint32_t ut_sixth_grpIndication_clr_ctr;
void ut_IlClrsixth_grpIndication(void);

extern uint32_t ut_fifth_grp_get_ctr;
void ut_IlGetRxfifth_grp(void);
extern bool_t ut_fifth_grpIndication;
extern uint32_t ut_fifth_grpIndication_ctr;
uint8_t ut_IlGetfifth_grpIndication(void);
extern uint32_t ut_fifth_grpIndication_clr_ctr;
void ut_IlClrfifth_grpIndication(void);

extern uint32_t ut_third_grp_get_ctr;
void ut_IlGetRxthird_grp(void);
extern bool_t ut_third_grpIndication;
extern uint32_t ut_third_grpIndication_ctr;
uint8_t ut_IlGetthird_grpIndication(void);
extern uint32_t ut_third_grpIndication_clr_ctr;
void ut_IlClrthird_grpIndication(void);

extern uint32_t ut_fourth_grp_get_ctr;
void ut_IlGetRxfourth_grp(void);
extern bool_t ut_fourth_grpIndication;
extern uint32_t ut_fourth_grpIndication_ctr;
uint8_t ut_IlGetfourth_grpIndication(void);
extern uint32_t ut_fourth_grpIndication_clr_ctr;
void ut_IlClrfourth_grpIndication(void);

extern uint32_t ut_second_grp_get_ctr;
void ut_IlGetRxsecond_grp(void);
extern bool_t ut_second_grpIndication;
extern uint32_t ut_second_grpIndication_ctr;
uint8_t ut_IlGetsecond_grpIndication(void);
extern uint32_t ut_second_grpIndication_clr_ctr;
void ut_IlClrsecond_grpIndication(void);

extern uint32_t ut_first_grp_get_ctr;
void ut_IlGetRxfirst_grp(void);
extern bool_t ut_first_grpIndication;
extern uint32_t ut_first_grpIndication_ctr;
uint8_t ut_IlGetfirst_grpIndication(void);
extern uint32_t ut_first_grpIndication_clr_ctr;
void ut_IlClrfirst_grpIndication(void);

extern uint32_t ut_response_grp_get_ctr;
void ut_IlGetRxresponse_grp(void);
extern bool_t ut_response_grpIndication;
extern uint32_t ut_response_grpIndication_ctr;
uint8_t ut_IlGetresponse_grpIndication(void);
extern uint32_t ut_response_grpIndication_clr_ctr;
void ut_IlClrresponse_grpIndication(void);

/* Tx groups header start */
extern uint32_t ut_Command_grp_put_ctr;
void ut_IlPutTxCommand_grp(void);

/* Rx signals header start */
extern uint8_t ut_BChargerCableFault_get;
extern uint32_t ut_BChargerCableFault_get_ctr;
uint8_t ut_IlGetRxBChargerCableFault(void);

extern uint8_t ut_NCharger12VSupplyStatus_get;
extern uint32_t ut_NCharger12VSupplyStatus_get_ctr;
uint8_t ut_IlGetRxNCharger12VSupplyStatus(void);

extern uint8_t ut_NOBC_1Counter_get;
extern uint32_t ut_NOBC_1Counter_get_ctr;
uint8_t ut_IlGetRxNOBC_1Counter(void);

extern uint8_t ut_IChargerMax_get;
extern uint32_t ut_IChargerMax_get_ctr;
uint8_t ut_IlGetRxIChargerMax(void);

extern uint8_t ut_BChargerIsolationInhibit_get;
extern uint32_t ut_BChargerIsolationInhibit_get_ctr;
uint8_t ut_IlGetRxBChargerIsolationInhibit(void);

extern uint8_t ut_NOBC_2Checksum_get;
extern uint32_t ut_NOBC_2Checksum_get_ctr;
uint8_t ut_IlGetRxNOBC_2Checksum(void);

extern uint8_t ut_BChargerDetectBMSFault_get;
extern uint32_t ut_BChargerDetectBMSFault_get_ctr;
uint8_t ut_IlGetRxBChargerDetectBMSFault(void);

extern uint8_t ut_BBMSIndicatesFault_get;
extern uint32_t ut_BBMSIndicatesFault_get_ctr;
uint8_t ut_IlGetRxBBMSIndicatesFault(void);

extern uint16_t ut_VCharger_get;
extern uint32_t ut_VCharger_get_ctr;
uint16_t ut_IlGetRxVCharger(void);

extern uint8_t ut_NChargerStatus_get;
extern uint32_t ut_NChargerStatus_get_ctr;
uint8_t ut_IlGetRxNChargerStatus(void);

extern uint8_t ut_NOBC_2Counter_get;
extern uint32_t ut_NOBC_2Counter_get_ctr;
uint8_t ut_IlGetRxNOBC_2Counter(void);

extern uint8_t ut_NChargerHVContactorStatus_get;
extern uint32_t ut_NChargerHVContactorStatus_get_ctr;
uint8_t ut_IlGetRxNChargerHVContactorStatus(void);

extern uint16_t ut_NChargerSerialID_get;
extern uint32_t ut_NChargerSerialID_get_ctr;
uint16_t ut_IlGetRxNChargerSerialID(void);

extern uint8_t ut_BChargerHVFault_get;
extern uint32_t ut_BChargerHVFault_get_ctr;
uint8_t ut_IlGetRxBChargerHVFault(void);

extern uint8_t ut_ICharger_get;
extern uint32_t ut_ICharger_get_ctr;
uint8_t ut_IlGetRxICharger(void);

extern uint8_t ut_NOBC_1Checksum_get;
extern uint32_t ut_NOBC_1Checksum_get_ctr;
uint8_t ut_IlGetRxNOBC_1Checksum(void);

extern uint8_t ut_BChargerFault_get;
extern uint32_t ut_BChargerFault_get_ctr;
uint8_t ut_IlGetRxBChargerFault(void);

extern uint8_t ut_BChargerIndicatesEPBInhibit_get;
extern uint32_t ut_BChargerIndicatesEPBInhibit_get_ctr;
uint8_t ut_IlGetRxBChargerIndicatesEPBInhibit(void);

/* Tx signals header start */
extern uint8_t ut_BBMSIsolationInhibit_put;
extern uint32_t ut_BBMSIsolationInhibit_put_ctr;
void ut_IlPutTxBBMSIsolationInhibit(uint8_t val);

extern uint8_t ut_NChargingCompleteStatus_put;
extern uint32_t ut_NChargingCompleteStatus_put_ctr;
void ut_IlPutTxNChargingCompleteStatus(uint8_t val);

extern uint8_t ut_BConnectorCheckInhibit_put;
extern uint32_t ut_BConnectorCheckInhibit_put_ctr;
void ut_IlPutTxBConnectorCheckInhibit(uint8_t val);

extern uint8_t ut_rBatterySOCCharger_put;
extern uint32_t ut_rBatterySOCCharger_put_ctr;
void ut_IlPutTxrBatterySOCCharger(uint8_t val);

extern uint8_t ut_BBMSIndicateChargerFault_put;
extern uint32_t ut_BBMSIndicateChargerFault_put_ctr;
void ut_IlPutTxBBMSIndicateChargerFault(uint8_t val);

extern uint8_t ut_NChargerHVContactorReq_put;
extern uint32_t ut_NChargerHVContactorReq_put_ctr;
void ut_IlPutTxNChargerHVContactorReq(uint8_t val);

extern uint8_t ut_BVehicleAwakeInhibit_put;
extern uint32_t ut_BVehicleAwakeInhibit_put_ctr;
void ut_IlPutTxBVehicleAwakeInhibit(uint8_t val);

extern uint8_t ut_B12VSupplyInhibit_put;
extern uint32_t ut_B12VSupplyInhibit_put_ctr;
void ut_IlPutTxB12VSupplyInhibit(uint8_t val);

extern uint8_t ut_NBMS_CH1Checksum_put;
extern uint32_t ut_NBMS_CH1Checksum_put_ctr;
void ut_IlPutTxNBMS_CH1Checksum(uint8_t val);

extern uint8_t ut_BCableFault_put;
extern uint32_t ut_BCableFault_put_ctr;
void ut_IlPutTxBCableFault(uint8_t val);

extern uint8_t ut_BBMSFault_put;
extern uint32_t ut_BBMSFault_put_ctr;
void ut_IlPutTxBBMSFault(uint8_t val);

extern uint8_t ut_BBMSHVFault_put;
extern uint32_t ut_BBMSHVFault_put_ctr;
void ut_IlPutTxBBMSHVFault(uint8_t val);

extern uint8_t ut_IChargerSet_put;
extern uint32_t ut_IChargerSet_put_ctr;
void ut_IlPutTxIChargerSet(uint8_t val);

extern uint8_t ut_BEPBInhibit_put;
extern uint32_t ut_BEPBInhibit_put_ctr;
void ut_IlPutTxBEPBInhibit(uint8_t val);

extern uint8_t ut_NBMS_CH1Counter_put;
extern uint32_t ut_NBMS_CH1Counter_put_ctr;
void ut_IlPutTxNBMS_CH1Counter(uint8_t val);

extern uint16_t ut_VChargerSet_put;
extern uint32_t ut_VChargerSet_put_ctr;
void ut_IlPutTxVChargerSet(uint16_t val);

extern uint8_t ut_NBMSCommandedMode_put;
extern uint32_t ut_NBMSCommandedMode_put_ctr;
void ut_IlPutTxNBMSCommandedMode(uint8_t val);

extern uint8_t ut_BBMSVehicleShuttingDownInhibit_put;
extern uint32_t ut_BBMSVehicleShuttingDownInhibit_put_ctr;
void ut_IlPutTxBBMSVehicleShuttingDownInhibit(uint8_t val);

extern uint8_t ut_BChargerIndicatesFault_put;
extern uint32_t ut_BChargerIndicatesFault_put_ctr;
void ut_IlPutTxBChargerIndicatesFault(uint8_t val);

extern uint8_t ut_BVehicleFault_put;
extern uint32_t ut_BVehicleFault_put_ctr;
void ut_IlPutTxBVehicleFault(uint8_t val);

extern uint16_t ut_RBMSHVPos_put;
extern uint32_t ut_RBMSHVPos_put_ctr;
void ut_IlPutTxRBMSHVPos(uint16_t val);

extern uint8_t ut_NBMS_CH2Counter_put;
extern uint32_t ut_NBMS_CH2Counter_put_ctr;
void ut_IlPutTxNBMS_CH2Counter(uint8_t val);

extern uint16_t ut_RBMSHVNeg_put;
extern uint32_t ut_RBMSHVNeg_put_ctr;
void ut_IlPutTxRBMSHVNeg(uint16_t val);

extern uint8_t ut_NBMS_CH2Checksum_put;
extern uint32_t ut_NBMS_CH2Checksum_put_ctr;
void ut_IlPutTxNBMS_CH2Checksum(uint8_t val);

extern uint8_t ut_BIsTetheredToSomething_put;
extern uint32_t ut_BIsTetheredToSomething_put_ctr;
void ut_IlPutTxBIsTetheredToSomething(uint8_t val);

extern uint8_t ut_BIsTetheredToActiveCharger_put;
extern uint32_t ut_BIsTetheredToActiveCharger_put_ctr;
void ut_IlPutTxBIsTetheredToActiveCharger(uint8_t val);

extern uint8_t ut_BPilotControl_put;
extern uint32_t ut_BPilotControl_put_ctr;
void ut_IlPutTxBPilotControl(uint8_t val);

extern uint8_t ut_NPilotStatus_put;
extern uint32_t ut_NPilotStatus_put_ctr;
void ut_IlPutTxNPilotStatus(uint8_t val);

extern uint8_t ut_NProxStatus_put;
extern uint32_t ut_NProxStatus_put_ctr;
void ut_IlPutTxNProxStatus(uint8_t val);

extern uint8_t ut_BIsChargerCapableToCharge_put;
extern uint32_t ut_BIsChargerCapableToCharge_put_ctr;
void ut_IlPutTxBIsChargerCapableToCharge(uint8_t val);

extern int16_t ut_VPackBMS_put;
extern uint32_t ut_VPackBMS_put_ctr;
void ut_IlPutTxVPackBMS(int16_t val);

extern uint8_t ut_BIsTetheredToCharger_put;
extern uint32_t ut_BIsTetheredToCharger_put_ctr;
void ut_IlPutTxBIsTetheredToCharger(uint8_t val);

extern uint8_t ut_BIsTetheredToNothing_put;
extern uint32_t ut_BIsTetheredToNothing_put_ctr;
void ut_IlPutTxBIsTetheredToNothing(uint8_t val);

extern uint8_t ut_BIsChargerReady_put;
extern uint32_t ut_BIsChargerReady_put_ctr;
void ut_IlPutTxBIsChargerReady(uint8_t val);

extern uint8_t ut_Additional_BMS_Errors_Chg_1_put;
extern uint32_t ut_Additional_BMS_Errors_Chg_1_put_ctr;
void ut_IlPutTxAdditional_BMS_Errors_Chg_1(uint8_t val);

extern uint8_t ut_Additional_BMS_Errors_Chg_3_put;
extern uint32_t ut_Additional_BMS_Errors_Chg_3_put_ctr;
void ut_IlPutTxAdditional_BMS_Errors_Chg_3(uint8_t val);

extern uint8_t ut_Additional_BMS_Errors_Chg_2_put;
extern uint32_t ut_Additional_BMS_Errors_Chg_2_put_ctr;
void ut_IlPutTxAdditional_BMS_Errors_Chg_2(uint8_t val);

extern uint8_t ut_BCtcNegVehHs_put;
extern uint32_t ut_BCtcNegVehHs_put_ctr;
void ut_IlPutTxBCtcNegVehHs(uint8_t val);

extern uint8_t ut_NChgLastResult_put;
extern uint32_t ut_NChgLastResult_put_ctr;
void ut_IlPutTxNChgLastResult(uint8_t val);

extern uint8_t ut_NChgState_put;
extern uint32_t ut_NChgState_put_ctr;
void ut_IlPutTxNChgState(uint8_t val);

extern uint8_t ut_BCtcPosChgLs_put;
extern uint32_t ut_BCtcPosChgLs_put_ctr;
void ut_IlPutTxBCtcPosChgLs(uint8_t val);

extern uint8_t ut_NCcoState_put;
extern uint32_t ut_NCcoState_put_ctr;
void ut_IlPutTxNCcoState(uint8_t val);

extern uint8_t ut_BCtcNegVehLs_put;
extern uint32_t ut_BCtcNegVehLs_put_ctr;
void ut_IlPutTxBCtcNegVehLs(uint8_t val);

extern uint8_t ut_BCtcPosChgHs_put;
extern uint32_t ut_BCtcPosChgHs_put_ctr;
void ut_IlPutTxBCtcPosChgHs(uint8_t val);

extern int16_t ut_VChgPosBMS_put;
extern uint32_t ut_VChgPosBMS_put_ctr;
void ut_IlPutTxVChgPosBMS(int16_t val);

extern uint8_t ut_BCtcNegChgHs_put;
extern uint32_t ut_BCtcNegChgHs_put_ctr;
void ut_IlPutTxBCtcNegChgHs(uint8_t val);

extern int16_t ut_VChgNegBMS_put;
extern uint32_t ut_VChgNegBMS_put_ctr;
void ut_IlPutTxVChgNegBMS(int16_t val);

extern uint8_t ut_BCtcNegChgLs_put;
extern uint32_t ut_BCtcNegChgLs_put_ctr;
void ut_IlPutTxBCtcNegChgLs(uint8_t val);

extern int32_t ut_IChargerBMS_put;
extern uint32_t ut_IChargerBMS_put_ctr;
void ut_IlPutTxIChargerBMS(int32_t val);

extern uint16_t ut_VCellMinBMS_put;
extern uint32_t ut_VCellMinBMS_put_ctr;
void ut_IlPutTxVCellMinBMS(uint16_t val);

extern uint16_t ut_VCellMaxBMS_put;
extern uint32_t ut_VCellMaxBMS_put_ctr;
void ut_IlPutTxVCellMaxBMS(uint16_t val);

extern int16_t ut_VPilotPin_put;
extern uint32_t ut_VPilotPin_put_ctr;
void ut_IlPutTxVPilotPin(int16_t val);

extern uint8_t ut_NBMSState_put;
extern uint32_t ut_NBMSState_put_ctr;
void ut_IlPutTxNBMSState(uint8_t val);

extern uint16_t ut_NBMSChargeInhibitMask_put;
extern uint32_t ut_NBMSChargeInhibitMask_put_ctr;
void ut_IlPutTxNBMSChargeInhibitMask(uint16_t val);

extern int16_t ut_VProxPin_put;
extern uint32_t ut_VProxPin_put_ctr;
void ut_IlPutTxVProxPin(int16_t val);

extern uint8_t ut_NTermReason_put;
extern uint32_t ut_NTermReason_put_ctr;
void ut_IlPutTxNTermReason(uint8_t val);

extern uint8_t ut_NBMS_Diag_15_put[8];
extern uint32_t ut_NBMS_Diag_15_put_ctr;
void ut_IlPutTxNBMS_Diag_15(uint8_t * val);

extern uint8_t ut_NBMS_Diag_16_put[8];
extern uint32_t ut_NBMS_Diag_16_put_ctr;
void ut_IlPutTxNBMS_Diag_16(uint8_t * val);

extern uint8_t ut_NBMS_Diag_14_put[8];
extern uint32_t ut_NBMS_Diag_14_put_ctr;
void ut_IlPutTxNBMS_Diag_14(uint8_t * val);

extern uint8_t ut_NBMS_Diag_13_put[8];
extern uint32_t ut_NBMS_Diag_13_put_ctr;
void ut_IlPutTxNBMS_Diag_13(uint8_t * val);

extern uint8_t ut_NBMS_Diag_10_put[8];
extern uint32_t ut_NBMS_Diag_10_put_ctr;
void ut_IlPutTxNBMS_Diag_10(uint8_t * val);

extern uint8_t ut_NBMS_Diag_09_put[8];
extern uint32_t ut_NBMS_Diag_09_put_ctr;
void ut_IlPutTxNBMS_Diag_09(uint8_t * val);

extern uint8_t ut_NBMS_Diag_12_put[8];
extern uint32_t ut_NBMS_Diag_12_put_ctr;
void ut_IlPutTxNBMS_Diag_12(uint8_t * val);

extern uint8_t ut_NBMS_Diag_11_put[8];
extern uint32_t ut_NBMS_Diag_11_put_ctr;
void ut_IlPutTxNBMS_Diag_11(uint8_t * val);

extern uint8_t ut_NBMS_Diag_06_put[8];
extern uint32_t ut_NBMS_Diag_06_put_ctr;
void ut_IlPutTxNBMS_Diag_06(uint8_t * val);

extern int16_t ut_VVehNegBMS_put;
extern uint32_t ut_VVehNegBMS_put_ctr;
void ut_IlPutTxVVehNegBMS(int16_t val);

extern int16_t ut_VVehPosBMS_put;
extern uint32_t ut_VVehPosBMS_put_ctr;
void ut_IlPutTxVVehPosBMS(int16_t val);

extern uint8_t ut_NBMS_Diag_08_put[8];
extern uint32_t ut_NBMS_Diag_08_put_ctr;
void ut_IlPutTxNBMS_Diag_08(uint8_t * val);

extern uint8_t ut_NBMS_Diag_07_put[8];
extern uint32_t ut_NBMS_Diag_07_put_ctr;
void ut_IlPutTxNBMS_Diag_07(uint8_t * val);

/* Rx groups header start */
/* Tx groups header start */
