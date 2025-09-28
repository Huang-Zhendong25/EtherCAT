/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */
uint32_t txallowed = 1U;
/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}
/* SPI2 init function */
void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
  else if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
  else if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void spiEcatInit(void)
{
	/* PIN */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	/* SPI3 SEL and EEPROM */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	
	/*Configure GPIO pin : SPI_SEL_Pin Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/*Configure GPIO pin : EEPROM_LOADED_Pin */
	GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	/* SPI2 Master */
	hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.NSS = SPI_NSS_SOFT;
#if defined(SPI2_MODE) && (SPI2_MODE == 0)
	//MODE0
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
#endif
#if defined(SPI2_MODE) && (SPI2_MODE == 1)
	//MODE1
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
#endif
#if defined(SPI2_MODE) && (SPI2_MODE == 2)
	//MODE2
	hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
#endif
#if defined(SPI2_MODE) && (SPI2_MODE == 3)
	//MODE3
	hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
#endif

  //hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	//hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	//hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	//hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	//hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	//hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	//hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;

#if defined(SPI2_USEMSB) && (SPI2_USEMSB == 1)
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
#else
	hspi2.Init.FirstBit = SPI_FIRSTBIT_LSB;
#endif
  
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
//  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
//  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
	
	
	// Enable
	spiEnable();
	
	/* Set fiforxthreshold according the reception data length: 8bit */
//  SET_BIT(hspi2.Instance->CR2, SPI_RXFIFO_THRESHOLD);
	//
	//spiReadWrite(0xFF);//启动传输
	
	//Change
	spiReadWrite3(0xFF);//启动传输
}
void spiEnable(void)
{
	__HAL_SPI_ENABLE(&hspi2);
}
void spiDisable(void)
{
	__HAL_SPI_DISABLE(&hspi2);
}
void spiSelect(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	
	//level = HAL_GPIO_ReadPin(SPI_SEL_GPIO_Port, SPI_SEL_Pin);
	//HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin, level);
}
void spiDeselect(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
	
	//level = HAL_GPIO_ReadPin(SPI_SEL_GPIO_Port, SPI_SEL_Pin);
	//HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin, level);
}


uint8_t spiRxBufferNotEmpty(void)
{
	return __HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_RXNE);
}
uint8_t spiTxBufferEmpty(void)
{
	return __HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE);
}

void spiReadSingle1(uint8_t *data)
{
	//while (spiRxBufferComplete() == RESET){}
	while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_RXNE) == RESET){}//
	*data = (uint8_t)hspi2.Instance->DR;
}

void spiWriteSingle1(uint8_t *data)
{
	//while (spiTxBufferEmpty() == RESET){}
	while (__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE) == RESET){}
	hspi2.Instance->DR = *data;
}


void spiRead(uint8_t *data)
{
	HAL_SPI_Receive(&hspi2,data,1,100);
}
void spiWrite(uint8_t data)
{
	HAL_SPI_Transmit(&hspi2,&data,1,100);
}

uint8_t spiReadWrite1(uint8_t data)
{
	static uint8_t txdata,rxdata;
	txdata = data;
	if(HAL_SPI_TransmitReceive(&hspi2,&txdata,&rxdata,1,100)!=HAL_OK)
	{
		spiDeselect();
	}
	return rxdata;
}

