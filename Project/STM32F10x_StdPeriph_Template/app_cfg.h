/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-LK1 Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.10
* Programmer(s) : BH3NVN
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/


/**********************************************************************************************************
*                                       uC/OS-II任务的优先级
**********************************************************************************************************/
#define  App_SysTimeCheck_PRIO                             4
#define  APP_MODBUS_POLL_PRIO                              3	
#define  APP_MODBUS_MASTER_POLL_PRIO                       2
#define  App_SlaveRW_PRIO																	16
#define  APP_TASK_START_PRIO                               22								  
#define  APP_LOGIN_PRIO                                    6
#define		App_UpdateAnalogInput_PRIO												10
#define		App_MTankControl_PRIO															7
#define		App_UpdateDigitInput_PRIO												  8
#define		App_UpdateOutput_PRIO															9
#define		App_CO2Control_PRIO																11
#define		App_PHControl_PRIO																17
#define		App_AirTControl_PRIO															13
#define		App_H2OTControl_PRIO															14
#define		App_WateringCtrl_PRIO															15
#define		App_LEDControl_PRIO																5
//#define		App_HumControl_PRIO															  12
#define		App_Alarm_PRIO																	  18
#define		App_CheckRightHum_PRIO															19
#define		App_CheckLeftHum_PRIO															20
#define		App_CheckMidHum_PRIO															21


/**********************************************************************************************************
*                                       uC/OS-II任务的堆栈大小
*                             Size of the task stacks (# of OS_STK entries)
**********************************************************************************************************/
#define  App_SysTimeCheck_SIZE                           512
#define  APP_MODBUS_POLL_SIZE   								     		 512
#define	 App_MY_STK_SIZE															   512
#define  APP_TASK_START_STK_SIZE                         512
#define  APP_LOGIN_SIZE                                  512



/*
*********************************************************************************************************
*                                                  LIB
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED

#endif


static void eMBPollExecute( void );
static  void App_TaskManager(void* p_arg);
