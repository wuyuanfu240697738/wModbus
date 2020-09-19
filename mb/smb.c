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

/* Filename    : smb.c*/

#include "mb.h"

#define sMB_PDU_FN01_RcvLen_Min                  ( 5 )
#define sMB_PDU_FN02_RcvLen_Min                  ( 5 )
#define sMB_PDU_FN03_RcvLen_Min                  ( 5 )
#define sMB_PDU_FN04_RcvLen_Min                  ( 5 )
#define sMB_PDU_FN05_RcvLen_Min                  ( 5 )
#define sMB_PDU_FN06_RcvLen_Min                  ( 5 )
#define sMB_PDU_FN15_RcvLen_Min                  ( 7 )
#define sMB_PDU_FN16_RcvLen_Min                  ( 8 )
#define sMB_PDU_FN23_RcvLen_Min                  ( 12 )
#define sMB_GetRegValue( x )    	( USHORT )(( FramePtr[x] << 8 ) | FramePtr[x+ 1])

#if ( MB_SLAVE_ENABLED  == MBENABLED)

/*
********************************************************************************************************
*                                                sMB RegInputCB
*
* Description: 此函数用于从机 RegInput 的数据处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 用于FN04 命令
********************************************************************************************************
*/
static MB_Exception sMB_RegInputCB( wMB *p_mb ,UCHAR* pBuf)
{
    UCHAR  *pRegFrame;
    USHORT *sMBpBuf = sMB_GetpBuf(p_mb->RegAddress,p_mb->RegCnt);
	if(sMBpBuf != MBNULL)
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
             pRegFrame= &pBuf[MB_PDU_TCP_OFF + p_mb->Value_off];
        }else
        {pRegFrame = &pBuf[MB_PDU_RTU_OFF + p_mb->Value_off];}         
		
        for(USHORT i = 0;i < p_mb->RegCnt;i++)
        {
            *pRegFrame++ = *sMBpBuf >> 8;
			*pRegFrame++ = *sMBpBuf++ ;  
        }
    }
    else{return  MB_EX_ILLEGAL_DATA_ADDRESS;}
    return  MB_EX_NONE;
}

/*
********************************************************************************************************
*                                                sMB RegHolding
*
* Description: 此函数用于从机 RegHolding 的数据处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 用于FN03 FN06 FN16 FN23 命令
********************************************************************************************************
*/
static MB_Exception sMB_RegHoldingCB( wMB *p_mb ,UCHAR* pBuf)
{
    UCHAR   *pRegFrame; 
    USHORT *sMBpBuf = sMB_GetpBuf(p_mb->RegAddress,p_mb->RegCnt);
	if(sMBpBuf != MBNULL)
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
             pRegFrame= &pBuf[MB_PDU_TCP_OFF + p_mb->Value_off];
        }else
        {pRegFrame = &pBuf[MB_PDU_RTU_OFF + p_mb->Value_off];} 
        switch ( p_mb->ReadWrite )
        {          
        case MBREAD:
            for(USHORT i = 0;i < p_mb->RegCnt;i++)
            {
                *pRegFrame++ = *sMBpBuf >> 8;
                *pRegFrame++ = *sMBpBuf++ ;  
            }
            break;
        case MBWRITE:
            for(USHORT i = 0;i < p_mb->RegCnt;i++)
            {
                *sMBpBuf = *pRegFrame++ << 8;
                *sMBpBuf |= *pRegFrame++;
                sMB_RegHoldingWriteCallback(p_mb->RegAddress + i,*sMBpBuf++);
            }
            break;
        }		
    }
	else{return MB_EX_ILLEGAL_DATA_ADDRESS;	}
    return MB_EX_NONE;
}

