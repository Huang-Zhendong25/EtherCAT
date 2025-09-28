/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
\addtogroup Hardware Platform (Serial ESC Access)
@{
*/

/**
\file    hardware.c
\author EthercatSSC@beckhoff.com
\brief Implementation
Hardware access implementation for EL9800 onboard PIC18/PIC24 connected via SPI to ESC

\version 5.12

<br>Changes to version V5.11:<br>
V5.12 EL9800 1: improve the SPI access<br>
<br>Changes to version V5.10:<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
V5.11 EL9800 2: change PDI access test to 32Bit ESC access and reset AL Event mask after test even if AL Event is not enabled<br>
<br>Changes to version V5.01:<br>
V5.10 ESC5: Add missing swapping<br>
V5.10 HW3: Sync1 Isr added<br>
V5.10 HW4: Add volatile directive for direct ESC DWORD/WORD/BYTE access<br>
           Add missing swapping in mcihw.c<br>
           Add "volatile" directive vor dummy variables in enable and disable SyncManger functions<br>
           Add missing swapping in EL9800hw files<br>
<br>Changes to version V5.0:<br>
V5.01 HW1: Invalid ESC access function was used<br>
<br>Changes to version V4.40:<br>
V5.0 ESC4: Save SM disable/Enable. Operation may be pending due to frame handling.<br>
<br>Changes to version V4.30:<br>
V4.40 : File renamed from spihw.c to el9800hw.c<br>
<br>Changes to version V4.20:<br>
V4.30 ESM: if mailbox Syncmanger is disabled and bMbxRunning is true the SyncManger settings need to be revalidate<br>
V4.30 EL9800: EL9800_x hardware initialization is moved to el9800.c<br>
V4.30 SYNC: change synchronisation control function. Add usage of 0x1C32:12 [SM missed counter].<br>
Calculate bus cycle time (0x1C32:02 ; 0x1C33:02) CalcSMCycleTime()<br>
V4.30 PDO: rename PDO specific functions (COE_xxMapping -> PDO_xxMapping and COE_Application -> ECAT_Application)<br>
V4.30 ESC: change requested address in GetInterruptRegister() to prevent acknowledge events.<br>
(e.g. reading an SM config register acknowledge SM change event)<br>
GENERIC: renamed several variables to identify used SPI if multiple interfaces are available<br>
V4.20 MBX 1: Add Mailbox queue support<br>
V4.20 SPI 1: include SPI RxBuffer dummy read<br>
V4.20 DC 1: Add Sync0 Handling<br>
V4.20 PIC24: Add EL9800_4 (PIC24) required source code<br>
V4.08 ECAT 3: The AlStatusCode is changed as parameter of the function AL_ControlInd<br>
<br>Changes to version V4.02:<br>
V4.03 SPI 1: In ISR_GetInterruptRegister the NOP-command should be used.<br>
<br>Changes to version V4.01:<br>
V4.02 SPI 1: In HW_OutputMapping the variable u16OldTimer shall not be set,<br>
             otherwise the watchdog might exceed too early.<br>
<br>Changes to version V4.00:<br>
V4.01 SPI 1: DI and DO were changed (DI is now an input for the uC, DO is now an output for the uC)<br>
V4.01 SPI 2: The SPI has to operate with Late-Sample = FALSE on the Eva-Board<br>
<br>Changes to version V3.20:<br>
V4.00 ECAT 1: The handling of the Sync Manager Parameter was included according to<br>
              the EtherCAT Guidelines and Protocol Enhancements Specification<br>
V4.00 APPL 1: The watchdog checking should be done by a microcontroller<br>
                 timer because the watchdog trigger of the ESC will be reset too<br>
                 if only a part of the sync manager data is written<br>
V4.00 APPL 4: The EEPROM access through the ESC is added

*/


/*--------------------------------------------------------------------------------------
------
------    Includes
------
--------------------------------------------------------------------------------------*/
#include "ecat_def.h"

#include "ecatslv.h"


#define    _HARDWARE_ 1
#include "hardware.h"
#undef    _HARDWARE_
/*remove definition of _HARDWARE_ (#ifdef is used in hardware.h)*/

#include "ecatappl.h"

//YXY_CHANGE:HW_INC
#include "spi.h"
//#include "key.h"
//#include "led.h"

#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "TM1650.h"
#include "MCP4728.h"

/*--------------------------------------------------------------------------------------
------
------    internal Types and Defines
------
--------------------------------------------------------------------------------------*/

typedef union
{
    unsigned short    Word;
    unsigned char    Byte[2];
} UBYTETOWORD;

typedef union 
{
    UINT8           Byte[2];
    UINT16          Word;
}
UALEVENT;

/*-----------------------------------------------------------------------------------------
------
------    SPI defines/macros
------
-----------------------------------------------------------------------------------------*/
//YXY_CHANGE:HW_SPI
//CHANGE_START
// #define SPI1_SEL                        _LATB2
// #define SPI1_IF                            _SPI1IF
// #define SPI1_BUF                        SPI1BUF
// #define SPI1_CON1                        SPI1CON1
// #define SPI1_STAT                        SPI1STAT
// #define    WAIT_SPI_IF                        while( !SPI1_IF)
// #define    SELECT_SPI                        {(SPI1_SEL) = (SPI_ACTIVE);}
// #define    DESELECT_SPI                    {(SPI1_SEL) = (SPI_DEACTIVE);}
// #define    INIT_SSPIF                        {(SPI1_IF)=0;}
// #define SPI1_STAT_VALUE                    0x8000
// #define SPI1_CON1_VALUE                    0x027E
// #define SPI1_CON1_VALUE_16BIT            0x047E
// #define SPI_DEACTIVE                    1
// #define SPI_ACTIVE                        0


#define SELECT_SPI                           	{spiSelect();}
#define DESELECT_SPI	                      	{spiDeselect();}
#define ENABLE_SPI                            {spiEnable();}
#define DISABLE_SPI                           {spiDisable();}
#define SPI_INIT                              {spiEcatInit();}
//#define SPI_RXTX(TX)                          spiReadWrite(TX)
#define SPI_RXTX(TX)                          spiReadWrite3(TX)

extern SPI_HandleTypeDef hspi2;
#define WAIT_SPI_TX_IF              					while((__HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_TXE)) == RESET){}  //µÈ´ý·¢ËÍÇø¿Õ

//CHANGE_END

/*-----------------------------------------------------------------------------------------
------
------    Global Interrupt setting
------
-----------------------------------------------------------------------------------------*/
//YXY_CHANGE:HW_GLOBAL_INT
//CHANGE_START
//#define DISABLE_GLOBAL_INT            SET_CPU_IPL(4) //set CPU priority to level 4 (disable interrupt level 1 - 4)
//#define ENABLE_GLOBAL_INT            SET_CPU_IPL(0)

//YXY_TEST: 
volatile BOOL ESC_Int = FALSE;
volatile BOOL Global_Int = FALSE;
extern void SYNCOUT0_SET(void);
extern void SYNCOUT0_RESET(void);
volatile int Global_Int_Count[5] = {0,0,0,0,0};

uint8_t SpiErrorCounter = 0;  //Being used to compensate the spi read when EXT_IRQ triggered failed.


//#define DISABLE_GLOBAL_INT    				  {Disable_global_interrupt();}
//#define ENABLE_GLOBAL_INT     				  {Enable_global_interrupt();}

//#define DISABLE_GLOBAL_INT    				  {Disable_global_interrupt();ClearEXITPending();ClearTIMITPending();Global_Int = FALSE;}
#define DISABLE_GLOBAL_INT    				  {Disable_global_interrupt();Global_Int = FALSE;}
#define ENABLE_GLOBAL_INT     				  {Enable_global_interrupt();Global_Int = TRUE;}

#define DISABLE_GLOBAL_INT1    				  {Disable_global_interrupt();ClearEXITPending();ClearTIMITPending();SYNCOUT0_RESET();Global_Int = FALSE;}
#define ENABLE_GLOBAL_INT1     				  {Enable_global_interrupt();SYNCOUT0_SET();Global_Int = TRUE;}


//#define DISABLE_GLOBAL_INT(NUM)    				  {Disable_global_interrupt();ClearEXITPending();ClearTIMITPending();SYNCOUT0_RESET();Global_Int_Count[NUM]++;Global_Int = FALSE;}
//#define ENABLE_GLOBAL_INT(NUM)     				  {Enable_global_interrupt();SYNCOUT0_SET();Global_Int_Count[NUM]++;Global_Int = TRUE;}

//#define DISABLE_GLOBAL_INT(NUM)    				  {Disable_global_interrupt();ClearEXITPending();ClearTIMITPending();Global_Int_Count[NUM]++;Global_Int = FALSE;}
//#define ENABLE_GLOBAL_INT(NUM)     				  {Enable_global_interrupt();Global_Int_Count[NUM]++;Global_Int = TRUE;}


//CHANGE_END
#define    DISABLE_AL_EVENT_INT        DISABLE_GLOBAL_INT
#define    ENABLE_AL_EVENT_INT          ENABLE_GLOBAL_INT


/*-----------------------------------------------------------------------------------------
------
------    ESC Interrupt
------
-----------------------------------------------------------------------------------------*/

//YXY_CHANGE:HW_ESC_INT
//CHANGE_START
//#define    INIT_ESC_INT               {(_INT1EP) = 1;(_INT1IP) = 1;} //_INT1EP = 1:  negative edge ; _INT1IP = 1; //highest priority
//#define    ESC_INT_REQ                (_INT1IF) //ESC Interrupt (INT1) state
//#define    INT_EL                    (_RD8) //ESC Interrupt input port
//#define    EscIsr                    (_INT1Interrupt) // primary interrupt vector name
//#define    ACK_ESC_INT                {(ESC_INT_REQ)=0;}

#define    INIT_ESC_INT  							     {escInit();}
#define    EscIsr                    		(INT_ESC_Handler)
//CHANGE_END

/*-----------------------------------------------------------------------------------------
------
------    SYNC0 Interrupt
------
-----------------------------------------------------------------------------------------*/
//YXY_CHANGE:HW_SUNC0_INT
//CHANGE_START
//#define    INIT_SYNC0_INT                   {(_INT3EP) = 1;(_INT3IP) = 1;}//_INT3EP = 1:  negative edge ; _INT3IP = 1; //highest priority
//#define    SYNC0_INT_REQ                    (_INT3IF) //Sync0 Interrupt (INT3) state
//#define    INT_SYNC0                        (_RD10) //Sync1 Interrupt input port
//#define    Sync0Isr                        (_INT3Interrupt) // primary interrupt vector name
//#define    DISABLE_SYNC0_INT                {(_INT3IE)=0;}//disable interrupt source INT3
//#define    ENABLE_SYNC0_INT                {(_INT3IE) = 1;} //enable interrupt source INT3
//#define    ACK_SYNC0_INT                    {(SYNC0_INT_REQ) = 0;}
//#define    SET_SYNC0_INT                    {(SYNC0_INT_REQ) = 1;}
//#define    SYNC0_INT_PORT_IS_ACTIVE        {(INT_EL) == 0;}


#define    INIT_SYNC0_INT                   {sync0Init();}
#define    DISABLE_SYNC0_INT          			{sync0Disable();}
#define    ENABLE_SYNC0_INT            			{sync0Enable();}

#define    Sync0Isr                         (SYNC0_Handler)
//CHANGE_END

//YXY_CHANGE:HW_SUNC0_INT
//CHANGE_START
//#define    INIT_SYNC1_INT                   {(_INT4EP) = 1;(_INT4IP) = 1;}//_INT4EP = 1:  negative edge ; _INT4IP = 1; //highest priority
//#define    SYNC1_INT_REQ                    (_INT4IF) //Sync1 Interrupt (INT4) state
//#define    INT_SYNC1                        (_RD11) //Sync1 Interrupt input port
//#define    Sync1Isr                        (_INT4Interrupt) // primary interrupt vector name
//#define    DISABLE_SYNC1_INT                {(_INT4IE)=0;}//disable interrupt source INT4
//#define    ENABLE_SYNC1_INT                {(_INT4IE) = 1;} //enable interrupt source INT4
//#define    ACK_SYNC1_INT                    {(SYNC1_INT_REQ) = 0;}
//#define    SET_SYNC1_INT                    {(SYNC1_INT_REQ) = 1;}
//#define    SYNC1_INT_PORT_IS_ACTIVE        {(INT_EL) == 0;}

#define    INIT_SYNC1_INT                   {sync1Init();}
#define    DISABLE_SYNC1_INT                {sync1Disable();}
#define    ENABLE_SYNC1_INT                 {sync1Enable();}

#define    Sync1Isr                         (SYNC1_Handler)
//CHANGE_END

/*-----------------------------------------------------------------------------------------
------
------    Hardware timer
------
-----------------------------------------------------------------------------------------*/
//YXY_CHANGE:HW_ECAT_TIMER
//CHANGE_START
//#define INIT_ECAT_TIMER                {(PR7) = 2000;/*set period*/ \
    (T7CONbits.TCKPS) = 2;/*set prescaler to 1:8*/ \
    (TMR7) = 0;/*clear timer register*/}

