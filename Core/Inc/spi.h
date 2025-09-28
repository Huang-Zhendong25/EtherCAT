/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

extern SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN Private defines */
#define SPI2_MODE		3
#define SPI2_USEMSB	1
/* USER CODE END Private defines */

void MX_SPI1_Init(void);
void MX_SPI2_Init(void);

/* USER CODE BEGIN Prototypes */
void spiEcatInit(void);
void spiEnable(void);
void spiDisable(void);
void spiSelect(void);
void spiDeselect(void);
uint8_t spiRxBufferNotEmpty(void);
uint8_t spiTxBufferEmpty(void);


uint8_t spiReadSingle(void);
void spiWriteSingle(uint8_t *data);

void spiRead(uint8_t *data);
void spiWrite(uint8_t data);
uint8_t spiReadWrite(uint8_t data);

//Change
uint8_t spiReadWrite3(uint8_t txData);

uint8_t SPI3_RXNE_State(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

