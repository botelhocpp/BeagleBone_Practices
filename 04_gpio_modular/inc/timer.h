#ifndef TIMER_H_
#define TIMER_H_
/*
 * =====================================================================================
 *
 *       Filename:  timer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31/05/2018 19:52:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include "soc_AM335x.h"
#include "hw_types.h"
#include "hw_dmtimer.h"
#include "hw_cm_per.h"
#include "hw_cm_dpll.h"
#include "interrupt.h"
#include "clock_module.h"
#include "control_module.h"

/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
*******************************************************************************/
#define TIMER_INITIAL_COUNT             (0xFFFFA23Fu)
//#define TIMER_RLD_COUNT                 (0xFF000000u)

/* The Input clock is selected as 24MHz. So for 1ms set the count to 0x5DC0. 
 *If the input clock is changed to different source this value has to be updated 
 *accordingly.	
*/
#define TIMER_1MS_COUNT                 (0x5DC0u) 
#define TIMER_OVERFLOW                  (0xFFFFFFFFu)

#define DELAY_USE_INTERRUPTS            1

/*****************************************************************************/
/*
** Values that can be passed to DMTimerPreScalerClkEnable as ptv so as to derive
** Pre-Scalar clock from timer clock.
*/
/* Value used to divide timer clock by 2 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_2      ((0 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 4 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_4	    ((1 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 8 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_8	    ((2 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 16 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_16	    ((3 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 32 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_32	    ((4 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 64 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_64	    ((5 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 128 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_128    ((6 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 256 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_256    ((7 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/******************************************************************************/
/*
** Values that can be passed to DMTimerModeConfigure as timerMode.
*/
/* Value used to enable the timer in one-shot and compare mode */
#define DMTIMER_ONESHOT_CMP_ENABLE          (DMTIMER_TCLR_CE)

/* Value used to enable the timer only in one-shot mode */
#define DMTIMER_ONESHOT_NOCMP_ENABLE        (0x0000000)

/* Value used to enable the timer in auto-reload and compare mode */
#define DMTIMER_AUTORLD_CMP_ENABLE          (DMTIMER_TCLR_AR | DMTIMER_TCLR_CE)

/* Value used to enable the timer only in auto-reload mode */
#define DMTIMER_AUTORLD_NOCMP_ENABLE        (DMTIMER_TCLR_AR)

/******************************************************************************/
/*
** Values that can be passed to DMTimerGPOConfigure as gpoCfg.
*/
/* Value used to drive 0 on PORGPOCFG pin */
#define DMTIMER_GPO_CFG_0		    (DMTIMER_TCLR_GPO_CFG_DRIVE0)

/* Value used to drive 1 on PORGPOCFG pin */
#define DMTIMER_GPO_CFG_1		    (DMTIMER_TCLR_GPO_CFG_DRIVE1 << \
					     DMTIMER_TCLR_GPO_CFG_SHIFT)

/******************************************************************************/
/*
** Values that can be passed to DMTimerIntStatusClear/DMTimerIntRawStatusSet/
** as intFlags. Also these values can be used while checking the status got from 
** DMTimerIntRawStatusGet/DMTimerIntStatusGet.
** Any combination is also followed.
** Example- (DMTIMER_INT_TCAR_IT_FLAG | DMTIMER_INT_OVF_IT_FLAG)
*/
/* Value used for capture event of DMTimer */
#define DMTIMER_INT_TCAR_IT_FLAG             (DMTIMER_IRQSTATUS_RAW_TCAR_IT_FLAG)

/* Value used for overflow event of DMTimer */
#define DMTIMER_INT_OVF_IT_FLAG              (DMTIMER_IRQSTATUS_RAW_OVF_IT_FLAG) 

/* Value used for Match event of DMTimer */
#define DMTIMER_INT_MAT_IT_FLAG              (DMTIMER_IRQSTATUS_RAW_MAT_IT_FLAG)

/******************************************************************************/
/*
** Values that can be passed to DMTimerIntEnable/DMTimerIntDisable as intFlags.
** Also these values can be used while checking the status got from 
** DMTimerIntEnableGet.
** Any combination is also followed.
** Example- (DMTIMER_INT_TCAR_EN_FLAG | DMTIMER_INT_OVF_EN_FLAG)
*/
/* Value used for capture event of DMTimer */
#define DMTIMER_INT_TCAR_EN_FLAG             (DMTIMER_IRQENABLE_SET_TCAR_EN_FLAG)

/* Value used for overflow event of DMTimer */
#define DMTIMER_INT_OVF_EN_FLAG              (DMTIMER_IRQENABLE_SET_OVF_EN_FLAG)

/* Value used for Match event of DMTimer */
#define DMTIMER_INT_MAT_EN_FLAG              (DMTIMER_IRQENABLE_SET_MAT_EN_FLAG)

/******************************************************************************/
/*
** Values that can be passed to DMTimerResetConfigure as rstOption.
*/
/* Value used to enable software reset for DMTimer */
#define DMTIMER_SFT_RESET_ENABLE             (DMTIMER_TSICR_SFT_RESETENABLE)

/* Value used to disable software reset for DMTimer */
#define DMTIMER_SFT_RESET_DISABLE            (DMTIMER_TSICR_SFT)

/******************************************************************************/
/*
** Values that can be used while checking the status received from 
** DMTimerIsResetDone.
*/
/* Value used to check whether reset is done */
#define DMTIMER_IS_RESET_DONE                (DMTIMER_TIOCP_CFG_SOFTRESET_DONE)

/* Value used to check whether reset is ongoing */
#define DMTIMER_IS_RESET_ONGOING             (DMTIMER_TIOCP_CFG_SOFTRESET_ONGOING)

/******************************************************************************/
/*
** Values that can be passed to DMTimerPostedModeConfig as postMode.
*/
/* Value used to enable the posted mode of operation */
#define DMTIMER_POSTED                       (DMTIMER_TSICR_POSTED)

/* Value used to disable the posted mode of operation */
#define DMTIMER_NONPOSTED                    (DMTIMER_TSICR_POSTED_INACTIVE)

/******************************************************************************/
/*
** Values that can be used while checking status received from
** DMTimerWritePostedStatusGet API.
*/
/* Value used to check the write posted condition for TMAR register */
#define DMTIMER_WRITE_POST_TMAR              (DMTIMER_TWPS_W_PEND_TMAR)

/* Value used to check the write posted condition for TTGR register */
#define DMTIMER_WRITE_POST_TTGR              (DMTIMER_TWPS_W_PEND_TTGR)

/* Value used to check the write posted condition for TLDR register */
#define DMTIMER_WRITE_POST_TLDR              (DMTIMER_TWPS_W_PEND_TLDR)

/* Value used to check the write posted condition for TCRR register */
#define DMTIMER_WRITE_POST_TCRR              (DMTIMER_TWPS_W_PEND_TCRR)

/* Value used to check the write posted condition for TCLR register */
#define DMTIMER_WRITE_POST_TCLR              (DMTIMER_TWPS_W_PEND_TCLR)


//unsigned int flagIsr;

void DMTimerModuleClkConfig(void );
void DMTimerCounterSet(unsigned int , unsigned int );
unsigned int DMTimerCounterGet(unsigned int ); 
void DMTimerEnable(unsigned int );
void DMTimerDisable(unsigned int );
void DMTimerReset(unsigned int ); 
void DMTimerIntEnable(unsigned int , unsigned int );
void DMTimerIntDisable(unsigned int , unsigned int );
void DMTimerSetUp(void );
void Delay(unsigned int );
void DMTimerIsr(void);

#endif //TIMER_H_
