/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
 * \addtogroup Appl application
 * @{
 * This application includes two functional modules:<br>
 * Digital Outputs<br>
 * Analog Input<br>
 * The objects of the modules are defined according the ETG.5001.<br>
 */

/**
\file appl.h
\author EthercatSSC@beckhoff.com
\brief EL9800 Application defines, typedefs and global variables

\version 5.01

<br>Changes to version - :<br>
V5.01 : Start file change log
 */

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "ecatappl.h"


#ifndef _APPL_H_
#define _APPL_H_

/*-----------------------------------------------------------------------------------------
------
------    Defines and Typedef
------
-----------------------------------------------------------------------------------------*/
//YXY_CHANGE:PDO
//CHANGE_START
/**
 * \addtogroup PdoMappingObjects PDO Mapping Objects
 *
 * Input PDO mapping : 0x1A00<br>
 * Output PDO mapping : 0x1601<br>
 * @{
 */
 
  
/** \brief 0x1601 (Digital output RxPDO) data structure*/
#define NUM_1601 3
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0; /**< \brief SubIndex 0*/
   UINT32   aEntries[NUM_1601]; /**< \brief Entry buffer*/
} OBJ_STRUCT_PACKED_END
TOBJ1601;

//#define NUM_1A00      8
#define NUM_1A00      3

/** \brief 0x1A00 (Digital input TxPDO) data structure*/
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0; /**< \brief SubIndex 0*/
   UINT32 		aEntries[NUM_1A00];
} OBJ_STRUCT_PACKED_END
TOBJ1A00;

//CHANGE_END


/** @}*/

//YXY_CHANGE:PDO
//CHANGE_START
#define NUM_1C12    0x01
#define NUM_1C13    0x01

/**
 * \addtogroup SmAssignObjects SyncManager Assignment Objects
 * SyncManager 2 : 0x1C12 
 * <br>SyncManager 3 : 0x1C13
 * @{
 */
/** \brief 0x1C12 (SyncManager 2 assignment) data structure*/
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0; /**< \brief SubIndex 0*/
   UINT16   aEntries[NUM_1C12]; /**< \brief Entry buffer*/
} OBJ_STRUCT_PACKED_END
TOBJ1C12;


/** \brief 0x1C13 (SyncManager 3 assignment) data structure*/
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0; /**< \brief SubIndex 0*/
   UINT16   aEntries[NUM_1C13]; /**< \brief Entry buffer*/
} OBJ_STRUCT_PACKED_END
TOBJ1C13;
/** @}*/


/**
 * \addtogroup PDO Process Data Objects
 * Digital Outputs : 0x7020
 * @{
 */
/** \brief 0x7010 (Digital output object) data structure*/
#define NUM_7010		3
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0; /**< \brief SubIndex 0*/
	 REAL32		Voltage0;
	 REAL32		Voltage1;
	 REAL32		Voltage2;
} OBJ_STRUCT_PACKED_END
TOBJ7010;
/** @}*/

//#define NUM_8010	5
//typedef struct OBJ_STRUCT_PACKED_START {
//   UINT16   u16SubIndex0; /**< \brief SubIndex 0*/
//	 UINT8 Dig0;
//	 UINT8 Dig1;
//	 UINT8 Dig2;
//	 UINT8 Light;
//	 UINT8 CONF;
//} OBJ_STRUCT_PACKED_END
//TOBJ8010;


//YXY_CHANGE:ADDPDO:OBJ
//CHANGE_START

/**
 * \addtogroup PDO Process Data Objects
 * REAL32 Inputs : 0x6010<br>
 * @{
 */
/** \brief 0x6010 (Digital input object) data structure*/
#define NUM_6010        3
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0; /**< \brief SubIndex 0*/
	 UINT8		Symbol0;
	 UINT8		Symbol1;
	 UINT8 		Synbol2;
} OBJ_STRUCT_PACKED_END
TOBJ6010;

