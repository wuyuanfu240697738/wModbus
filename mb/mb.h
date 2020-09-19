/*
********************************************************************************************************
*                                                wModbus
*                                 The Embedded Modbus Stack Software
*
*               Copyright: Wu YuanFu. All rights reserved.Protected by international copyright laws.
*
* Programmer  : Wu YuanFu
* Version     : V2.50
* LICENSING TERMS:
* ---------------
*           wModbus is provided in source form for FREE short-term evaluation, for educational use or
*           for peaceful research.  If you plan or intend to use wModbus in a commercial application/
*           product then, you need to contact Wu YuanFu<240697738@qq.com> to properly license wModbus
*           for its use in your application/product.  The fact that the source is provided does NOT 
*           mean that you can use it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*               
********************************************************************************************************
*/

/* Filename    : mb.h*/

#include "mbtype.h"

#if ( MB_SLAVE_ENABLED  == MBENABLED)
#include "smbbuf.h"
#endif

#ifndef _MB_H
#define _MB_H
#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

#if ( MB_MASTER_ENABLED == MBENABLED || MB_SLAVE_ENABLED== MBENABLED )
void         MB_UtilSetBits( USHORT * pWordBuf, USHORT BitOffset,UCHAR NBits, USHORT SetValue );
USHORT       MB_UtilGetBits( USHORT * pWordBuf, USHORT BitOffset,UCHAR NBits );
UCHAR        MB_Receive(wMB *p_mb,UCHAR* pRevBuf,USHORT RecvCnt);
USHORT       MB_Send(wMB *p_mb,UCHAR* pSndBuf);
UCHAR        MB_MutexInit( wMB *p_mb , void* mutex);
UCHAR        MB_MutexClose( wMB *p_mb );
UCHAR        MB_MutexPend( wMB *p_mb);
UCHAR        MB_MutexPost( wMB *p_mb );

//////////////////////////////////////////////////////////////////////////////////////

UCHAR        MB_Init(wMB *p_mb,MB_ModeType Mode,UCHAR Address,void *Parent,void* mutex);
void         MB_PortSet(wMB *p_mb,MBPortTransmit TxCpltPtr,MBPortReceive RxCpltPtr);
UCHAR        MB_Close( wMB *p_mb);
MB_Exception MB_Poll(wMB *p_mb,UCHAR* pBuf,LONG lTimeOut);

#if (MB_STAT_ENABLED == MBENABLED)
ULONG MB_STATGet(wMB *p_mb,MB_StatType StatType);         
#endif 

#if ( MB_SLAVE_ENABLED  == MBENABLED)
USHORT sMB_Deal(wMB *p_mb,UCHAR* pBuf,USHORT length);
USHORT* sMB_GetpBuf(USHORT addr,USHORT num);
void   sMB_RegHoldingWriteCallback(USHORT addr,USHORT value);
void   sMB_RegCoilsWriteCallback( USHORT addr,UCHAR NBits,USHORT value);
#endif

#if (MB_MASTER_ENABLED  == MBENABLED)
MB_Exception mMB_Deal(wMB *p_mb,UCHAR* pBuf,USHORT length);

#if MB_FN01_READ_COILS_ENABLED == MBENABLED
MB_Exception mMB_FN01_Read_CoilsRegister(wMB *p_mb,UCHAR SlaveAddr,USHORT RegAddr,USHORT RegCnt,USHORT *pRegBuffer,USHORT ReadBitOffset,LONG lTimeOut);
#endif
#if MB_FN02_READ_DISCRETE_ENABLED == MBENABLED
MB_Exception mMB_FN02_Read_DiscreteRegister(wMB *p_mb,UCHAR SlaveAddr,USHORT RegAddr,USHORT RegCnt,USHORT *pRegBuffer,USHORT ReadBitOffset,LONG lTimeOut);
#endif
#if MB_FN03_READ_HOLDING_ENABLED == MBENABLED
MB_Exception mMB_FN03_Read_HoldingRegister(wMB *p_mb,UCHAR SlaveAddr, USHORT RegAddr,USHORT RegCnt, USHORT *pRegBuffer,LONG lTimeOut);
#endif
#if MB_FN04_READ_INPUT_ENABLED == MBENABLED
MB_Exception mMB_FN04_Read_InputRegister(wMB *p_mb,UCHAR SlaveAddr,USHORT  RegAddr,USHORT RegCnt,USHORT *pRegBuffer,LONG lTimeOut);                                             
#endif
#if MB_FN05_WRITE_COIL_ENABLED == MBENABLED
MB_Exception mMB_FN05_Write_CoilsRegister(wMB *p_mb,UCHAR SlaveAddr, USHORT RegAddr,USHORT WriteData,LONG lTimeOut);
#endif
#if MB_FN06_WRITE_HOLDING_ENABLED == MBENABLED
MB_Exception mMB_FN06_Write_HoldingRegister(wMB *p_mb,UCHAR SlaveAddr,USHORT RegAddr,USHORT WriteData,LONG lTimeOut);                                                  
#endif 
#if MB_FN15_WRITE_COILS_ENABLED == MBENABLED
MB_Exception mMB_FN15_Write_CoilsRegister(wMB *p_mb,UCHAR SlaveAddr,USHORT RegAddr,USHORT RegCnt,USHORT *pRegBuffer,USHORT WriteBitOffset,LONG lTimeOut);                                               
#endif
#if MB_FN16_WRITE_HOLDING_ENABLED == MBENABLED
MB_Exception mMB_FN16_Write_HoldingRegister(wMB *p_mb,UCHAR SlaveAddr,USHORT RegAddr,USHORT RegCnt,USHORT *pRegBuffer,LONG lTimeOut );                                                 
#endif
#if MB_FN23_READWRITE_HOLDING_ENABLED == MBENABLED
MB_Exception mMB_FN23_ReadWrite_HoldingRegister(wMB *p_mb,UCHAR SlaveAddr,USHORT ReadRegAddr,USHORT ReadRegCnt,USHORT *pReadRegBuffer,USHORT WriteRegAddr,USHORT WriteRegCnt,USHORT *pWriteRegBuffer,LONG lTimeOut);
#endif

#endif

#endif
#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
