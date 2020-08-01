/*
********************************************************************************************************
*                                                wModbus
*                                       The Embedded Modbus Stack
*
*               Copyright: Wu HaiMeng. All rights reserved.Protected by international copyright laws.
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

/* Filename    : mbconfig.h*/

#ifndef _MB_CONFIG_H
#define _MB_CONFIG_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

#define MBDISABLED                              ( 0 )
#define MBENABLED                               ( 1 )

#define MB_ADDRESS_BROADCAST    				( 0  )
#define MB_ADDRESS_MIN          				( 1  )
#define MB_ADDRESS_MAX          				( 247)  

#define MB_MASTER_ENABLED                       MBENABLED
#define MB_SLAVE_ENABLED                        MBENABLED

#define MB_TCP_ENABLED                          MBENABLED
#define MB_RTU_ENABLED                          MBENABLED
#define MB_ASCII_ENABLED                        MBENABLED

#define MB_FN01_READ_COILS_ENABLED              MBENABLED
#define MB_FN02_READ_DISCRETE_ENABLED           MBENABLED
#define MB_FN03_READ_HOLDING_ENABLED            MBENABLED
#define MB_FN04_READ_INPUT_ENABLED              MBENABLED
#define MB_FN05_WRITE_COIL_ENABLED              MBENABLED
#define MB_FN06_WRITE_HOLDING_ENABLED           MBENABLED
#define MB_FN15_WRITE_COILS_ENABLED             MBENABLED
#define MB_FN16_WRITE_HOLDING_ENABLED           MBENABLED
#define MB_FN17_OTHER_REP_SLAVEID_ENABLED       MBENABLED
#define MB_FN23_READWRITE_HOLDING_ENABLED       MBENABLED

#define MB_STAT_ENABLED                         MBENABLED
#define MB_BROADCASTRESPOND_ENABLED             MBDISABLED

#ifdef __cplusplus
    PR_END_EXTERN_C
#endif
#endif