//typedef struct OBJ_STRUCT_PACKED_START {
//   UINT16   u16SubIndex0; /**< \brief SubIndex 0*/
//   REAL32 flazerTrackerData_positionX; /**< \brief position*/
//   REAL32 flazerTrackerData_positionY; /**< \brief positionY*/
//   REAL32 flazerTrackerData_positionZ; /**< \brief positionZ*/
//   REAL32 flazerTrackerData_angleA; /**< \brief angleA*/
//   REAL32 flazerTrackerData_angleB; /**< \brief angleB*/
//   REAL32 flazerTrackerData_angleC; /**< \brief angleC*/
//   REAL32 flazerTrackerData_angleD; /**< \brief angleD*/
//   ALIGN8(SubIndex009) /**< \brief 8Bit alignment*/
//} OBJ_STRUCT_PACKED_END
//TOBJ6010;


/** @}*/

//CHANGE_END


/**
 * \addtogroup DeviceParaObjects Device Parameter Objects
 * Modular Device Profile: 0xF000
 * @{
 */
/** \brief 0xF000 (Modular Device Profile) data structure*/
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0; /**< \brief SubIndex0*/
   UINT16   u16Moduleindexdistance; /**< \brief Module Index distance
                                     * 
                                     * Index distance between two modules (maximum number of objects per module and area)<br>
                                     * Default: 0x10*/
   UINT16   u16Maximumnumberofmodules; /**< \brief Maximum number of modules*/
} OBJ_STRUCT_PACKED_END
TOBJF000;

//YXY_CHANGE:PDO
//CHANGE_START
#define NUM_F010     (NUM_1C12+NUM_1C13)
/** \brief 0xF010 (Module Profile List) data structure*/
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0; /**< \brief SubIndex0*/
   UINT32   aEntries[NUM_F010]; /**< \brief Module profile information buffer
                          * 
                          * Bit 0..15: Profile number of the module on position 1<br>
                          * Bit 16..31: Profile specific*/
} OBJ_STRUCT_PACKED_END
TOBJF010;
/** @}*/

#endif //_APPL_H_

#ifdef _EVALBOARD_
    #define PROTO
#else
    #define PROTO extern
#endif


#ifdef _OBJD_
/**
 * \addtogroup SmAssignObjects SyncManager Assignment Objects
 * @{
 */
/**
 * \brief Entry descriptions of SyncManager assign objects
 *
 * SubIndex0<br>
 * SubIndex1 (for all other entries the same description will be used (because the object code is ARRAY))
 */
OBJCONST TSDOINFOENTRYDESC    OBJMEM asPDOAssignEntryDesc[] = {
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ}};
/** @}*/

   
/**
 * \addtogroup EnumObjects Enum Objects
 * @{
 * Presentation (Signed/Unsigned) : 0x800
 */

/*---------------------------------------------
-    0x0800: ENUM (Signed/Unsigned Presentation)
-----------------------------------------------*/
CHAR sEnum0800_Value00[] = "\000\000\000\000Signed presentation"; /**< \brief Value = 0x00, Text = Signed presentation */
CHAR sEnum0800_Value01[] = "\001\000\000\000Unsigned presentation"; /**< \brief Value = 0x01, Text = Unsigned presentation */
CHAR *apEnum0800[2] = { sEnum0800_Value00, sEnum0800_Value01};/**< \brief List of Enum (0x800) values*/

/**
 * \brief Enum entry description
 *
 * SubIndex0<br>
 * Enum (Signed Presentation)
 * enum (Unsigned Presentation)
 */
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x0800[] =
   {{DEFTYPE_UNSIGNED8, 8, ACCESS_READ | OBJACCESS_NOPDOMAPPING},
    {DEFTYPE_OCTETSTRING, 8*SIZEOF(sEnum0800_Value00), ACCESS_READ | OBJACCESS_NOPDOMAPPING},
   {DEFTYPE_OCTETSTRING, 8*SIZEOF(sEnum0800_Value01), ACCESS_READ | OBJACCESS_NOPDOMAPPING}};
/** @}*/


/**
* \addtogroup PdoMappingObjects PDO Mapping Objects
* @{
*/
/**
 * \brief Object 0x1601 (Digital output RxPDO) entry descriptions
 * 
 * SubIndex 0 : read only<br>
 * SubIndex x : read only<br>
 *  (x > 0)
 */
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1601[] = {
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ },
	 {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}
};

/**
 * \brief Object 0x1601 (Digital output RxPDO) name
 *
 * In this example no specific entry name is defined ("SubIndex xxx" is used)
 */
OBJCONST UCHAR OBJMEM aName0x1601[] = "RxPDO-Map\000\377";
#endif //#ifdef _OBJD_