//#define STOP_ECAT_TIMER                {(T7CONbits.TON) = 0; /*disable timer*/}

//#define START_ECAT_TIMER            {(T7CONbits.TON) = 1; /*enable timer*/}


//#define INIT_ECAT_TIMER                {tim6Init();tim6ITStart();/*init timer*/}
//#define STOP_ECAT_TIMER                {tim6Disable(); /*disable timer*/}
//#define START_ECAT_TIMER            	 {tim6Enable(); /*enable timer*/}

#define INIT_ECAT_TIMER                {tim2Init();/*init timer*/}
#define STOP_ECAT_TIMER                {tim2Disable(); /*disable timer*/}
#define START_ECAT_TIMER            	 {tim2ITStart();tim2Enable(); /*enable timer*/}


#define TIM_INIT                  	  {tim7Init();}//tim3Init();tim4Init();tim5Init();
#define TIM_STOP  										{tim7Disable();}//tim3Disable();tim4Disable();tim5Disable();
#define TIM_START             	      {tim7ITStart();tim7Enable();}//tim3ITStart();tim3Enable();tim4ITStart();tim4Enable();tim5ITStart();tim5Enable();
//CHANGE_END

/*-----------------------------------------------------------------------------------------
------
------    Configuration Bits
------
-----------------------------------------------------------------------------------------*/
//YXY_CHANGE:HW_OTHER
//CHANGE_START
// _FGS(GCP_OFF);
// _FOSCSEL(FNOSC_PRI & IESO_OFF);
// _FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_HS);
// _FWDT(FWDTEN_OFF);
// _FPOR(FPWRT_PWR1);
// _FICD(ICS_PGD3 & JTAGEN_OFF);
//CHANGE_END
/*-----------------------------------------------------------------------------------------
------
------    LED defines
------
-----------------------------------------------------------------------------------------*/
//YXY_CHANGE:HW_LED
//CHANGE_START
// EtherCAT Status LEDs -> StateMachine
// #define LED_ECATGREEN                 LATFbits.LATF1
// #define LED_ECATRED                    LATFbits.LATF0
#define  LED_INIT            {ledInit();}
//CHANGE_END

