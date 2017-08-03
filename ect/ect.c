/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    ect.c
 *
 * description:  
 *  Contains ect initiate for sent.
 * 
 *
 
 * revision history:
 *
 * Date        Author           Description
 * 2016-Feb-2  Jun Wu           initial version
 * 2017-Mar_16 Jun Wu			Add ECT2, ECT1 as SENT1, ECT2 as SENT2
 *
 *****************************************************************************/
/* MODULE ECT. */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "regs.h"
#include "Main_isr_types.h"
#include "isr.h"
#include "ect.h"
#include "platform_test.h"
#include "gate.h"       /* XGATE control register defiitions */

/*****************************************************************************
 * MACROS
 *****************************************************************************/

/*****************************************************************************
 * PUBLIC DATA
 *****************************************************************************/
uint16_t PWM_In_Freq = 0u;
uint16_t PWM_In_Duty = 0u;
uint16_t PWM_In_Timeout_CNT = 0u;
uint8_t PWM_In_Freq_Cur = 0u;
uint8_t PWM_In_Freq_Pre = 0u;
uint8_t PWM_In_Freq_delta = 0u;
uint8_t PWM_In_Duty_Cur = 0u;
uint8_t PWM_In_Duty_Pre = 0u;
uint8_t PWM_In_Duty_delta = 0u;
uint8_t PWMIN_Freq_Unvalid_Cnt = 0u;
uint8_t PWMIN_Duty_Unvalid_Cnt = 0u;
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
PRIVATE bool_t EnUser =FALSE;          /* Enable/Disable device by user */
PRIVATE uint16_t Cap3_value_cur = 0u;
PRIVATE uint16_t Cap3_value_cur_pre = 0u;
PRIVATE uint32_t Cap3_capture[2] = {0u};
uint8_t Cap3_cnt = 0u;
PRIVATE uint32_t ect_overflow_cnt = 0u;

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
void Cap3_OnCapture(void);
/*
** ===================================================================
**     Method      :  Cap1_Enable (bean Capture)
**
**     Description :
**         This method enables the bean - it starts the capture. Events
**         may be generated (<DisableEvent>/<EnableEvent>).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
uint8_t Cap1_Enable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    EnUser = TRUE;                     /* If yes then set the flag "device enabled" */
  /* ECT_TFLG1: C7F=0,C6F=0,C5F=0,C4F=0,C3F=0,C2F=0,C1F=1,C0F=0 */
    TFLG1 |= ECT_Channel1_MASK;    /* Reset interrupt request flag */
	TFLG2|=ECT_Overflow_MASK;		/* Reset overflow interrupt request flag */
	
	TIE |= ECT_Channel1_MASK;          /* channel1 Enable interrupt */
	TSCR2 |= ECT_Overflow_MASK;			/* Enable overflow interrupt */
	
  /* TCTL4: EDG1B=1 */
    TCTL4 = 0x08;                    /* Enable capture function */
  }
  return 0;                       /* OK */
}

/*
** ===================================================================
**     Method      :  Cap1_Disable (bean Capture)
**
**     Description :
**         This method disables the bean - it stops the capture. No
**         events will be generated.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
uint8_t Cap1_Disable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    EnUser = FALSE;                    /* If yes then set the flag "device disabled" */
    TIE &= ~ECT_Channel1_MASK;                   /* Disable interrupt */
	TSCR2 &= ~ECT_Overflow_MASK;
  /* TCTL4: EDG1B=0,EDG1A=0 */
    TCTL4 = 0x00;                   /* Disable capture function */
  }
  return 0;                       /* OK */
}