/**
 * \brief Object 0x1601 (Digital output RxPDO) variable to handle object data
 * 
 * SubIndex 0 : 1<br>
 * SubIndex 1 : 0x7010.1 1bit (Reference to LED1)<br>
 * SubIndex 2 : 0x7010.1 1bit (Reference to LED2)<br>
 * SubIndex 3 : 0x7010.1 1bit (Reference to LED3)<br>
 * SubIndex 4 : 0x7010.1 1bit (Reference to LED4)<br>
 * SubIndex 5 : 0x7010.1 1bit (Reference to LED5), only for PIC24<br>
 * SubIndex 6 : 0x7010.1 1bit (Reference to LED6), only for PIC24<br>
 * SubIndex 7 : 0x7010.1 1bit (Reference to LED7), only for PIC24<br>
 * SubIndex 8 : 0x7010.1 1bit (Reference to LED8), only for PIC24
 */
PROTO TOBJ1601 sDORxPDOMap
#ifdef _EVALBOARD_
 = {NUM_1601, {0x70100120, 0x70100220, 0x70100320}}
#endif
;
/** @}*/


/**
 * \addtogroup PdoMapping PDO Mapping Objects
 * @{
 */
/**
 * \brief Object 0x1A00 (Input TxPDO) entry descriptions
 *
 * SubIndex 0 : read only<br>
 * SubIndex x : read only<br>
 *  (x > 0)
*/
#ifdef _OBJD_
/*
OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1A00[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ },
   {DEFTYPE_REAL32, 0x20, ACCESS_READ},
   {DEFTYPE_REAL32, 0x20, ACCESS_READ},
   {DEFTYPE_REAL32, 0x20, ACCESS_READ},
   {DEFTYPE_REAL32, 0x20, ACCESS_READ},
   {DEFTYPE_REAL32, 0x20, ACCESS_READ},
   {DEFTYPE_REAL32, 0x20, ACCESS_READ},
   {DEFTYPE_REAL32, 0x20, ACCESS_READ}
	 };
	*/

OBJCONST TSDOINFOENTRYDESC	OBJMEM asEntryDesc0x1A00[] = {
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ },
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}
	 };

/**
 * \brief Object 0x1A00 (Input TxPDO) object and entry names
 *
 * In this example no specific entry name is defined ("SubIndex xxx" is used)
 */
OBJCONST UCHAR OBJMEM aName0x1A00[] = "TxPDO-Map\000\377";
#endif //#ifdef _OBJD_


/**
 * \brief Object 0x1A00 (Input TxPDO) variable to handle object data
 * 
 * SubIndex 0 : 1<br>
 * SubIndex 1 : 0x6020.1 REAL32 (Reference to flazerTrackerData_positionX)
 * SubIndex 2 : 0x6020.2 REAL32 (Reference to flazerTrackerData_positionY)
 * SubIndex 3 : 0x6020.3 REAL32 (Reference to flazerTrackerData_positionZ)
 * SubIndex 4 : 0x6020.4 REAL32 (Reference to flazerTrackerData_angleA)
 * SubIndex 5 : 0x6020.5 REAL32 (Reference to flazerTrackerData_angleB)
 * SubIndex 6 : 0x6020.6 REAL32 (Reference to flazerTrackerData_angleC)
 * SubIndex 7 : 0x6020.7 REAL32 (Reference to flazerTrackerData_angleD)
 */
PROTO TOBJ1A00 sITxPDOMap
#ifdef _EVALBOARD_
= {NUM_1A00, {0x60100108, 0x60100208, 0x60100308}}
//= {NUM_1A00, {0x60100120, 0x60100220, 0x60100320, 0x60100420,0x60100520, 0x60100620, 0x60100720, 0x08}}
#endif
;


/** @}*/


/**
 * \addtogroup PDO Process Data Objects
 * @{
 */