/*
********************************************************************************************************
*                                                sMB RegCoilsCB
*
* Description: 此函数用于从机 RegCoilsCB 的数据处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 用于FN01 FN05 FN15命令
********************************************************************************************************
*/
static MB_Exception sMB_RegCoilsCB( wMB *p_mb ,UCHAR* pBuf)
{
    USHORT *pRegFrame;
    USHORT *sMBpBuf = sMB_GetpBuf(p_mb->RegAddress,p_mb->RegCnt);
	if(sMBpBuf != MBNULL)
    {       
        if(p_mb->Mode == MB_MODE_TCP)
        {
             pRegFrame = (USHORT *)&pBuf[MB_PDU_TCP_OFF + p_mb->Value_off];
        }else
        {pRegFrame = (USHORT *)&pBuf[MB_PDU_RTU_OFF + p_mb->Value_off];}          
		USHORT Index = ( p_mb->RegAddress - sMB_COILS_START );        
        SHORT RegCnt = ( SHORT )p_mb->RegCnt;    
        switch ( p_mb->ReadWrite )
        {
        case MBREAD:
            while( RegCnt > 0 )
            {
                UCHAR NBits = ( UCHAR )( RegCnt > MB_BITS ? MB_BITS : RegCnt );
                *pRegFrame++ = MB_UtilGetBits( sMBpBuf, Index,NBits);
                RegCnt -= MB_BITS;
                Index += MB_BITS;
            }
            break;
        
        case MBWRITE:
            while( RegCnt > 0 )
            {
                UCHAR NBits = ( UCHAR )( RegCnt > MB_BITS ? MB_BITS : RegCnt );
                MB_UtilSetBits(sMBpBuf, Index,NBits,*pRegFrame);
                sMB_RegCoilsWriteCallback(Index + sMB_COILS_START,NBits,*pRegFrame);	
                RegCnt -= MB_BITS;
                Index += MB_BITS;
                pRegFrame++;
            }
            break;
        }           
    }        
    else{return MB_EX_ILLEGAL_DATA_ADDRESS;}
    return MB_EX_NONE; 
}

/*
********************************************************************************************************
*                                                sMB RegDiscreteCB
*
* Description: 此函数用于从机 RegDiscreteCB 的数据处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 用于FN02 命令
********************************************************************************************************
*/
static MB_Exception sMB_RegDiscreteCB(wMB *p_mb,UCHAR* pBuf)
{
    USHORT *pRegFrame;
    USHORT *sMBpBuf = sMB_GetpBuf(p_mb->RegAddress,p_mb->RegCnt);
	if(sMBpBuf != MBNULL)
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
             pRegFrame = (USHORT *)&pBuf[MB_PDU_TCP_OFF + p_mb->Value_off];
        }else
        {pRegFrame = (USHORT *)&pBuf[MB_PDU_RTU_OFF + p_mb->Value_off];}          
		USHORT Index = ( p_mb->RegAddress - sMB_DISCRETE_START );         
        SHORT RegCnt = ( SHORT )p_mb->RegCnt;            
        while( RegCnt > 0 )
        {
            UCHAR NBits = ( UCHAR )( RegCnt > MB_BITS ? MB_BITS : RegCnt );
            *pRegFrame++ = MB_UtilGetBits( sMBpBuf, Index,NBits );
            RegCnt -= MB_BITS;
            Index += MB_BITS;
        }         
    }    
    else{return MB_EX_ILLEGAL_DATA_ADDRESS;}
    return MB_EX_NONE;     
}

/*
********************************************************************************************************
*                                                sMB FN01 Handler
*
* Description: 此函数用于从机 FN01 号命令的处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 
********************************************************************************************************
*/
#if MB_FN01_READ_COILS_ENABLED == MBENABLED
static MB_Exception sMB_FN01_Read_CoilsHandler( wMB *p_mb ,UCHAR* pBuf)
{
    UCHAR          *FramePtr;    
    UCHAR           ucNBytes;
    MB_Exception    eStatus = MB_EX_NONE;
    
    if( p_mb->BufCnt == sMB_PDU_FN01_RcvLen_Min )
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
            FramePtr = &pBuf[MB_PDU_TCP_OFF];
        }else
        {FramePtr = &pBuf[MB_PDU_RTU_OFF];}
        p_mb->RegAddress = sMB_GetRegValue( MB_PDU_REQ_ADDR_OFF );
        p_mb->RegCnt   = sMB_GetRegValue( MB_PDU_REQ_CNT_OFF );       
        if((p_mb->RegCnt >= MB_PDU_FNxx_COUNT_MIN ) && (p_mb->RegCnt< MB_PDU_FN01_READCOUNT_MAX))
        {
            if( ( p_mb->RegCnt & 0x0007 ) != 0 )
            {
                ucNBytes = ( UCHAR )( p_mb->RegCnt / 8 + 1 );
            }
            else
            {
                ucNBytes = ( UCHAR )( p_mb->RegCnt / 8 );
            }
            FramePtr[MB_PDU_FN01_BYTECNT_OFF] = ucNBytes;
            p_mb->BufCnt = 2 + ucNBytes;
            p_mb->Value_off = MB_PDU_FN01_VALUE_OFF;
            p_mb->ReadWrite = MBREAD;
            eStatus = sMB_RegCoilsCB( p_mb,pBuf);
        }
        else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    }
    else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    
    return eStatus;
}
#endif

