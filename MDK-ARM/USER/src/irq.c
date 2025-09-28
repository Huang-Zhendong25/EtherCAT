/*
 * @Author: YXY
 * @Date: 2020-08-27 10:34:29
 * @LastEditTime: 2020-08-27 11:00:36
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \undefinedu:\Bao Huang\Documents\Keil\Data_Ethernet_CAT\UTILS\Src\irq.c
 */
/* Includes ------------------------------------------------------------------*/
#include "irq.h"
#include "stm32f1xx_it.h"
//#include "led.h"
/* Variables -----------------------------------------------------------------*/



/* Functions  ----------------------------------------------------------------*/
/* ESC_INT  ----------------------*/
void escInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/* EXTI interrupt init*/
	HAL_NVIC_ClearPendingIRQ(EXTI4_IRQn);
  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
	//The interrupts is enabled by the SSC itself, rather than being enabled manually from the beginning.
  //HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

void escEnable(void)
{
	EXTI->IMR |= ((uint32_t)GPIO_PIN_4);
}
void escDisable(void)
{
	EXTI->IMR &= ~((uint32_t)GPIO_PIN_4);
}

/* SYNC0  ----------------------*/
void sync0Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	/*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* EXTI interrupt init*/
	HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  //HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void sync0Enable(void)
{
	EXTI->IMR |= ((uint32_t)GPIO_PIN_11);
}

void sync0Disable(void)
{
	EXTI->IMR &= ~((uint32_t)GPIO_PIN_11);
}




/* SYNC1  ----------------------*/

void sync1Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	/*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/* EXTI interrupt init*/
	HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  //HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void sync1Enable(void)
{
	EXTI->IMR |= ((uint32_t)GPIO_PIN_12);
}

void sync1Disable(void)
{
	EXTI->IMR &= ~((uint32_t)GPIO_PIN_12);
}




/* EXTI Callback  ----------------------*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
}

void F_EXTI15_10_IRQHandler0 (void)
{
	SYNC0_Handler();
}
void F_EXTI15_10_IRQHandler1 (void)
{
	SYNC1_Handler();
}
void F_EXTI4_IRQHandler (void)
{
	INT_ESC_Handler();
}

//__weak void INT_ESC_Handler(void)
//{

  /* NOTE: This function Should not be modified, when the callback is needed,
           the INT_ESC_Handler could be implemented in the user file
   */
//}

//__weak void SYNC0_Handler(void)
//{

  /* NOTE: This function Should not be modified, when the callback is needed,
           the INT_ESC_Handler could be implemented in the user file
   */
//}

//__weak void SYNC1_Handler(void)
//{

  /* NOTE: This function Should not be modified, when the callback is needed,
           the INT_ESC_Handler could be implemented in the user file
   */
//}
/* Clear EXIT Pending  ----------------------*/
void ClearEXITPending(void)
{
	__NVIC_ClearPendingIRQ(EXTI4_IRQn);
	//__NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	__NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
}


/************************ (C) COPYRIGHT YXY *****END OF FILE*******************/
