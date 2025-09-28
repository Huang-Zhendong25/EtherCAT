/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
\addtogroup Ethernet_CAT application
@{
*/

/**
\file appl.c
\author EthercatSSC@beckhoff.com
\brief Implementation

\version 5.12

<br>Changes to version V5.11:<br>
V5.12 ECAT1: update SM Parameter measurement (based on the system time), enhancement for input only devices and no mailbox support, use only 16Bit pointer in process data length caluclation<br>
<br>Changes to version V5.10:<br>
V5.11 ECAT11: create application interface function pointer, add eeprom emulation interface functions<br>
V5.11 EL9800 1: reset outputs on fallback from OP state<br>
<br>Changes to version V5.01:<br>
V5.10 ECAT6: Add "USE_DEFAULT_MAIN" to enable or disable the main function<br>
<br>Changes to version V5.0:<br>
V5.01 EL9800 2: Add TxPdo Parameter object 0x1802<br>
<br>Changes to version V4.30:<br>
V4.50 ECAT2: Create generic application interface functions. Documentation in Application Note ET9300.<br>
V4.50 COE2: Handle invalid PDO assign values.<br>
V4.30 : create file
*/


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "ecat_def.h"


#include "applInterface.h"


#include "hardware.h"


//Light driver
#include "MCP4728.h"
#include "i2c.h"
#include "TM1650.h"
#include "LM3409.h"

#define _EVALBOARD_
#include "appl.h"
#undef _EVALBOARD_

//YXY_CHANGE
//#include "led.h"
/*--------------------------------------------------------------------------------------
------
------    local types and defines
------
--------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/
/*
REAL32 lazerTrackerData_position[3];
REAL32 lazerTrackerData_angle[4];
TOBJ7010 LED_A;
*/
uint8_t channel[3] = {CHANNEL_A, CHANNEL_B, CHANNEL_C};
float voltage[3];
uint8_t dig[3];
uint8_t light;
/*-----------------------------------------------------------------------------------------
------
------    application specific functions
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    generic functions
------
-----------------------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when an error state was acknowledged by the master

*////////////////////////////////////////////////////////////////////////////////////////

void    APPL_AckErrorInd(UINT16 stateTrans)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from INIT to PREOP when
           all general settings were checked to start the mailbox handler. This function
           informs the application about the state transition, the application can refuse
           the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from PREEOP to INIT
             to stop the mailbox handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pIntMask    pointer to the AL Event Mask which will be written to the AL event Mask
                        register (0x204) when this function is succeeded. The event mask can be adapted
                        in this function
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from PREOP to SAFEOP when
             all general settings were checked to start the input handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case the application need to be complete 
            the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartInputHandler(UINT16 *pIntMask)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from SAFEOP to PREEOP
             to stop the input handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopInputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from SAFEOP to OP when
             all general settings were checked to start the output handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartOutputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from OP to SAFEOP
             to stop the output handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopOutputHandler(void)
{
/*
	sDOOutputs.bLED1 = 0;
    sDOOutputs.bLED2 = 0;
    sDOOutputs.bLED3 = 0;
    sDOOutputs.bLED4 = 0;
    sDOOutputs.bLED5 = 0;
    sDOOutputs.bLED7 = 0;
    sDOOutputs.bLED6 = 0;
    sDOOutputs.bLED8 = 0;
    // LED_1                        = sDOOutputs.bLED1;
    // LED_2                        = sDOOutputs.bLED2;
    // LED_3                        = sDOOutputs.bLED3;
    // LED_4                        = sDOOutputs.bLED4;
    // LED_5                        = sDOOutputs.bLED5;
    // LED_7                        = sDOOutputs.bLED7;
    // LED_6                        = sDOOutputs.bLED6;
    // LED_8                        = sDOOutputs.bLED8;
	LED_A.bLED1 = sDOOutputs.bLED1;
	LED_A.bLED2 = sDOOutputs.bLED2;
	LED_A.bLED3 = sDOOutputs.bLED3;
	LED_A.bLED4 = sDOOutputs.bLED4;
	LED_A.bLED5 = sDOOutputs.bLED5;
	LED_A.bLED6 = sDOOutputs.bLED6;
	LED_A.bLED7 = sDOOutputs.bLED7;
	LED_A.bLED8 = sDOOutputs.bLED8;
	
	*/
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0(ALSTATUSCODE_NOERROR), NOERROR_INWORK
\param      pInputSize  pointer to save the input process data length
\param      pOutputSize  pointer to save the output process data length

\brief    This function calculates the process data sizes from the actual SM-PDO-Assign
            and PDO mapping
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GenerateMapping(UINT16* pInputSize,UINT16* pOutputSize)
{
    UINT16 result = ALSTATUSCODE_NOERROR;
    UINT16 PDOAssignEntryCnt = 0;
    OBJCONST TOBJECT OBJMEM * pPDO = NULL;
    UINT16 PDOSubindex0 = 0;
    UINT32 *pPDOEntry = NULL;
    UINT16 PDOEntryCnt = 0;
    UINT16 InputSize = 0;
    UINT16 OutputSize = 0;

    /*Scan object 0x1C12 RXPDO assign*/
    for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
    {
        pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
        if(pPDO != NULL)
        {
            PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
            for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
            {
/*ECATCHANGE_START(V5.12) ECAT1 */
                pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
/*ECATCHANGE_END(V5.12) ECAT1 */
                /* we increment the expected output size depending on the mapped Entry */
                OutputSize += (UINT16) ((*pPDOEntry) & 0xFF);
            }
        }
        else
        {
            //assigned PDO was not found in object dictionary. return invalid mapping
            OutputSize = 0;
            result = ALSTATUSCODE_INVALIDOUTPUTMAPPING;
            break;
        }
    }

    OutputSize = (OutputSize + 7) >> 3;
		
    if(result == 0)
    {
        /*Scan Object 0x1C13 TXPDO assign*/
        for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
        {
            pPDO = OBJ_GetObjectHandle(sTxPDOassign.aEntries[PDOAssignEntryCnt]);
            if(pPDO != NULL)
            {
                PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
                for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
                {
/*ECATCHANGE_START(V5.12) ECAT1*/
                    pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
/*ECATCHANGE_END(V5.12) ECAT1*/
                    // we increment the expected output size depending on the mapped Entry
                    InputSize += (UINT16) ((*pPDOEntry) & 0xFF);
                }
            }
            else
            {
                /*assigned PDO was not found in object dictionary. return invalid mapping*/
                InputSize = 0;
                result = ALSTATUSCODE_INVALIDINPUTMAPPING;
                break;
            }
        }
    }
    InputSize = (InputSize + 7) >> 3;

    *pInputSize = InputSize;
    *pOutputSize = OutputSize;
    return result;

}


/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to input process data
\brief      This function will copies the inputs from the local memory to the ESC memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_InputMapping(UINT16* pData)//YXY_NOTE:MASTER<-SLAVE
{
	
    UINT16 j = 0;
    UINT16 *pTmpData = (UINT16 *)pData;
		//LaserTrackerPDO1A00 *pInputs = (LaserTrackerPDO1A00 *)pTmpData;
	LuStagePDO1A00 *pInputs = (LuStagePDO1A00 *)pTmpData;

    /* we go through all entries of the TxPDO Assign object to get the assigned TxPDOs */
   for (j = 0; j < sTxPDOassign.u16SubIndex0; j++)
   {
      switch (sTxPDOassign.aEntries[j])
      {
      /* TxPDO 1 */
      case 0x1A00:
				pInputs->Symbol0 = SWAPWORD(sPVInputs.Symbol0);
				pInputs->Symbol1 = SWAPWORD(sPVInputs.Symbol1);
				pInputs->Symbol2 = SWAPWORD(sPVInputs.Synbol2);
				//pInputs->ObjlazerTrackerData_positionX = SWAPWORD(sPVInputs.flazerTrackerData_positionX);
				//pInputs->ObjlazerTrackerData_positionY = SWAPWORD(sPVInputs.flazerTrackerData_positionY);
				//pInputs->ObjlazerTrackerData_positionZ = SWAPWORD(sPVInputs.flazerTrackerData_positionZ);
				//pInputs->ObjlazerTrackerData_angleA = SWAPWORD(sPVInputs.flazerTrackerData_angleA);
				//pInputs->ObjlazerTrackerData_angleB = SWAPWORD(sPVInputs.flazerTrackerData_angleB);
				//pInputs->ObjlazerTrackerData_angleB = SWAPWORD(sPVInputs.flazerTrackerData_angleB);
				//pInputs->ObjlazerTrackerData_angleB = SWAPWORD(sPVInputs.flazerTrackerData_angleB);
        break;
      }
   }
	 
	 //MEMCPY(pData, &sPVInputs, SIZEOF(sPVInputs));
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to output process data

\brief    This function will copies the outputs from the ESC memory to the local memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_OutputMapping(UINT16* pData)//YXY_NOTE:MASTER->SLAVE
{

    UINT16 j = 0;
    UINT16 *pTmpData = (UINT16 *)pData;

    /* we go through all entries of the RxPDO Assign object to get the assigned RxPDOs */
    for (j = 0; j < sRxPDOassign.u16SubIndex0; j++)
    {
        switch (sRxPDOassign.aEntries[j])
        {
        /* RxPDO 2 */
        case 0x1601:
            //((UINT16 *) &sDOOutputs)[1] = SWAPWORD(*pTmpData++);
						//sDOOutputs = SWAPWORD(*pTmpData++);
						MEMCPY(&sDOOutputs.Voltage0, pTmpData, 4);
						pTmpData += 2;
						MEMCPY(&sDOOutputs.Voltage1, pTmpData, 4);
						pTmpData += 2;
						MEMCPY(&sDOOutputs.Voltage2, pTmpData, 4);
				/*
						pTmpData += 2;
						sDOOutputs.Dig0 = (UINT8)(pTmpData[0] & 0xFF);
						sDOOutputs.Dig1 = (UINT8)(pTmpData[0] >> 8);
						sDOOutputs.Dig2 = (UINT8)(pTmpData[1] & 0xFF);
						sDOOutputs.Light = (UINT8)(pTmpData[1] >> 8);
						sDOOutputs.LED = (UINT8)(pTmpData[2] & 0xFF);
				*/
            break;
        }
    }
	
	//MEMCPY(&sDOOutputs, pData, SIZEOF(sDOOutputs));
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function will called from the synchronisation ISR 
            or from the mainloop if no synchronisation is supported
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_Application(void)
{
	voltage[0] = sDOOutputs.Voltage0;
	voltage[1] = sDOOutputs.Voltage1;
	voltage[2] = sDOOutputs.Voltage2;
	/*
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	*/
	
	//Debug
	
	//LM3409 Control according to the voltage
	for(uint8_t i = 0; i < 3; i++)
	{
		LM3409_Control(i, (voltage[i] < 0.05) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	}
	
//	MCP4728_WriteReg_Single(&hi2c1, CHANNEL_C, voltage[0]);
//	MCP4728_WriteReg_Single(&hi2c1, CHANNEL_B, voltage[1]);
//	MCP4728_WriteReg_Single(&hi2c1, CHANNEL_A, voltage[2]);
	MCP4728_WriteReg_Multi(&hi2c1, channel, voltage);
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This is the main function

*////////////////////////////////////////////////////////////////////////////////////////
/*
int main(void)
{
    // initialize the Hardware and the EtherCAT Slave Controller
    HW_Init();

    MainInit();

    bRunApplication = TRUE;
    do
    {
        MainLoop();

    } while (bRunApplication == TRUE);

    HW_Release();
    return 0;
}
*/

/** @} */