/*
********************************************************************************************************
*                                                sMB FN02 Handler
*
* Description: 此函数用于从机 FN02 号命令的处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 
********************************************************************************************************
*/
#if MB_FN02_READ_DISCRETE_ENABLED == MBENABLED
static MB_Exception sMB_FN02_Read_DiscreteHandler( wMB *p_mb,UCHAR* pBuf)
{
    UCHAR          *FramePtr; 
    UCHAR           ucNBytes;
    MB_Exception    eStatus = MB_EX_NONE;
    if( p_mb->BufCnt == sMB_PDU_FN02_RcvLen_Min )
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
            FramePtr = &pBuf[MB_PDU_TCP_OFF];
        }else
        {FramePtr = &pBuf[MB_PDU_RTU_OFF];}
        p_mb->RegAddress = sMB_GetRegValue( MB_PDU_REQ_ADDR_OFF );
        p_mb->RegCnt   = sMB_GetRegValue( MB_PDU_REQ_CNT_OFF );   
        if((p_mb->RegCnt >= MB_PDU_FNxx_COUNT_MIN) && (p_mb->RegCnt<MB_PDU_FN02_READCOUNT_MAX))
        {
            if( ( p_mb->RegCnt & 0x0007 ) != 0 )
            {
                ucNBytes = ( UCHAR ) ( p_mb->RegCnt / 8 + 1 );
            }
            else
            {
                ucNBytes = ( UCHAR ) ( p_mb->RegCnt / 8 );
            }
            FramePtr[MB_PDU_FN02_BYTECNT_OFF] = ucNBytes;
            p_mb->BufCnt =  2 + ucNBytes;
            p_mb->Value_off = MB_PDU_FN02_VALUE_OFF ;
            eStatus = sMB_RegDiscreteCB( p_mb,pBuf);            
        }
        else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    }
    else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    return eStatus;
}
#endif

/*
********************************************************************************************************
*                                                sMB FN03 Handler
*
* Description: 此函数用于从机 FN03 号命令的处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 
********************************************************************************************************
*/
#if MB_FN03_READ_HOLDING_ENABLED == MBENABLED
static MB_Exception sMB_FN03_Read_HoldingRegister( wMB *p_mb,UCHAR* pBuf)
{
    UCHAR          *FramePtr; 
    MB_Exception    eStatus = MB_EX_NONE;
    if( p_mb->BufCnt == sMB_PDU_FN03_RcvLen_Min )
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
            FramePtr = &pBuf[MB_PDU_TCP_OFF];
        }else
        {FramePtr = &pBuf[MB_PDU_RTU_OFF];}
        p_mb->RegAddress = sMB_GetRegValue( MB_PDU_REQ_ADDR_OFF );
        p_mb->RegCnt   = sMB_GetRegValue( MB_PDU_REQ_CNT_OFF );   
        if((p_mb->RegCnt>=MB_PDU_FNxx_COUNT_MIN) && (p_mb->RegCnt<= MB_PDU_FN03_READCOUNT_MAX))
        {
            FramePtr[MB_PDU_FN03_BYTECNT_OFF] =  p_mb->RegCnt * 2;
            p_mb->BufCnt = 2 + p_mb->RegCnt * 2;
            p_mb->Value_off = MB_PDU_FN03_VALUE_OFF ;
            p_mb->ReadWrite = MBREAD;
            eStatus = sMB_RegHoldingCB( p_mb,pBuf);
        }
        else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    }
    else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    return eStatus;
}
#endif