/** \brief Data structure to handle the process data transmitted via 0x1A00 (csp/csv TxPDO)*/
typedef struct STRUCT_PACKED_START
{
    REAL32 ObjlazerTrackerData_positionX;    /**< \brief Lazer Tracker Data Position X*/
    REAL32 ObjlazerTrackerData_positionY;    /**< \brief Lazer Tracker Data Position Y*/
    REAL32 ObjlazerTrackerData_positionZ;    /**< \brief Lazer Tracker Data Position Z*/
    REAL32 ObjlazerTrackerData_angleA;       /**< \brief Lazer Tracker Data Angle A*/
    REAL32 ObjlazerTrackerData_angleB;       /**< \brief Lazer Tracker Data Angle B*/
		REAL32 ObjlazerTrackerData_angleC;       /**< \brief Lazer Tracker Data Angle C*/
		REAL32 ObjlazerTrackerData_angleD;       /**< \brief Lazer Tracker Data Angle D*/
}STRUCT_PACKED_END
LaserTrackerPDO1A00;

typedef struct STRUCT_PACKED_START
{
		UINT8 Symbol0;
		UINT8 Symbol1;
	  UINT8 Symbol2;
}STRUCT_PACKED_END
LuStagePDO1A00;

/** @}*/

/**
 * \addtogroup SmAssignObjects SyncManager Assignment Objects
 * @{
 */
#ifdef _OBJD_
/**
 * \brief 0x1C12 (SyncManager 2 assignment) object name 
 * 
 * No entry names defined because the object code is ARRAY and all entry names are "SubIndex 000"
 */
OBJCONST UCHAR OBJMEM aName0x1C12[] = "RxPDO assign";
#endif //#ifdef _OBJD_


/**
 * \brief 0x1C12 (SyncManager 2 assignment) variable to handle object data
 * 
 * SubIndex 0 : 1<br>
 * SubIndex 1 : 0x1601
 */
PROTO TOBJ1C12 sRxPDOassign
#ifdef _EVALBOARD_
= {0x01, {0x1601}}
#endif
;


#ifdef _OBJD_
/**
 * \brief 0x1C13 (SyncManager 3 assignment) object name 
 * 
 * No entry names defined because the object code is ARRAY and all entry names are "SubIndex 000"
 */
OBJCONST UCHAR OBJMEM aName0x1C13[] = "TxPDO assign";
#endif //#ifdef _OBJD_

//YXY_CHANGE:PDO
//CHANGE_SATRT

/**
 * \brief 0x1C13 (SyncManager 3 assignment) variable to handle object data
 * 
 * SubIndex 0 : 1<br>
 * SubIndex 1 : 0x1A00<br>
 */
PROTO TOBJ1C13 sTxPDOassign
#ifdef _EVALBOARD_
= {0x01, {0x1A00}}
#endif
;

//CHANGE_END

/** @}*/


/**
 * \addtogroup PDO Process Data Objects
 * @{
 */
#ifdef _OBJD_
/**
 * \brief Object 0x6010 (Input object) entry descriptions
 *
 * SubIndex 0 : read only<br>
 * SubIndex x : (One description for each switch) read only and TxPdo mappable<br>
 *  (x > 0)
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6010[] = {
//   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
	{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ},
	{DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ | OBJACCESS_TXPDOMAPPING},    //Symbol0
	{DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ | OBJACCESS_TXPDOMAPPING},    //Symbol1
	{DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ | OBJACCESS_TXPDOMAPPING}    //Symbol1
   //{0x0000, 0x08, 0}
}; /* Subindex 008 for align */

/**
 * \brief 0x6010 (Input object) object and entry names
 */
OBJCONST UCHAR OBJMEM aName0x6010[] = "Lustage Data Iutputs\000Symbol0\000Symbol1\000ymbol2\000\377";
#endif //#ifdef _OBJD_


/**
 * \brief 0x6010 (Input object) variable to handle object data
 * 
 * SubIndex 0 : 1<br>
 * SubIndex x : every position value is 0 by default
 */
PROTO TOBJ6010 sPVInputs
#ifdef _EVALBOARD_
= {NUM_6010, 0, 0, 0}
//= {NUM_6010, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0}
#endif
;

//CHANGE_END

/*
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC OBJMEM sEntryDesc0x60F0 = {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READWRITE};
OBJCONST UCHAR OBJMEM aName0x60F0[] = "LED_SDO";
#endif

PROTO UINT8 sLEDInputs
#ifdef _EVALBOARD_
= 0
#endif
;
*/