/*-----------------------------------------------------------------------------------------
------
------    KEY defines
------
-----------------------------------------------------------------------------------------*/
//YXY_CHANGE:HW_KEY
//CHANGE_START
#define  KEY_INIT            {keyInit();}
//CHANGE_END
/*-----------------------------------------------------------------------------------------
------
------    NOP defines/macros
------
-----------------------------------------------------------------------------------------*/
//YXY_CHANGE:HW_NOP
//CHANGE_START
#define NOP_CMD                   1
#define Nop        		 __NOP
//CHANGE_END



/*--------------------------------------------------------------------------------------
------
------    internal Variables
------
--------------------------------------------------------------------------------------*/
UALEVENT         EscALEvent;            //contains the content of the ALEvent register (0x220), this variable is updated on each Access to the Esc
/*--------------------------------------------------------------------------------------
------
------    internal functions
------
--------------------------------------------------------------------------------------*/

/*ECATCHANGE_START(V5.12) EL9800 1*/
static UINT8 RxTxSpiData(UINT8 MosiByte)
{
    VARVOLATILE UINT8 MisoByte = 0;
		//YXY_CHANGE:HW_SPI_TX_RX
		//CHANGE_START
		
    /*
    if((SPI1_STAT & 0x1) != 0)
    {
        //read buffer to prevent buffer overrun error
        MisoByte = SPI1_BUF;
    }
    
    
    SPI1_IF = 0;

    SPI1_BUF = MosiByte;
    
    // wait until the transmission of the byte is finished
    WAIT_SPI_IF;

    MisoByte = SPI1_BUF;

    // reset transmission flag
    SPI1_IF = 0;
		*/
		
		MisoByte = SPI_RXTX(MosiByte);
		//SPI_RXTX(0xFF);
		//MisoByte = spiReadWrite(MosiByte);
    
    //CHANGE_END
    
		
		
		
    return MisoByte;
}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Command    ESC_WR performs a write access; ESC_RD performs a read access.

 \brief The function addresses the EtherCAT ASIC via SPI for a following SPI access.