/*
********************************************************************************************************
*                                                sMB FN04 Handler
*
* Description: 此函数用于从机 FN04 号命令的处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 
********************************************************************************************************
*/
#if MB_FN04_READ_INPUT_ENABLED == MBENABLED
static MB_Exception sMB_FN04_Read_InputHandler( wMB *p_mb,UCHAR* pBuf)
{
    UCHAR          *FramePtr; 
    MB_Exception    eStatus = MB_EX_NONE;
    if( p_mb->BufCnt == sMB_PDU_FN04_RcvLen_Min )
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
            FramePtr = &pBuf[MB_PDU_TCP_OFF];
        }else
        {FramePtr = &pBuf[MB_PDU_RTU_OFF];}        
        p_mb->RegAddress = sMB_GetRegValue( MB_PDU_REQ_ADDR_OFF );
        p_mb->RegCnt   = sMB_GetRegValue( MB_PDU_REQ_CNT_OFF ); 
        if( ( p_mb->RegCnt >= MB_PDU_FNxx_COUNT_MIN) && (p_mb->RegCnt<MB_PDU_FN04_READCOUNT_MAX))
        {
            FramePtr[MB_PDU_FN04_BYTECNT_OFF] =  p_mb->RegCnt * 2 ;
            p_mb->BufCnt = 2 + p_mb->RegCnt * 2;
            p_mb->Value_off = MB_PDU_FN04_VALUE_OFF ;
            eStatus = sMB_RegInputCB( p_mb,pBuf);
        }
        else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    }
    else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    return eStatus;
}
#endif

/*
********************************************************************************************************
*                                                sMB FN05 Handler
*
* Description: 此函数用于从机 FN05 号命令的处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 
********************************************************************************************************
*/
#if MB_FN05_WRITE_COIL_ENABLED == MBENABLED
static MB_Exception sMB_FN05_Write_CoilsHandler( wMB *p_mb ,UCHAR* pBuf)
{
    UCHAR          *FramePtr; 
    MB_Exception    eStatus = MB_EX_NONE;
    if( p_mb->BufCnt == sMB_PDU_FN05_RcvLen_Min )
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
            FramePtr = &pBuf[MB_PDU_TCP_OFF];
        }else
        {FramePtr = &pBuf[MB_PDU_RTU_OFF];}
        p_mb->RegAddress = sMB_GetRegValue( MB_PDU_REQ_ADDR_OFF );
        p_mb->RegCnt   = 1;
        if((FramePtr[MB_PDU_FN05_VALUE_OFF + 1] == 0x00 ) 
            && ((FramePtr[MB_PDU_FN05_VALUE_OFF] == 0xFF ) 
		    || (FramePtr[MB_PDU_FN05_VALUE_OFF] == 0 )))
        {
            if( FramePtr[MB_PDU_FN05_VALUE_OFF] == 0xFF )
            {
                FramePtr[MB_PDU_FN05_VALUE_OFF] = 1;
            }
            p_mb->Value_off = MB_PDU_FN05_VALUE_OFF ;
            p_mb->ReadWrite = MBWRITE;
            eStatus = sMB_RegCoilsCB( p_mb,pBuf);
            if(FramePtr[MB_PDU_FN05_VALUE_OFF] == 1)
            {
                FramePtr[MB_PDU_FN05_VALUE_OFF] = 0xFF;
            }
        }
        else {eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    }
    else {eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    return eStatus;
}
#endif

/*
********************************************************************************************************
*                                                sMB FN06 Handler
*
* Description: 此函数用于从机 FN06 号命令的处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 
********************************************************************************************************
*/
#if MB_FN06_WRITE_HOLDING_ENABLED == MBENABLED
static MB_Exception sMB_FN06_Write_HoldingHandler( wMB *p_mb ,UCHAR* pBuf)
{
    UCHAR          *FramePtr; 
    MB_Exception    eStatus = MB_EX_NONE;
    if( p_mb->BufCnt == sMB_PDU_FN06_RcvLen_Min )
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
            FramePtr = &pBuf[MB_PDU_TCP_OFF];
        }else
        {FramePtr = &pBuf[MB_PDU_RTU_OFF];}
        p_mb->RegAddress = sMB_GetRegValue( MB_PDU_REQ_ADDR_OFF );
        p_mb->RegCnt   = 1;
        p_mb->Value_off = MB_PDU_FN06_VALUE_OFF ;
        p_mb->ReadWrite = MBWRITE;
        eStatus = sMB_RegHoldingCB( p_mb,pBuf);
    }
    else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    return eStatus;
}
#endif