/*
** ===================================================================
**     Method      :  Cap1_Init (bean Capture)
**
**     Description :
**         Initializes the associated peripheral(s) and the beans 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void ect_init(void)
{  
  	TSCR1=0x90; 								// TEN:定时器使能， TFFCA：定时器标志快速清除选择位
	TSCR2=0X00; 								// TOI中断不使能即TOF溢出不能引起中断，1分频：50m, 0.02us记一个数
	
	TIOS &= ~ECT_Channel1_MASK; 				// 设置PT1为输入捕捉
	TIOS &= ~ECT_Channel2_MASK; 				// 设置PT1为输入捕捉
	TIOS &= ~ECT_Channel3_MASK; 				// 设置PT3为输入捕捉

	TCTL4 = ECT_Channel1_Falling_Edge_MASK;  	// 1通道下降沿捕捉 
	TCTL4 |= ECT_Channel2_Falling_Edge_MASK; 	// 2通道下降沿捕捉 
	TCTL4 |= ECT_Channel3_Rising_Edge_MASK;  	// 3通道上升沿捕捉

	DLYCT = 0x00;	    						// 延迟控制功能禁止
	ICOVW = 0x00;	    						// 对应的寄存器允许被覆盖;  NOVWx = 1, 对应的寄存器不允许覆盖
	ICSYS = 0x00;	    						// 保持寄存器使能，其填充后标志位才置位，队列模式

	TSCR2 |= ECT_Overflow_MASK;         		// 开启溢出中断
	TIE |= ECT_Channel1_MASK;     				// 1通道标志位可引起中断
	TIE |= ECT_Channel2_MASK;     				// 2通道标志位可引起中断
	TIE |= ECT_Channel3_MASK;     				// 3通道标志位可引起中断
	TFLG1 = 0xff;	    						// 清除各IC/OC中断标志位
	TFLG2 = 0xff;   							// 清除自由定时器中断标志位	
	
	/* enable the flash fault interrupt vector */
	//(void)set_interrupt_level(VECT_OFFSET_ECT_Overflow, INT_LVL_DEFAULT, INT_CPU_DEST);
    
	// (void)set_interrupt_level(VECT_OFFSET_ECT_Channel1Interrupt, INT_LVL_DEFAULT, INT_CPU_DEST);
	 (void)set_interrupt_level(VECT_OFFSET_ECT_Channel2Interrupt, 4, INT_CPU_DEST);

	/* interrupt to xgate */
	(void)set_interrupt_level(ECT_Channel1_Xg_Address_MASK * 2, INT_LVL_DEFAULT, INT_XGATE_DEST);  
	//(void)set_interrupt_level(ECT_Channel2_Xg_Address_MASK * 2, INT_LVL_DEFAULT, INT_XGATE_DEST); 
}

/*
** ===================================================================
**     Method      :  Interrupt (bean Capture)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes of the bean event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

/* Move to xgate init */
// INTERRUPT void ECT_Channel1_Interrupt(void)
// {
//   /* ECT_TFLG1: C7F=0,C6F=0,C5F=0,C4F=0,C3F=0,C2F=0,C1F=1,C0F=0 */
//   TFLG1 |= ECT_Channel1_MASK;                       /* Reset interrupt request flag */
//   Cap1_OnCapture();                    /* Invoke user event */
// }
// 
// INTERRUPT void ECT_Channel2_Interrupt(void)
// {
//   /* ECT_TFLG2: C7F=0,C6F=0,C5F=0,C4F=0,C3F=0,C2F=0,C1F=1,C0F=0 */
//   TFLG1 |= ECT_Channel2_MASK;                       /* Reset interrupt request flag */
//   Cap2_OnCapture();                    /* Invoke user event */
// }

INTERRUPT void ECT_Channel3_Interrupt(void)
{
  /* ECT_TFLG1: C7F=0,C6F=0,C5F=0,C4F=0,C3F=0,C2F=0,C1F=1,C0F=0 */
  TFLG1 |= ECT_Channel3_MASK;                       /* Reset interrupt request flag */
  Cap3_OnCapture();                    /* Invoke user event */
}


/*
** ===================================================================
**     Method      :  Interrupt (bean ECT overflow)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes of the bean event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
INTERRUPT void ECT_Overflow_Interrupt(void)
{
	TFLG2 |= ECT_Overflow_MASK; 
	ect_overflow_cnt++;
}

/* Cap3_OnCapture */