/**
 * \brief Object 0x7010 (Digital output object) entry descriptions
 *
 * SubIndex 0 : read only<br>
 * SubIndex x : (One description for each led) read only and RxPdo mappable<br>
 *  (x > 0)
*/
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7010[] = {
	{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ },
	{DEFTYPE_REAL32, 0x20, ACCESS_READ | OBJACCESS_RXPDOMAPPING },   //voltage[3]
	{DEFTYPE_REAL32, 0x20, ACCESS_READ | OBJACCESS_RXPDOMAPPING },
	{DEFTYPE_REAL32, 0x20, ACCESS_READ | OBJACCESS_RXPDOMAPPING }
}; /* Subindex 008 for align */


/**
 * \brief 0x7010 (Digital Ouput object) object and entry names
 */
OBJCONST UCHAR OBJMEM aName0x7010[] = "Lustage Data Outputs\000Voltage0\000Voltage1\000Voltage2\000\377";
#endif //#ifdef _OBJD_

PROTO TOBJ7010 sDOOutputs
#ifdef _EVALBOARD_
= {NUM_7010, 0, 0, 0}
//= {NUM_6010, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0}
#endif
;
/** @}*/

/*
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x8010[] = {
	{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ },
	{DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE},
	{DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE},
	{DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE},
	{DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE},
	{DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE}
};

OBJCONST UCHAR OBJMEM aName0x8010[] = "Digit SDO\000Dig0\000Dig1\000Dig2\000Light\000CONF\000\377";
#endif

PROTO TOBJ8010 Digits_SDO0x8010
#ifdef _EVALBOARD_
= {NUM_8010, SEG_DIG_0, SEG_DIG_0, SEG_DIG_0, TM1650_BRIGHT5, 0};
#endif
;
*/

/*
#ifdef _OBJD_
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x8010 = {
	DEFTYPE_UNSIGNED8 , 0x08 , ACCESS_READWRITE
};

OBJCONST UCHAR OBJMEM aName0x8010[] = "LEDSSDO\000\377";
#endif

PROTO TOBJ8010 LED_SDO0x8010
#ifdef _EVALBOARD_
= 0
#endif
;
*/

/**
 * \addtogroup DeviceParaObjects Device Parameter Objects
 * @{
 */
#ifdef _OBJD_
/** 
 * \brief 0xF000 (Modular Device Profile) entry descriptions
 *
 * Subindex 000<br>
 * SubIndex 001: Module index distance<br>
 * SubIndex 002: Maximum number of modules<br>
 */
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF000[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ },
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ},
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ}};


/** \brief 0xF000 (Modular Device Profile) object and entry names*/
OBJCONST UCHAR OBJMEM aName0xF000[] = "Modular Device Profile\000Index distance\000Maximum number of modules\000\377";
#endif


/**
 * \brief 0xF000 (Modular Device Profile) variable to handle the object data
 *
 * SubIndex 0
 * SubIndex 1 (Module Index distance) : 0x10
 * SubIndex 2 (Maximum number of Modules) : 2
 */
/******************************************************************************
*                    Object 0xF000
******************************************************************************/
PROTO TOBJF000 sModulardeviceprofile
#ifdef _EVALBOARD_
= {2, 0x10, 0x03}
#endif
;


#ifdef _OBJD_
/** 
 * \brief 0xF010 (Module profile list) entry descriptions
 *
 * Subindex 0<br>
 * SubIndex x
 * (x > 0)
 */
 
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF010[] = {
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}};

/**
 * \brief 0xF010 (Module profile list) object name
 *
 * no entry names defined because the object is an array (for every entry > 0 "SubIndex xxx" is returned)
 */
OBJCONST UCHAR OBJMEM aName0xF010[] = "Module Profile List";
#endif


/**
 *\brief 0xF010 (Module profile list) variable to handle object data
 * 
 * SubIndex 0 : 3
 * SubIndex 1 : 100 (Input SubProfile)
 * SubIndex 2 : 200 (Digital Output SubProfile)
 */

PROTO TOBJF010 sModulelist
#ifdef _EVALBOARD_
= {NUM_F010,{0x64, 0xC8}}
#endif
;

//CHANGE_END

/** @}*/


#ifdef _OBJD_

/*if _PIC18 is enabled the object dictionary is fixed defined in coeappl.c*/

/**
 *\brief EL9800 Application specific object dictionary
 * 
 */