//YXY_ERROR
extern int time1;
extern int time2;
extern int time3;
#include "cmsis_armcc.h"
uint8_t spiReadWrite2(uint8_t data)
{
	uint8_t txdata, rxdata;
	txdata = data;
	
	//uint8_t *ptxdata;
	//uint8_t prxdata[1];
	//ptxdata = &data;
	
	SPI_HandleTypeDef *hspi = &hspi2;
	
	/* Process Locked */
  __HAL_LOCK(hspi);
	
	//YXY_CHANGE:MOVE
	/* Set fiforxthreshold according the reception data length: 8bit */
  //SET_BIT(hspi->Instance->CR2, SPI_RXFIFO_THRESHOLD);
	
	/* Check if the SPI is already enabled */
  //YXY_CHANGE:leave out

	if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    // Enable SPI periphera
    __HAL_SPI_ENABLE(hspi);
  }

	
	if(txallowed == 0U)//clear DB
	{
		while ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) == RESET){ time1 = 100;}
		time1 = 1000;
		{
			//*(uint8_t *)prxdata = *(__IO uint8_t *)&hspi->Instance->DR;
			//rxdata = *prxdata;
			rxdata = *(__IO uint8_t *)(&hspi->Instance->DR);
			/* Next Data is a Transmission (Tx). Tx is allowed */
			txallowed = 1U;
		}
	}
	
	if(txallowed == 1U)//send tx
	{
		while((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET)){time2 = 100; }
		time2 = 1000;
		{
			//*(__IO uint8_t *)&hspi->Instance->DR = *ptxdata;
			*(__IO uint8_t *)(&hspi->Instance->DR) = txdata;
			/* Next Data is a reception (Rx). Tx not allowed */
			txallowed = 0U;
		}
	}
	
	//while ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_BSY)) != RESET){time3 = 100; }
	while ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) == RESET){time3 = 100; }
		time3= 1000;
  {
		//*(uint8_t *)prxdata = *(__IO uint8_t *)&hspi->Instance->DR;
		//rxdata = *prxdata;
		rxdata = *(__IO uint8_t *)(&hspi->Instance->DR);
		/* Next Data is a Transmission (Tx). Tx is allowed */
    txallowed = 1U;
	}
	
	__HAL_UNLOCK(hspi);
	return rxdata;
	//return *prxdata;
}

uint8_t spiReadWrite(uint8_t data)
{
	uint8_t txdata, rxdata;
	txdata = data;
	
	//uint8_t *ptxdata;
	//uint8_t prxdata[1];
	//ptxdata = &data;
	
	SPI_HandleTypeDef *hspi = &hspi2;
	
	/* Process Locked */
  __HAL_LOCK(hspi);
	
	//YXY_CHANGE:MOVE
	/* Set fiforxthreshold according the reception data length: 8bit */
  //SET_BIT(hspi->Instance->CR2, SPI_RXFIFO_THRESHOLD);
	
	/* Check if the SPI is already enabled */
  //YXY_CHANGE:leave out
	/*
	if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    // Enable SPI periphera
    __HAL_SPI_ENABLE(hspi);
  }
	*/
	if(txallowed == 0U)//clear DB
	{
		while ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) == RESET){ //time1 = 100;
		}
		time1 = 1000;
		{
			rxdata = *(__IO uint8_t *)(&hspi->Instance->DR);
			/* Next Data is a Transmission (Tx). Tx is allowed */
			txallowed = 1U;
		}
	}
	
	if(txallowed == 1U)//send tx
	{
		while((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET)){//time2 = 100; 
		}
		//time2 = 1000;
		{
			*(__IO uint8_t *)(&hspi->Instance->DR) = txdata;
			/* Next Data is a reception (Rx). Tx not allowed */
			txallowed = 0U;
		}
	}

	while ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) == RESET){time3 = 100;}
		time3= 1000;
  {
		rxdata = *(__IO uint8_t *)(&hspi->Instance->DR);
		/* Next Data is a Transmission (Tx). Tx is allowed */
    txallowed = 1U;
	}
	
	__HAL_UNLOCK(hspi);
	return rxdata;
}

uint8_t spiReadSingle(void)
{
	SPI_HandleTypeDef *hspi = &hspi2;
	uint8_t rxdata;
	
	while ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) == RESET){time3 = 100;}
		time3= 1000;
  {
		rxdata = *(__IO uint8_t *)(&hspi->Instance->DR);
		/* Next Data is a Transmission (Tx). Tx is allowed */
    txallowed = 1U;
	}
	return rxdata;
}

void spiWriteSingle(uint8_t *data)
{
	SPI_HandleTypeDef *hspi = &hspi2;
	
	while((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET)){//time2 = 100; 
	}
	//time2 = 1000;
	{
		*(__IO uint8_t *)(&hspi->Instance->DR) = *data;
		/* Next Data is a reception (Rx). Tx not allowed */
		txallowed = 0U;
	}
}

uint8_t SPI3_RXNE_State(void)
{
	return (SPI_CHECK_FLAG(hspi2.Instance->SR, SPI_FLAG_RXNE) != RESET);
}

//CHANGE_END

//Change
uint8_t spiReadWrite3(uint8_t txData)
{
	uint8_t rxData;
	
	HAL_SPI_TransmitReceive(&hspi2, &txData, &rxData, 1, HAL_MAX_DELAY);
	
	return rxData;
}
/* USER CODE END 1 */