void Cap3_OnCapture(void)
{
	Cap3_value_cur = TC3;
	Cap3_capture[Cap3_cnt] =  65536*ect_overflow_cnt +Cap3_value_cur - Cap3_value_cur_pre;
	
		
	Cap3_value_cur_pre = Cap3_value_cur;
	
	Cap3_cnt++;
	
	if(Cap3_cnt==1u)
	{
		TCTL4 &= ~ECT_Channel3_Rising_Edge_MASK; 
		TCTL4 |= ECT_Channel3_Falling_Edge_MASK; //改成3通道下降沿捕捉
	}
	else if(Cap3_cnt == 2u)
	{
		PWM_In_Freq = (uint16_t)((50000000 + (uint32_t)(Cap3_capture[0]+Cap3_capture[1])/2)/(uint32_t)(Cap3_capture[0]+Cap3_capture[1]));

		PWM_In_Duty= (uint16_t)(((uint32_t)Cap3_capture[1]*100u + (uint32_t)(Cap3_capture[0]+Cap3_capture[1])/2)/(uint32_t)(Cap3_capture[0]+Cap3_capture[1]));
		
		TCTL4 &= ~ECT_Channel3_Falling_Edge_MASK; 
		TCTL4 |= ECT_Channel3_Rising_Edge_MASK; //改成3通道上升沿捕捉
		
		Cap3_cnt=0u;
	}
	
	ect_overflow_cnt = 0u;
	PWM_In_Timeout_CNT++;
}

void pwmin_read_inputs(void)
{	
	if(PWM_In_Timeout_CNT > 0u)
	{
		PWM_In_Freq_Cur = (uint8_t)((PWM_In_Freq + 50)/100);            /* 0.1K every count */
		PWM_In_Duty_Cur = (uint8_t)PWM_In_Duty;
		
		/* filter some unvalid value for Freq */
		if(PWM_In_Freq_Cur > PWM_In_Freq_Pre)
		{
			PWM_In_Freq_delta = PWM_In_Freq_Cur - PWM_In_Freq_Pre;
		}
		else
		{
			PWM_In_Freq_delta = PWM_In_Freq_Pre - PWM_In_Freq_Cur; 
		}
		
		if(PWM_In_Freq_delta > 1)                                    /* gap over 0.1k */
		{
			PWMIN_Freq_Unvalid_Cnt++;
			
			if(PWMIN_Freq_Unvalid_Cnt < 4)							/* if unvaild value continuously over 3 times, then take it as vaild value */
			{
				PWM_In_Freq_Cur = PWM_In_Freq_Pre;                   
			}			
		}
		else
		{
			PWMIN_Freq_Unvalid_Cnt = 0u;
		}
		
		/* filter some unvalid value for Duty */
		if(PWM_In_Duty_Cur > PWM_In_Duty_Pre)
		{
			PWM_In_Duty_delta = PWM_In_Duty_Cur - PWM_In_Duty_Pre;
		}
		else
		{
			PWM_In_Duty_delta = PWM_In_Duty_Pre - PWM_In_Duty_Cur; 
		}
		
		if(PWM_In_Duty_delta > 5)                                    /* gap over 5% */
		{
			PWMIN_Duty_Unvalid_Cnt++;
			
			if(PWMIN_Duty_Unvalid_Cnt < 4)							/* if unvaild value continuously over 3 times, then take it as vaild value */
			{
				PWM_In_Duty_Cur = PWM_In_Duty_Pre;                   
			}			
		}
		else
		{
			PWMIN_Duty_Unvalid_Cnt = 0u;
		}
		
		
		PWM_In_Freq_Pre = PWM_In_Freq_Cur;
		PWM_In_Duty_Pre = PWM_In_Duty_Cur;
		PWM_In_Timeout_CNT = 0u;                              
	}
	else                                                               /* if time out */
	{
		PWM_In_Freq_Cur = 0u;
		PWM_In_Duty_Cur = 0u;
		PWM_In_Freq_Pre = 0u;
		PWM_In_Duty_Pre = 0u;
	}
	
	
}
/* END ect. */
