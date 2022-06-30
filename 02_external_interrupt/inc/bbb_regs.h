/**
 * @file bbb_regs.h
 * @author Francisco Helder (helderhdw@gmail.com)
 * @brief Beaglenone Black configuration registers.
 * @version 1.0
 * @date 2022-06-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _BBB_REGS_H_
#define _BBB_REGS_H_

/* Interrupt */
#define INTC_BASE       						0x48200000
#define INTC_SIR_IRQ    						0x48200040
#define INTC_CONTROL    						0x48200048
#define INTC_IDLE       						0x48200050
#define INTC_ILR_32        					0x48200180 // 0x48200100 + 0x4×0x20
#define INTC_ILR_33        					0x48200184 // 0x48200100 + 0x4×0x21
#define INTC_ILR_98        					0x48200288 // 0x48200100 + 0x4×0x62
#define INTC_ILR_99        					0x4820028C // 0x48200100 + 0x4×0x63
#define INTC_MIR_CLEAR0 						0x48200088
#define INTC_MIR_CLEAR1 						0x482000A8
#define INTC_MIR_CLEAR2 						0x482000C8
#define INTC_MIR_CLEAR3 						0x482000E8
#define INTC_SYSCONFIG						0x48200010
#define INTC_SYSSTATUS						0x48200014
#define INTC_THRESHOLD						0x48200068
	
/* UART0 */
#define UART0_BASE 							0x44E09000
#define UART0_RHR  							0x44E09000
#define UART0_THR  							0x44E09000
#define UART0_LSR  							0x44E09014

/* Watch Dog Timer */
#define  WDT_BASE 							0x44E35000
#define  WDT_WSPR 							0x44E35048
#define  WDT_WWPS 							0x44E35034

/* Clock */
#define CM_PER_GPIO1_CLKCTRL                      0x44E000AC
#define CM_PER_GPIO2_CLKCTRL                      0x44E000B0
#define CM_PER_RTC_RTC_CLKCTRL                    0x44E00800
#define CM_PER_RTC_CLKSTCTRL                      0x44E00804
#define CM_PER_TIMER7_CLKCTRL                     0x44E0007C
#define SOC_CM_DPLL_CLKSEL_TIMER7_CLK             0x44E00504

/* pin name */
#define CM_PER_BASE                               0x44E10000
#define CM_PER_GPMCA5_REGS                        0x44E10854
#define CM_PER_GPMCA6_REGS                        0x44E10858
#define CM_PER_GPMCA7_REGS                        0x44E1085C
#define CM_PER_GPMCA8_REGS                        0x44E10860
#define CM_PER_GPMCAD12_REGS                      0x44E10830
#define CM_PER_GPMCAD14_REGS                      0x44E10838
#define CM_PER_GPMCCLK_REGS                       0x44E1088C

/* GPIO1 */
#define GPIO1_BASE                                0x4804C000
#define GPIO1_IRQSTATUS_0				     0x4804C02C
#define GPIO1_IRQSTATUS_1					0x4804C030
#define GPIO1_IRQSTATUS_SET_0  				0x4804C034
#define GPIO1_IRQSTATUS_SET_1  				0x4804C038
#define GPIO1_OE            					0x4804C134
#define GPIO1_DATAIN  						0x4804C138
#define GPIO1_DATAOUT                             0x4804C13C
#define GPIO1_RISINGDETECT  					0x4804C148
#define GPIO1_CLEARDATAOUT  					0x4804C190
#define GPIO1_SETDATAOUT   				 	0x4804C194

/* GPIO2 */
#define GPIO2_BASE                                0x481AC000
#define GPIO2_IRQSTATUS_0				     0x481AC02C
#define GPIO2_IRQSTATUS_1					0x481AC030
#define GPIO2_IRQSTATUS_SET_0  				0x481AC034
#define GPIO2_IRQSTATUS_SET_1  				0x481AC038
#define GPIO2_OE            					0x481AC134
#define GPIO2_DATAIN  						0x481AC138
#define GPIO2_DATAOUT                             0x481AC13C
#define GPIO2_RISINGDETECT  					0x481AC148
#define GPIO2_CLEARDATAOUT  					0x481AC190
#define GPIO2_SETDATAOUT   				 	0x481AC194



/* The Input clock is selected as 24MHz. So for 1ms set the count to 0x5DC0. 
 *If the input clock is changed to different source this value has to be updated 
 *accordingly.	
*/
#define TIMER_1MS_COUNT                 		0x5DC0u
#define TIMER_OVERFLOW                  		0xFFFFFFFFu



#endif //__BB_REGS_H__