*////////////////////////////////////////////////////////////////////////////////////////
//YXY_TEST
static void AddressingEsc( UINT16 Address, UINT8 Command )
{
    VARVOLATILE UBYTETOWORD tmp;
    tmp.Word = ( Address << 3 ) | Command;
		
    /* select the SPI */
    SELECT_SPI;

    /* send the first address/command byte to the ESC 
       receive the first AL Event Byte*/

		EscALEvent.Byte[0] = RxTxSpiData(tmp.Byte[1]);

    EscALEvent.Byte[1] = RxTxSpiData(tmp.Byte[0]);

		
}

static void AddressingEsc1( UINT16 Address, UINT8 Command ){
	
		VARVOLATILE UBYTETOWORD tmp;
		tmp.Word = ( Address << 3 ) | Command;
		
		VARVOLATILE UINT8 dummy;
		UINT8 Val;

		/* select the SPI */
		SELECT_SPI;


		Val = tmp.Byte[1];
		spiWriteSingle(&Val);
		EscALEvent.Byte[0] = spiReadSingle();

		Val = tmp.Byte[0];
		spiWriteSingle(&Val);
		EscALEvent.Byte[1] = spiReadSingle();

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief  The function operates a SPI access without addressing.

        The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
*////////////////////////////////////////////////////////////////////////////////////////
static void GetInterruptRegister(void)
{
    VARVOLATILE UINT8            dummy;
    HW_EscRead((MEM_ADDR *)&dummy, 0, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief  The function operates a SPI access without addressing.
        Shall be implemented if interrupts are supported else this function is equal to "GetInterruptRegsiter()"

        The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
*////////////////////////////////////////////////////////////////////////////////////////
static void ISR_GetInterruptRegister(void)
{
    VARVOLATILE UINT8 dummy;
    HW_EscReadIsr((MEM_ADDR *)&dummy, 0, 1);
}
/*ECATCHANGE_END(V5.12) EL9800 1*/

/*--------------------------------------------------------------------------------------
------
------    exported hardware access functions
------
--------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0 if initialization was successful

 \brief    This function intialize the Process Data Interface (PDI) and the host controller.
*////////////////////////////////////////////////////////////////////////////////////////
UINT8 HW_Init(void)
{
    UINT32 intMask;
	
		
	//YXY_CHANGE:HW_APPL_INIT
    //CHANGE_START
		//LED_INIT
		//KEY_INIT
		//TIM_INIT
		//TIM_START
	  MX_GPIO_Init();
  MX_I2C1_Init();
	//MX_TIM2_Init();   //1ms
	//HAL_TIM_Base_Start_IT(&htim2);
	//MX_TIM1_Init();
	//MX_SPI2_Init();
	MX_SPI1_Init();
  MX_I2C2_Init();
	
	
//	HAL_GPIO_WritePin(DRIVER_CONTROL1_GPIO_Port, DRIVER_CONTROL1_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(DRIVER_CONTROL2_GPIO_Port, DRIVER_CONTROL2_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(DRIVER_CONTROL3_GPIO_Port, DRIVER_CONTROL3_Pin, GPIO_PIN_SET);

	
	//SPI_INIT
		//LED3_SET
		spiEcatInit();
	
	//TM1650_Init();
	
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	
	//HAL_TIM_Base_Start(&htim1);
	//HAL_TIM_Base_Start_IT(&htim2);
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 500);
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 500);
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, 500);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

  //MX_TIM1_Init();
	//HAL_TIM_Base_Start_IT(&htim1);
	//__HAL_TIM_ENABLE(&htim1);
	
		//CHANGE_END
		
	//YXY_CHANGE:HW_SPI_INIT
    //CHANGE_START
    /* initialize the SPI registers for the ESC SPI */
    //SPI1_CON1 = SPI1_CON1_VALUE;
    //SPI1_STAT = SPI1_STAT_VALUE;
	
    
    //CHANGE_END
		
		//ESC_AL_EVENTMASK_OFFSET: 0x0204
    do
    {
        intMask = 0x93;
        HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
        intMask = 0;
        HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
    } while (intMask != 0x93);
	
    intMask = 0x00;
		HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
		
		//Test Register Write
//		UINT32 pTmpDataW = 0x123c567d;
//		UINT32 pTmpDataR = 0;
//		uint8_t pTmpDataW1[3];
//		pTmpDataW1[0] = 0x123456;
//		pTmpDataW1[1] = 0x78abcd;
//		pTmpDataW1[2] = 0x111111;
//		uint32_t pTmpDataW1 = pTmpDataW >> 3;
//		HW_EscWriteDWord(pTmpDataW1, 0x0f80);
//		HW_EscWriteDWord(pTmpDataW1, 0x0f80);
//		HW_EscReadDWord(pTmpDataR, 0x0f80);
		
		//pTmpDataW = 0x9999;
		//HW_EscWriteDWord(pTmpDataW, 0x0f84);
		
		//if(pTmpDataR == 0x8888)
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
//		if(pTmpDataR == 0x8888)
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
		

    INIT_ESC_INT
    //YXY_CHANGE:HW_ESC_INIT
		//CHANGE_START
    //(_INT1IE)=1;
		ENABLE_ESC_INT();
		//CHANGE_END

    INIT_SYNC0_INT
    INIT_SYNC1_INT

    ENABLE_SYNC0_INT;
    ENABLE_SYNC1_INT;
		
    INIT_ECAT_TIMER;
    START_ECAT_TIMER;
		
		//CHANGE: Enable Software interrupt
		EXTI->IMR |= 1<<0;
		HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
		//CHANGE END

    /* enable all interrupts */
    ENABLE_GLOBAL_INT;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function shall be implemented if hardware resources need to be release
        when the sample application stops
*////////////////////////////////////////////////////////////////////////////////////////
void HW_Release(void)
{
	STOP_ECAT_TIMER
	DISABLE_SYNC0_INT
	DISABLE_SYNC1_INT
	DISABLE_ESC_INT();
	DISABLE_SPI
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  This function gets the current content of ALEvent register
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 HW_GetALEventRegister(void)
{
    GetInterruptRegister();
    return EscALEvent.Word;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  The SPI PDI requires an extra ESC read access functions from interrupts service routines.
        The behaviour is equal to "HW_GetALEventRegister()"
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 HW_GetALEventRegister_Isr(void)
{
     ISR_GetInterruptRegister();
    return EscALEvent.Word;
}



/*ECATCHANGE_START(V5.12) EL9800 1*/
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  This function operates the SPI read access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
//YXY_TEST
void HW_EscRead( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    /* HBu 24.01.06: if the SPI will be read by an interrupt routine too the
                     mailbox reading may be interrupted but an interrupted
                     reading will remain in a SPI transmission fault that will
                     reset the internal Sync Manager status. Therefore the reading
                     will be divided in 1-byte reads with disabled interrupt */
    UINT16 i = Len;
    UINT8 *pTmpData = (UINT8 *)pData;
	
	//Check EXTI and NVIC's registers
//	uint8_t Register_Val[3];
//	uint8_t Register_Val1[3];
//	Register_Val[0] = EXTI->IMR;
//	Register_Val[1] = EXTI->FTSR;
//	Register_Val[2] = EXTI->PR;
//	Register_Val1[0] = NVIC->ISER[0] >> 6;
//	Register_Val1[1] = NVIC->ISPR[0] >> 6;
//	Register_Val1[2] = NVIC->IABR[0] >> 6;
//	
//	DISABLE_GLOBAL_INT;
//	AddressingEsc(0x0f80, ESC_RD);
//	RxTxSpiData(Register_Val[0]);
//	RxTxSpiData(Register_Val[1]);
//	RxTxSpiData(Register_Val[2]);
//	RxTxSpiData(Register_Val1[0]);
//	RxTxSpiData(Register_Val1[1]);
//	RxTxSpiData(Register_Val1[2]);
//	DESELECT_SPI
//	ENABLE_GLOBAL_INT;

    /* loop for all bytes to be read */
    while ( i-- > 0 )
    {
/*ECATCHANGE_START(V5.12) EL9800 1*/
        /* the reading of data from the ESC can be interrupted by the
           AL Event ISR, in that case the address has to be reinitialized,
           in that case the status flag will indicate an error because
           the reading operation was interrupted without setting the last
           sent byte to 0xFF */
			
        DISABLE_GLOBAL_INT
			
//			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET)
//			{
//				if(Address != 0x00)
//				{
//					AddressingEsc(0x00, ESC_RD);
//					RxTxSpiData(0xFF);
//					DESELECT_SPI
//					AddressingEsc(0x1100, ESC_RD);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0x00);
//					RxTxSpiData(0xFF);
//					DESELECT_SPI
//					AddressingEsc(0x00, ESC_RD);
//					RxTxSpiData(0xFF);
//					DESELECT_SPI
//				}
//				else
//				{}
//				AddressingEsc(0x0f80, ESC_RD);
//				RxTxSpiData(0x99);
//				DESELECT_SPI
//			}
			
/*ECATCHANGE_END(V5.12) EL9800 1*/

         AddressingEsc( Address, ESC_RD );

         /*Each Byte will be read with a new addressing phase so the out data is 0xFF*/
         *pTmpData = RxTxSpiData(0xFF);
         
         pTmpData++;

         /*Wait for 250ns because the each byte is transmitted separate*/
#if defined(NOP_CMD) && (NOP_CMD == 1)
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
#endif

/*ECATCHANGE_START(V5.12) EL9800 1*/        

        /* there has to be at least 15 ns + CLK/2 after the transmission is finished
           before the SPI1_SEL signal shall be 1 */
        DESELECT_SPI

        ENABLE_GLOBAL_INT;
				
				//CHANGE: EXT_IRQ spi read compensation
				if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET)
				{
					if(SpiErrorCounter == 0)
					{
						if(Address != 0x00 && Address != 0x1100)
						{
							EXTI->SWIER |= 1<<0;   //Trigger software interrupt
							//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
						}
						else if(Address == 0x00)
						{
							SpiErrorCounter++;
						}
						else
						{}
					}
					else if(SpiErrorCounter == 1)
					{
						if(Address != 0x1100)
						{
							EXTI->SWIER |= 1<<0;
							//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
							//SpiErrorCounter = 0;
						}
						else
						{
							SpiErrorCounter = 0;
						}
					}
				}
				
				//CHANGE END
				
/*ECATCHANGE_END(V5.12) EL9800 1*/

        /* next address */
        Address++;

    }
}

void HW_EscRead1( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    /* HBu 24.01.06: if the SPI will be read by an interrupt routine too the
                     mailbox reading may be interrupted but an interrupted
                     reading will remain in a SPI transmission fault that will
                     reset the internal Sync Manager status. Therefore the reading
                     will be divided in 1-byte reads with disabled interrupt */
    UINT16 i = Len;
    UINT8 *pTmpData = (UINT8 *)pData;
		UINT8 Val;//YXY_CHANGE:20200222

    /* loop for all bytes to be read */
    while ( i-- > 0 )
    {
/*ECATCHANGE_START(V5.12) EL9800 1*/
        /* the reading of data from the ESC can be interrupted by the
           AL Event ISR, in that case the address has to be reinitialized,
           in that case the status flag will indicate an error because
           the reading operation was interrupted without setting the last
           sent byte to 0xFF */
        DISABLE_GLOBAL_INT;
/*ECATCHANGE_END(V5.12) EL9800 1*/

         AddressingEsc( Address, ESC_RD );

         /*Each Byte will be read with a new addressing phase so the out data is 0xFF*/
					Val = 0xFF;
				spiWriteSingle(&Val);
			
			/* get data byte */
				Val = spiReadSingle();
				*pTmpData++ = Val;

         /*Wait for 250ns because the each byte is transmitted separate*/
#if defined(NOP_CMD) && (NOP_CMD == 1)
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
#endif

/*ECATCHANGE_START(V5.12) EL9800 1*/        

        /* there has to be at least 15 ns + CLK/2 after the transmission is finished
           before the SPI1_SEL signal shall be 1 */
        DESELECT_SPI

        ENABLE_GLOBAL_INT;
/*ECATCHANGE_END(V5.12) EL9800 1*/

        /* next address */
        Address++;
				
				WAIT_SPI_TX_IF
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

\brief  The SPI PDI requires an extra ESC read access functions from interrupts service routines.
        The behaviour is equal to "HW_EscRead()"
*////////////////////////////////////////////////////////////////////////////////////////
//YXY_TEST
void HW_EscReadIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    UINT8 data = 0;

   UINT8 *pTmpData = (UINT8 *)pData;

    /* send the address and command to the ESC */
     AddressingEsc( Address, ESC_RD );

    /* loop for all bytes to be read */
    while ( i-- > 0 )
    {
        if ( i == 0 )
        {
            /* when reading the last byte the DI pin shall be 1 */
            data = 0xFF;
        }

        *pTmpData = RxTxSpiData(data);

        pTmpData++;
    }
#if defined(NOP_CMD) && (NOP_CMD == 1)
     Nop();
#endif
     //only a single byte was transmitted => wait for the 250ns
     if(Len == 1)
     {
#if defined(NOP_CMD) && (NOP_CMD == 1)
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
#endif
    }
    
    /* there has to be at least 15 ns + CLK/2 after the transmission is finished
       before the SPI1_SEL signal shall be 1 */
    DESELECT_SPI
}
void HW_EscReadIsr1( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    UINT8 data = 0;
	UINT8 Val;//YXY_CHANGE:20210222

   UINT8 *pTmpData = (UINT8 *)pData;

    /* send the address and command to the ESC */
     AddressingEsc( Address, ESC_RD );

    /* loop for all bytes to be read */
    while ( i-- > 0 )
    {
        if ( i == 0 )
        {
            /* when reading the last byte the DI pin shall be 1 */
            data = 0xFF;
        }
				/* reset transmission flag */
				spiWriteSingle(&data);
				
        /* get data byte */
				Val = spiReadSingle();
				*pTmpData++ = Val;
    }
#if defined(NOP_CMD) && (NOP_CMD == 1)
     Nop();
#endif
     //only a single byte was transmitted => wait for the 250ns
     if(Len == 1)
     {
#if defined(NOP_CMD) && (NOP_CMD == 1)
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
#endif
    }
    WAIT_SPI_TX_IF
    /* there has to be at least 15 ns + CLK/2 after the transmission is finished
       before the SPI1_SEL signal shall be 1 */
    DESELECT_SPI
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

  \brief  This function operates the SPI write access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
//YXY_TEST
void HW_EscWrite( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    VARVOLATILE UINT8 dummy;

    UINT8 *pTmpData = (UINT8 *)pData;
	
    /* loop for all bytes to be written */
    while ( i-- > 0 )
    {
/*ECATCHANGE_START(V5.12) EL9800 1*/
        /* the reading of data from the ESC can be interrupted by the
           AL Event ISR, so every byte will be written separate */
        DISABLE_GLOBAL_INT;
/*ECATCHANGE_END(V5.12) EL9800 1*/

        /* HBu 24.01.06: wrong parameter ESC_RD */
         AddressingEsc( Address, ESC_WR );
				
        dummy = RxTxSpiData(*pTmpData);
			
         pTmpData++;

/*ECATCHANGE_START(V5.12) EL9800 1*/
         /*Wait for 250ns because the each byte is transmitted separate*/
#if defined(NOP_CMD) && (NOP_CMD == 1)
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
#endif
        /* there has to be at least 15 ns + CLK/2 after the transmission is finished
           before the SPI1_SEL signal shall be 1 */

        DESELECT_SPI


        ENABLE_GLOBAL_INT;
/*ECATCHANGE_END(V5.12) EL9800 1*/

        /* next address */
        Address++;


    }
}


void HW_EscWrite1( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    VARVOLATILE UINT8 dummy;
	UINT8 Val;//YXY_CHANGE:20170302

    UINT8 *pTmpData = (UINT8 *)pData;
	
    /* loop for all bytes to be written */
    while ( i-- > 0 )
    {
/*ECATCHANGE_START(V5.12) EL9800 1*/
        /* the reading of data from the ESC can be interrupted by the
           AL Event ISR, so every byte will be written separate */
        DISABLE_GLOBAL_INT;
/*ECATCHANGE_END(V5.12) EL9800 1*/

        /* HBu 24.01.06: wrong parameter ESC_RD */
         AddressingEsc( Address, ESC_WR );
				
        Val = *pTmpData++;
				spiWriteSingle(&Val);
				dummy = spiReadSingle();

/*ECATCHANGE_START(V5.12) EL9800 1*/
         /*Wait for 250ns because the each byte is transmitted separate*/
#if defined(NOP_CMD) && (NOP_CMD == 1)
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
#endif
        /* there has to be at least 15 ns + CLK/2 after the transmission is finished
           before the SPI1_SEL signal shall be 1 */

        DESELECT_SPI


        ENABLE_GLOBAL_INT;
/*ECATCHANGE_END(V5.12) EL9800 1*/

        /* next address */
        Address++;


    }
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  The SPI PDI requires an extra ESC write access functions from interrupts service routines.
        The behaviour is equal to "HW_EscWrite()"
*////////////////////////////////////////////////////////////////////////////////////////
//YXY_TEST
void HW_EscWriteIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    VARVOLATILE UINT16 dummy;
    UINT8 *pTmpData = (UINT8 *)pData;

    /* send the address and command to the ESC */
     AddressingEsc( Address, ESC_WR );
     /* loop for all bytes to be written */
    while ( i-- > 0 )
    {

         dummy = RxTxSpiData(*pTmpData);
         pTmpData++;
    }
#if defined(NOP_CMD) && (NOP_CMD == 1)
     Nop();
#endif
     //only a single byte was transmitted => wait for the 250ns
     if(Len == 1)
     {
#if defined(NOP_CMD) && (NOP_CMD == 1)
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
#endif
    }

    /* there has to be at least 15 ns + CLK/2 after the transmission is finished
       before the SPI1_SEL signal shall be 1 */
    DESELECT_SPI
}


void HW_EscWriteIsr1( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    VARVOLATILE UINT16 dummy;
    UINT8 *pTmpData = (UINT8 *)pData;
		UINT8 Val;//YXY_CHANGE:

    /* send the address and command to the ESC */
     AddressingEsc( Address, ESC_WR );
     /* loop for all bytes to be written */
    while ( i-- > 0 )
    {
			Val = *pTmpData;
			spiWriteSingle(&Val);
			
			dummy = spiReadSingle();
			pTmpData++;	
    }
#if defined(NOP_CMD) && (NOP_CMD == 1)
     Nop();
#endif
     //only a single byte was transmitted => wait for the 250ns
     if(Len == 1)
     {
#if defined(NOP_CMD) && (NOP_CMD == 1)
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
#endif
    }

    /* there has to be at least 15 ns + CLK/2 after the transmission is finished
       before the SPI1_SEL signal shall be 1 */
    DESELECT_SPI
}
/*ECATCHANGE_END(V5.12) EL9800 1*/




/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Interrupt service routine for the PDI interrupt from the EtherCAT Slave Controller
*////////////////////////////////////////////////////////////////////////////////////////
//YXY_CHANGE:HW_ESC_HANDLER
//CHANGE_START
//void __attribute__ ((__interrupt__, no_auto_psv)) EscIsr(void)

void INT_ESC_Handler(void)
{
     PDI_Isr();
    /* reset the interrupt flag */
		//YXY_CHANGE:HW_ESC_HANDLER
		//CHANGE_START
    //ACK_ESC_INT;
		//CHANGE_END
}
//CHANGE_END



/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Interrupt service routine for the interrupts from SYNC0
*////////////////////////////////////////////////////////////////////////////////////////
//YXY_CHANGE:HW_SYNC0_HANDLER
//CHANGE_START
//void __attribute__((__interrupt__, no_auto_psv)) Sync0Isr(void)

void SYNC0_Handler(void)
{
	//Check Registers of EXTI and NVIC
//	uint8_t Register_Val[3];
//	uint8_t Register_Val1[3];
//	Register_Val[0] = EXTI->IMR;
//	Register_Val[1] = EXTI->FTSR;
//	Register_Val[2] = EXTI->PR;
//	Register_Val1[0] = NVIC->ISER[0] >> 6;
//	Register_Val1[1] = NVIC->ISPR[0] >> 6;
//	Register_Val1[2] = NVIC->IABR[0] >> 6;
//	
//	HW_EscWriteDWord(Register_Val, 0x0f80);
//	HW_EscWriteDWord(Register_Val1, 0x0f84);
	
    Sync0_Isr();
    /* reset the interrupt flag */
		//YXY_CHANGE:HW_SYNC0_HANDLER
		//CHANGE_START
    //ACK_SYNC0_INT;
		//CHANGE_END
}
//CHANGE_END
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Interrupt service routine for the interrupts from SYNC1
*////////////////////////////////////////////////////////////////////////////////////////
//YXY_CHANGE:HW_SYNC1_HANDLER
//CHANGE_START
//void __attribute__((__interrupt__, no_auto_psv)) Sync1Isr(void)

void SYNC1_Handler(void)
{
    Sync1_Isr();

    /* reset the interrupt flag */
		//YXY_CHANGE:HW_SYNC1_HANDLER
		//CHANGE_START
    //ACK_SYNC1_INT;
		//CHANGE_END
}

void EXTI0_IRQHandler()
{
	DISABLE_GLOBAL_INT
	
	if(EXTI->PR &= 1<<0)
	{
		EXTI->PR |= 1<<0;
		
		AddressingEsc(0x00, ESC_RD);
		RxTxSpiData(0xFF);
		DESELECT_SPI
		AddressingEsc(0x1100, ESC_RD);
		RxTxSpiData(0x00);
		RxTxSpiData(0x00);
		RxTxSpiData(0x00);
		RxTxSpiData(0x00);
		RxTxSpiData(0x00);
		RxTxSpiData(0x00);
		RxTxSpiData(0x00);
		RxTxSpiData(0x00);
		RxTxSpiData(0x00);
		RxTxSpiData(0x00);
		RxTxSpiData(0x00);
		RxTxSpiData(0xFF);
		DESELECT_SPI
		AddressingEsc(0x00, ESC_RD);
		RxTxSpiData(0xFF);
		DESELECT_SPI
		
		SpiErrorCounter = 0;
	}
	
	ENABLE_GLOBAL_INT;
}
//CHANGE_END



/** @} */
