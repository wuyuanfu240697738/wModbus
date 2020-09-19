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

/* Filename    : mbbuf.c*/

#include "smbbuf.h"

#if ( MB_SLAVE_ENABLED== MBENABLED )

	#define sMB_HoldingBuf_Addr 	0                                                       //数量为   sMB_HOLDING_NREGS
	#define sMB_InputBuf_Addr 	    (sMB_HoldingBuf_Addr + sMB_HOLDING_NREGS)               //数量为   sMB_INPUT_NREGS
	#define sMB_CoilsBuf_Addr 	    (sMB_InputBuf_Addr + sMB_INPUT_NREGS)                   //数量为   sMB_COILS_NREGS/MB_BITS + 1
	#define sMB_DiscreteBuf_Addr 	(sMB_CoilsBuf_Addr + sMB_COILS_NREGS/MB_BITS +1)        //数量为   sMB_DISCRETE_NREGS/MB_BITS + 1
    #define sMB_BUF_SIZE            (sMB_DiscreteBuf_Addr + sMB_DISCRETE_NREGS/MB_BITS + 1) //数量为   sMB_DISCRETE_NREGS/MB_BITS + 1
    
    static  USHORT MBBuf[sMB_BUF_SIZE];

/*
********************************************************************************************************
*                                                sMB RegHoldingWrite Callback
*
* Description: 此函数用于RegHoldingWrite回调函数
* Arguments  : addr       RegHolding地址
*            : value      RegHolding地址上的值
* Note(s)    : 1) sMB_RegHoldingCBCallback() 可作为用户hook函数。
********************************************************************************************************
*/
void sMB_RegHoldingWriteCallback(USHORT addr,USHORT value)
{

 
}

/*
********************************************************************************************************
*                                                sMB RegCoilsWrite Callback
*
* Description: 此函数用于RegCoilsWrite回调函数
* Arguments  : addr       RegCoils地址
*            : NBits      写入的位数 <=16
*            : value      RegCoils地址上的值
* Note(s)    : 1) sMB_RegCoilsCBCallback() 可作为用户hook函数。
********************************************************************************************************
*/
void sMB_RegCoilsWriteCallback( USHORT addr,UCHAR NBits,USHORT value)
{


}


USHORT* sMB_GetpBuf(USHORT addr,USHORT num)
{
    USHORT* pBuf;
    
    if((addr >= sMB_HOLDING_START ) && ((addr + num) <= (sMB_HOLDING_START+ sMB_HOLDING_NREGS)))
    {
        pBuf = &MBBuf[addr - sMB_HOLDING_START + sMB_HoldingBuf_Addr];
    }
    else if((addr >= sMB_INPUT_START ) && ((addr + num) <= (sMB_INPUT_START+ sMB_INPUT_NREGS)))
    {
        pBuf = &MBBuf[addr - sMB_INPUT_START + sMB_InputBuf_Addr];
    }
    else if((addr >= sMB_COILS_START ) && ((addr + num) <= (sMB_COILS_START+ sMB_COILS_NREGS)))
    {
        pBuf = &MBBuf[sMB_CoilsBuf_Addr];
    }
    else if((addr >= sMB_DISCRETE_START ) && ((addr + num) <= (sMB_DISCRETE_START+ sMB_DISCRETE_NREGS)))
    {
        pBuf = &MBBuf[sMB_DiscreteBuf_Addr];
    }    
    else{return  MBNULL;}
    
    return pBuf;
}

/*
********************************************************************************************************
*                                                  Set Buf
*
* Description: 此函数用于设置数据
* Arguments  : addr           buf数据偏移地址
*              dst            目标数据地址
*              num            数据个数
* Returns    : MBTRUE         成功
*              MBFALSE        失败
* Note(s)    : 1) MB_GetBuf() 
********************************************************************************************************
*/
MB_Exception MB_SetBufs(USHORT addr,USHORT* dst,USHORT num)
{
    USHORT i;
    USHORT* src;
    
    src = sMB_GetpBuf(addr,num);
    if(src != MBNULL)
    {
        for(i=0;i<num;i++)
        {
            src[i] = dst[i];
        }
        return MB_EX_NONE;
    }
    return  MB_EX_ILLEGAL_DATA_ADDRESS;
}

MB_Exception MB_SetBuf(USHORT addr,USHORT dst)
{
    USHORT* src;
    
    src = sMB_GetpBuf(addr,1);
    if(src != MBNULL)
    {
        *src = dst;
        return MB_EX_NONE;
    }
    return  MB_EX_ILLEGAL_DATA_ADDRESS;    
}
/*
********************************************************************************************************
*                                                  Get Buf
*
* Description: 此函数用于获取数据
* Arguments  : addr           buf数据偏移地址
*              dst            目标数据地址
*              num            数据个数
* Returns    : MBTRUE         成功
*              MBFALSE        失败
* Note(s)    : 1) MB_GetBuf() 
********************************************************************************************************
*/
MB_Exception MB_GetBufs(USHORT addr,USHORT* dst,USHORT num)
{
    USHORT i;    
    USHORT* src;
    
    src = sMB_GetpBuf(addr,num);
    if(src != MBNULL)
    {
        for(i=0;i<num;i++)
        {
            dst[i] = src[i];
        }
        return MB_EX_NONE;
    }
    return  MB_EX_ILLEGAL_DATA_ADDRESS;
}

USHORT MB_GetBuf(USHORT addr)
{
    USHORT* src;
    
    src = sMB_GetpBuf(addr,1);
    if(src != MBNULL)
    {
        return *src;
    }
    return  MBNULL;
}

#endif