/*
********************************************************************************************************
*                                                sMB FN15 Handler
*
* Description: 此函数用于从机 FN15 号命令的处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 
********************************************************************************************************
*/
#if MB_FN15_WRITE_COILS_ENABLED == MBENABLED
static MB_Exception sMB_FN15_Write_CoilsHandler( wMB *p_mb ,UCHAR* pBuf)
{
    UCHAR          *FramePtr; 
    UCHAR           ucNBytes;
    MB_Exception    eStatus = MB_EX_NONE;
    if( p_mb->BufCnt >= sMB_PDU_FN15_RcvLen_Min )
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
            FramePtr = &pBuf[MB_PDU_TCP_OFF];
        }else
        {FramePtr = &pBuf[MB_PDU_RTU_OFF];}        
        p_mb->RegAddress = sMB_GetRegValue( MB_PDU_REQ_ADDR_OFF );
        p_mb->RegCnt   = sMB_GetRegValue( MB_PDU_REQ_CNT_OFF );     
        if( ( p_mb->RegCnt & 0x0007 ) != 0 )
        {
            ucNBytes = ( UCHAR )( p_mb->RegCnt / 8 + 1 );
        }
        else
        {
            ucNBytes = ( UCHAR )( p_mb->RegCnt / 8 );
        }
        if((p_mb->RegCnt>=MB_PDU_FNxx_COUNT_MIN) && (p_mb->RegCnt<=MB_PDU_FN15_WRITECOUNT_MAX) 
            && ( ucNBytes == FramePtr[MB_PDU_FN15_BYTECNT_OFF] ))
        {
            p_mb->BufCnt = MB_PDU_FN15_BYTECNT_OFF; 
            p_mb->Value_off = MB_PDU_FN15_VALUE_OFF ;
            p_mb->ReadWrite = MBWRITE;            
            eStatus = sMB_RegCoilsCB( p_mb,pBuf);
        }
        else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    }
    else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    return eStatus;
}
#endif

/*
********************************************************************************************************
*                                                sMB FN16 Handler
*
* Description: 此函数用于从机 FN16 号命令的处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 
********************************************************************************************************
*/
#if MB_FN16_WRITE_HOLDING_ENABLED == MBENABLED
static MB_Exception sMB_FN16_Write_HoldingHandler( wMB *p_mb ,UCHAR* pBuf)
{
    UCHAR          *FramePtr; 
    MB_Exception    eStatus = MB_EX_NONE;
    if( p_mb->BufCnt >= sMB_PDU_FN16_RcvLen_Min )
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
            FramePtr = &pBuf[MB_PDU_TCP_OFF];
        }else
        {FramePtr = &pBuf[MB_PDU_RTU_OFF];}
        p_mb->RegAddress = sMB_GetRegValue( MB_PDU_REQ_ADDR_OFF );
        p_mb->RegCnt   = sMB_GetRegValue( MB_PDU_REQ_CNT_OFF );   
        if((p_mb->RegCnt>=MB_PDU_FNxx_COUNT_MIN) &&(p_mb->RegCnt <=MB_PDU_FN16_WRITECOUNT_MAX) 
            && ( FramePtr[MB_PDU_FN16_BYTECNT_OFF] == ( 2 * p_mb->RegCnt )))
        {
            p_mb->BufCnt = MB_PDU_FN16_BYTECNT_OFF;
            p_mb->Value_off = MB_PDU_FN16_VALUE_OFF ; 
            p_mb->ReadWrite = MBWRITE;            
            eStatus = sMB_RegHoldingCB( p_mb,pBuf);
        }
        else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    }
    else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    return eStatus;
}
#endif

