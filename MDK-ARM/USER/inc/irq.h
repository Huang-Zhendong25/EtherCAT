/*
 * @Author: YXY
 * @Date: 2020-08-27 10:34:39
 * @LastEditTime: 2020-08-27 10:54:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \undefinedu:\Bao Huang\Documents\Keil\Data_Ethernet_CAT\UTILS\Inc\irq.h
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __irq_H
#define __irq_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Define --------------------------------------------------------------------*/
/* interrupt  ----------------------*/
#define Enable_global_interrupt()            {__DMB();__enable_irq();}
//#define Enable_global_interrupt()            {__enable_irq();}
//#define Enable_global_interrupt()            {__set_BASEPRI(0x00);}
#define Disable_global_interrupt()           {__disable_irq();__DMB();}
//#define Disable_global_interrupt()           {__set_BASEPRI(0x0a);}
//#define Disable_global_interrupt()           {__disable_irq();}

/* Variables -----------------------------------------------------------------*/


/* Function prototypes -------------------------------------------------------*/
/* INIT  ----------------------*/
void escInit(void);
void sync0Init(void);
void sync1Init(void);
/* FUNC  ----------------------*/
void escEnable(void);
void escDisable(void);
void sync0Enable(void);
void sync0Disable(void);
void sync1Enable(void);
void sync1Disable(void);

/* EXTI Callback  ----------------------*/
void INT_ESC_Handler(void);
void SYNC0_Handler(void);
void SYNC1_Handler(void);
/* Clear EXIT Pending  ----------------------*/
void ClearEXITPending(void);

#ifdef __cplusplus
}
#endif
#endif /*__irq_H */
