/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    tmm.h
 *
 * description:  
 *  			tmm management.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Mar-2  Jun Wu           initial version
 * 2017-Mar-27  Jun Wu          Control two TMM
 *
 *****************************************************************************/
#if !defined(TMM_H)
#define      TMM_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "regs.h"
#include "vio.h"
#include "tmm_fm.h"
#include "smm.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/ 
#define TMM_Angle_Upper_MAX        269
#define TMM_Angle_Lower_MIN        -14
#define TMM_Sent_Upper_MAX         4095
#define TMM_Sent_Lower_MIN         0
#define TMM_UME_Default            259
#define TMM_LME_Default            -4
//#define TMM_UME_Default            122
//#define TMM_LME_Default            -4
#define TMM_UME_SENT_Default       3950
#define TMM_LME_SENT_Default       145
#define TMM_UAR_Default            250
#define TMM_LAR_Default            0
//#define TMM_UAR_Default            118
//#define TMM_LAR_Default            0
#define TMM_Speed_Default          50                             
#define TMM_Speed_MAX              180
#define TMM_Scaling_Speed          50
//#define TMM_Safe_Position          40

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef enum
{
	ref_unstart,
	ref_start,
	ref_succeed,
	ref_failed
} ref_run_status_e;

typedef enum
{
	m_stop,
	m_start,
	m_single,
	m_resetcutoff,
	loop_control_max
} close_loop_control_e;

struct tmm_endstop_s
{
	int16_t 	angle_upper_endstop;
	int16_t 	angle_lower_endstop;
	uint16_t	sent_upper_endstop;
	uint16_t	sent_lower_endstop;
};

typedef enum
{
    scaling_s_detect,
    scaling_to_UME,
    scaling_to_LME,
    scaling_to_UAR,
	scaling_to_LAR,
	scaling_stop_in_UE,         /* upper ends : UME or UAR */
	scaling_stop_in_LE,			/* low ends : LME or LAR */	
	scaling_to_Matrix,
	scaling_to_Target,
	scaling_shutdown
} scaling_state_e;

typedef enum
{
    Refrence_Run,
    Shift_Speed_AR,
    Shift_Speed_ME,
	Matrix_Run,
	PWM_To_ME_closeloop,
	To_Any_Target_Angle,
	To_Any_Target_Sent,
	PWM_To_ME_openloop,
	No_Operating
} tmm_operating_mode_e;

struct tmm_operating_s
{
	bool_t tmm_operating_start;
	tmm_operating_mode_e tmm_operating_mode;
	int16_t input_angle_ume;
	int16_t input_angle_lme;
	int16_t input_angle_uar;
	int16_t input_angle_lar;
	uint16_t stop_duration;         /* stop in ME or AR time duration, 1cnt = 1s */
	uint8_t matrix_sel;
	uint8_t pwm_dcmr_closeloop;
	int8_t pwm_ducmr_openloop;
	int16_t target_angle;
	uint16_t target_sent;
	int16_t target_speed;
};

struct tmm_rx_s
{
	uint8_t ecu_limitinput[8];
	uint8_t ecu_regelparainput[8];
	uint8_t ecu_angleinput[8];
	uint8_t ecu_sentmodeinput[8];
	uint8_t can2ana_rx2[8];
	uint8_t can2ana_rx1[8];
	uint8_t lin_rscv_input_b[4];
	uint8_t lin_rscv_input_a[4];
	uint8_t rx_lin_seq_confg1[8];
	uint8_t rx_lin_seq_confg2[8];
	uint8_t rx_lin_seq_sel[8];
	uint8_t rx_tmm_seq_confg[8];
	data_validity_e ecu_limitinput_valid;
	data_validity_e ecu_regelparainput_valid;
	data_validity_e ecu_angleinput_valid;
	data_validity_e ecu_sentmodeinput_valid;
	data_validity_e can2ana_rx2_valid;
	data_validity_e can2ana_rx1_valid;
	data_validity_e lin_rscv_input_b_valid;
	data_validity_e lin_rscv_input_a_valid;
};
/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
/* Flag: cancel PID for PWM control mode */
extern bool_t Flag_Cancel_PID[sent_channel_max];

/* Flag: enable angle control */
extern bool_t Flag_TMM_Angle_Control[sent_channel_max];

/* Flag: succeed to target position */
extern bool_t Flag_Succeed_To_Target[sent_channel_max];

/* tmm target angle */
extern int16_t TMM_Angle_Tar[sent_channel_max];

/* TMM target speed */
extern int16_t TMM_Speed_Tar[sent_channel_max];

/* tmm angle deviation between target and current angle */
extern uint16_t TMM_Angle_Deviation[sent_channel_max];

/* tmm state machine */
extern scaling_state_e TMM_Scaling_State[sent_channel_max];

/* TMM operating mode */
extern struct tmm_operating_s TMM_Operating[sent_channel_max];

/* TMM endstop angle */
extern struct tmm_endstop_s TMM_Endstop_Cur[sent_channel_max];



/* cyclic cnter */
extern uint16_t Cntr_Endpos_Mea[sent_channel_max];	

/* Flag: if the motor stalled */
extern bool_t Flag_Tmm_Stalling[sent_channel_max];	

/* Flag: refence run status */
extern ref_run_status_e Flag_Ref_Run_Status[sent_channel_max];					  


extern uint8_t TMM_Matrix_Num ;

extern struct tmm_rx_s TMM_RX_CAN;
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/ 
void tmm_init(void);
void tmm_update(void);
void tmm_reset_factors(sent_channel_e sent_channel);
void tmm_scaling_shutdown(sent_channel_e sent_channel);
#endif /* TMM_H */