/*
********************************************************************************************************
*                                                sMB FN23 Handler
*
* Description: 此函数用于从机 FN23 号命令的处理函数
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : 
* Note(s)    : 1) 
********************************************************************************************************
*/
#if MB_FN23_READWRITE_HOLDING_ENABLED == MBENABLED
static MB_Exception sMB_FN23_ReadWrite_HoldingHandler( wMB *p_mb,UCHAR* pBuf)
{
    UCHAR          *FramePtr; 
    MB_Exception    eStatus = MB_EX_NONE;
    if( p_mb->BufCnt >= sMB_PDU_FN23_RcvLen_Min )
    {
        if(p_mb->Mode == MB_MODE_TCP)
        {
            FramePtr = &pBuf[MB_PDU_TCP_OFF];
        }else
        {FramePtr = &pBuf[MB_PDU_RTU_OFF];}
        p_mb->RegAddress = sMB_GetRegValue( MB_PDU_REQ_2ndADDR_OFF );
        p_mb->RegCnt   = sMB_GetRegValue( MB_PDU_REQ_2ndCNT_OFF );  
        if((p_mb->RegCnt>=MB_PDU_FNxx_COUNT_MIN) && (p_mb->RegCnt <=MB_PDU_FN23_WRITECOUNT_MAX) 
         &&(( 2 * p_mb->RegCnt ) == FramePtr[MB_PDU_FN23_WRITEBYTECNT_OFF] ))
        {
            p_mb->Value_off = MB_PDU_FN23_WRITEVALUE_OFF ;
            p_mb->ReadWrite = MBWRITE;
            eStatus = sMB_RegHoldingCB( p_mb,pBuf);
            p_mb->RegAddress = sMB_GetRegValue( MB_PDU_REQ_ADDR_OFF );
            p_mb->RegCnt   = sMB_GetRegValue( MB_PDU_REQ_CNT_OFF );  
            if((eStatus == MB_EX_NONE ) &&( p_mb->RegCnt >= 1 ) 
				&& ( p_mb->RegCnt <= MB_PDU_FN23_READCOUNT_MAX ))
            {
                FramePtr[MB_PDU_FN23_READBYTECNT_OFF] =  p_mb->RegCnt * 2 ;
                p_mb->BufCnt = 2 + 2 * p_mb->RegCnt;
                p_mb->Value_off = MB_PDU_FN23_READVALUE_OFF ;
                p_mb->ReadWrite = MBREAD;
                eStatus =sMB_RegHoldingCB( p_mb,pBuf);
            }
        }
        else{eStatus = MB_EX_ILLEGAL_DATA_VALUE;}
    }
    return eStatus;
}
#endif