TOBJECT    OBJMEM ApplicationObjDic[] = {
   /* Enum 0x0800 */
   {NULL,NULL, 0x0800, {DEFTYPE_ENUM, 0x02 | (OBJCODE_REC << 8)}, asEntryDesc0x0800, 0, apEnum0800 },
   /* Object 0x1601 */
   {NULL,NULL,  0x1601, {DEFTYPE_PDOMAPPING, NUM_1601 | (OBJCODE_REC << 8)}, asEntryDesc0x1601, aName0x1601, &sDORxPDOMap, NULL, NULL, 0x0000 },
	 /* Object 0x1A00 */
   {NULL,NULL,   0x1A00, {DEFTYPE_PDOMAPPING, NUM_1A00 | (OBJCODE_REC << 8)}, asEntryDesc0x1A00, aName0x1A00, &sITxPDOMap, NULL, NULL, 0x0000 },
		/* Object 0x1C12 */
   {NULL,NULL,   0x1C12, {DEFTYPE_UNSIGNED16, NUM_1C12 | (OBJCODE_ARR << 8)}, asPDOAssignEntryDesc, aName0x1C12, &sRxPDOassign, NULL, NULL, 0x0000 },
		/* Object 0x1C13 */
   {NULL,NULL,   0x1C13, {DEFTYPE_UNSIGNED16, NUM_1C13 | (OBJCODE_ARR << 8)}, asPDOAssignEntryDesc, aName0x1C13, &sTxPDOassign, NULL, NULL, 0x0000 },
//YXY_CHANGE:ADDPDO   
//CHANGE_START   
	 /* Object 0x6010 */
   {NULL,NULL,   0x6010, {DEFTYPE_RECORD, NUM_6010 | (OBJCODE_REC << 8)}, asEntryDesc0x6010, aName0x6010, &sPVInputs, NULL, NULL, 0x0000 },
//CHANGE_END     
	 /* Object 0x7010 */
   {NULL,NULL,   0x7010, {DEFTYPE_RECORD, NUM_7010 | (OBJCODE_REC << 8)}, asEntryDesc0x7010, aName0x7010, &sDOOutputs, NULL, NULL, 0x0000 },
		/* Object 0xF000 */
	//{NULL , NULL ,  0x8020 , {DEFTYPE_UNSIGNED8 , 0 | (OBJCODE_VAR << 8)} , &sEntryDesc0x8020 , aName0x8020 , &LED_SDO0x8020 , NULL , NULL , 0x0000 },
	// {NULL , NULL ,  0x8010 , {DEFTYPE_RECORD , NUM_8010 | (OBJCODE_REC << 8)} , sEntryDesc0x8010 , aName0x8010 , &Digits_SDO0x8010 , NULL , NULL , 0x0000 },
	 //{NULL, NULL, 0x60F0, {DEFTYPE_UNSIGNED8, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x60F0, aName0x60F0, &sLEDInputs, NULL, NULL, 0x0000},
   {NULL,NULL,   0xF000, {DEFTYPE_RECORD, 2 | (OBJCODE_REC << 8)}, asEntryDesc0xF000, aName0xF000, &sModulardeviceprofile, NULL, NULL, 0x0000 },
	 /* Object 0xF010 */
   {NULL,NULL,   0xF010, {DEFTYPE_UNSIGNED32, NUM_F010 | (OBJCODE_ARR << 8)}, asEntryDesc0xF010, aName0xF010, &sModulelist, NULL, NULL, 0x0000 },
	 {NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};
#endif    //#ifdef _OBJD_

PROTO void APPL_Application(void);

PROTO void   APPL_AckErrorInd(UINT16 stateTrans);
PROTO UINT16 APPL_StartMailboxHandler(void);
PROTO UINT16 APPL_StopMailboxHandler(void);
PROTO UINT16 APPL_StartInputHandler(UINT16 *pIntMask);
PROTO UINT16 APPL_StopInputHandler(void);
PROTO UINT16 APPL_StartOutputHandler(void);
PROTO UINT16 APPL_StopOutputHandler(void);

PROTO UINT16 APPL_GenerateMapping(UINT16 *pInputSize,UINT16 *pOutputSize);
PROTO void APPL_InputMapping(UINT16* pData);
PROTO void APPL_OutputMapping(UINT16* pData);


#undef PROTO
/** @}*/
