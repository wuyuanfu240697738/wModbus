

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

/* Filename    : mbbuf.h*/

#include "mbtype.h"

#ifndef _MB_BUF_H
#define _MB_BUF_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

#if ( MB_SLAVE_ENABLED== MBENABLED )

MB_Exception MB_SetBufs(USHORT addr,USHORT* dst,USHORT num);
MB_Exception MB_GetBufs(USHORT addr,USHORT* dst,USHORT num);
MB_Exception MB_SetBuf(USHORT addr,USHORT dst);
USHORT MB_GetBuf(USHORT addr);

    #define sMB_HOLDING_START 	                0x1000      //HOLDING起始地址
    #define sMB_HOLDING_NREGS 	                512  
    #define sMB_INPUT_START 	                0x9000      //INPUT起始地址
    #define sMB_INPUT_NREGS 	                16
    #define sMB_COILS_START 	                0x0500      //COILS起始地址
    #define sMB_COILS_NREGS 	                64 
    #define sMB_DISCRETE_START 	                0x0400      //DISCRETE起始地址
    #define sMB_DISCRETE_NREGS 	                64
    
#endif

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