/*
********************************************************************************************************
*                                                Function Handlers
*
* Description: 此函数用于从机wModbus协议栈的功能函数库调用。
* Arguments  : p_mb           指向1个wModbus的指针
* Note(s)    : 1) sMB_FuncHandlers() 自动被 sMB_Poll()调用
********************************************************************************************************
*/
static MB_Exception sMB_FuncHandlers(wMB   *p_mb,UCHAR* pBuf)
{
    UCHAR          *FramePtr;    
    MB_Exception   Exception; 
    if(p_mb->Mode == MB_MODE_TCP)
    {
         FramePtr = &pBuf[MB_PDU_TCP_OFF];
    }else
    {FramePtr = &pBuf[MB_PDU_RTU_OFF];}     
    
    switch (FramePtr[MB_PDU_FUNC_OFF])
    {   
#if MB_FN01_READ_COILS_ENABLED == MBENABLED
        case MB_FN01_READ_COILS:
            #if (MB_BROADCASTRESPOND_ENABLED != MBENABLED)
            if(p_mb->RecvAddr != MB_ADDRESS_BROADCAST ) 
            #endif   
            {Exception = sMB_FN01_Read_CoilsHandler(p_mb,pBuf);}break;
#endif
#if MB_FN02_READ_DISCRETE_ENABLED == MBENABLED       
        case MB_FN02_READ_DISCRETE_INPUTS:
            #if (MB_BROADCASTRESPOND_ENABLED != MBENABLED)
            if(p_mb->RecvAddr != MB_ADDRESS_BROADCAST )
            #endif            
            {Exception = sMB_FN02_Read_DiscreteHandler(p_mb,pBuf);}break;
#endif   
#if MB_FN03_READ_HOLDING_ENABLED == MBENABLED
        case MB_FN03_READ_HOLDING_REGISTER:
            #if (MB_BROADCASTRESPOND_ENABLED != MBENABLED)
            if(p_mb->RecvAddr != MB_ADDRESS_BROADCAST )
            #endif              
            {Exception = sMB_FN03_Read_HoldingRegister(p_mb,pBuf);}break;
#endif    
#if MB_FN04_READ_INPUT_ENABLED == MBENABLED
        case MB_FN04_READ_INPUT_REGISTER:
            #if (MB_BROADCASTRESPOND_ENABLED != MBENABLED)
            if(p_mb->RecvAddr != MB_ADDRESS_BROADCAST )
            #endif              
            {Exception = sMB_FN04_Read_InputHandler(p_mb,pBuf);}break;
#endif 
#if MB_FN05_WRITE_COIL_ENABLED == MBENABLED
        case MB_FN05_WRITE_SINGLE_COIL:
            {Exception = sMB_FN05_Write_CoilsHandler(p_mb,pBuf);}break;
#endif   
#if MB_FN06_WRITE_HOLDING_ENABLED == MBENABLED
        case MB_FN06_WRITE_REGISTER:
            {Exception = sMB_FN06_Write_HoldingHandler(p_mb,pBuf);}break;
#endif  
#if MB_FN15_WRITE_COILS_ENABLED == MBENABLED
        case MB_FN15_WRITE_MULTIPLE_COILS:
            {Exception = sMB_FN15_Write_CoilsHandler(p_mb,pBuf);}break;
#endif  
#if MB_FN16_WRITE_HOLDING_ENABLED == MBENABLED
        case MB_FN16_WRITE_MULTIPLE_REGISTERS:
            {Exception = sMB_FN16_Write_HoldingHandler(p_mb,pBuf);}break;
#endif
#if MB_FN23_READWRITE_HOLDING_ENABLED == MBENABLED
        case MB_FN23_READWRITE_MULTIPLE_REGISTERS:
            {Exception = sMB_FN23_ReadWrite_HoldingHandler(p_mb,pBuf);}break;
#endif
        default:
            {Exception = MB_EX_ILLEGAL_FUNCTION;}break;
    } 
    return Exception;
}

/*
********************************************************************************************************
*                                                Deal A wModbus
*
* Description: 此函数用于从机wModbus协议栈Poll函数，需循环调用。
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : MB_Exception   返回错误代码。
* Note(s)    : 1) sMB_Deal() 应当创建一个wModbus协议栈并使能后使用
********************************************************************************************************
*/
USHORT sMB_Deal( wMB *p_mb,UCHAR* pBuf,USHORT length )
{
    UCHAR          *FramePtr;      
    MB_Exception   eStatus;  
    USHORT         slen = 0;
    if ( MB_MutexPend( p_mb ) != MBFALSE )
    {
        if(MB_Receive( p_mb,pBuf,length) != MBTRUE)
        {
            #if (MB_STAT_ENABLED == MBENABLED)
            p_mb->StatReceiveErrCtr++;
            #endif 
        }
        else 
        {   
            if(( p_mb->RecvAddr == p_mb->NodeAddr ) || ( p_mb->RecvAddr == MB_ADDRESS_BROADCAST ))
            {
                eStatus = sMB_FuncHandlers(p_mb,pBuf);
                if( eStatus != MB_EX_NONE )
                {
                    p_mb->BufCnt  = 0;
                    if(p_mb->Mode == MB_MODE_TCP)
                    {
                         FramePtr= &pBuf[MB_PDU_TCP_OFF];
                    }else
                    {
                        FramePtr = &pBuf[MB_PDU_RTU_OFF];
                    } 
                    FramePtr[p_mb->BufCnt ++] = FramePtr[MB_PDU_FUNC_OFF] | MB_FUNC_ERROR ;
                    FramePtr[p_mb->BufCnt ++] = eStatus;
                    #if (MB_STAT_ENABLED == MBENABLED)
                    p_mb->StatHandlersErrCtr++;
                    #endif  
                }
                #if (MB_STAT_ENABLED == MBENABLED)                    
                else
                {
                    p_mb->StatOKCtr++;
                }
                #endif                      
                slen = MB_Send(p_mb,pBuf);
            }
        } 
        MB_MutexPost(p_mb);
    }
    return slen;
}
#endif
