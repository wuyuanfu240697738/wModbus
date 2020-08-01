/*
********************************************************************************************************
*                                                wModbus
*                          The Embedded Modbus Stack Software
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

/* Filename    : mb.c*/
#include "mb.h"
    
#if ( MB_MASTER_ENABLED == MBENABLED || MB_SLAVE_ENABLED== MBENABLED )

/*
********************************************************************************************************
*                                                LRC
*
* Description: 此函数用于ASCII模式下的LRC校验
* Arguments  : pDataBuf       指向需要校验DataBuf的指针
*              iLen           DataBuf的长度
* Returns    : UCHAR          LRC校验的结果
* Note(s)    : 1) MB_LRC() 
********************************************************************************************************
*/
#if ( MB_ASCII_ENABLED == MBENABLED ) 
static UCHAR MB_LRC( UCHAR* pDataBuf, USHORT iLen)
{
    UCHAR    mLRC = 0x00; 
    while( iLen-- ){mLRC += *pDataBuf++;}
    return ( - mLRC );
}
#endif 

/*
********************************************************************************************************
*                                                CRC
*
* Description: 此函数用于RTU模式下的CRC校验
* Arguments  : pDataBuf       指向需要校验DataBuf的指针
*              iLen           DataBuf的长度
* Returns    : USHORT         CRC校验的结果
* Note(s)    : 1) MB_CRC() 
********************************************************************************************************
*/
#if ( MB_RTU_ENABLED == MBENABLED )

const USHORT MBCRC_table[256] = {
0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

static USHORT MB_CRC( UCHAR * pDataBuf, USHORT iLen )
{  
    USHORT mCRC = 0xffff;
    while (iLen-- > 0)
        mCRC = MBCRC_table[(mCRC ^ *pDataBuf++) & 0x00ff] ^ (mCRC >> 8) ;
    return mCRC; 
}
#endif


/*
********************************************************************************************************
*                                                Init A wModbus
*
* Description: 此函数用于创建一个wModbus协议栈
* Arguments  : p_mb           指向1个wModbus的指针
*              Mode           传输模式
*              Address        地址
*              Parent         传输硬件句柄
* Returns    : MBTRUE         创建成功
*              MBFALSE        创建失败
* Note(s)    : 1) MB_Init() 
********************************************************************************************************
*/
UCHAR MB_Init( wMB *p_mb,MB_ModeType Mode,UCHAR Address,void *Parent,void* mutex)
{
    if(p_mb != MBNULL)
    {
        if( Address == MB_ADDRESS_BROADCAST ){p_mb->MasterSlave = MBMASTER;}
        else if(( Address >= MB_ADDRESS_MIN ) && ( Address <= MB_ADDRESS_MAX ))
        {
            p_mb->MasterSlave = MBSLAVE;   
            p_mb->NodeAddr = Address;
        }
        else {return MBFALSE;}
        
        p_mb->Mode = Mode;
        p_mb->Parent = Parent;
        p_mb->ComCnt = 0;
        
        #if (MB_STAT_ENABLED == MBENABLED)
        p_mb->StatSendCtr = 0;
        p_mb->StatReceiveErrCtr = 0;
        p_mb->StatHandlersErrCtr = 0;
        p_mb->StatOKCtr = 0;             
        #endif
        
        return MB_MutexInit(p_mb,mutex);
    }

    return MBFALSE;    
} 

/*
********************************************************************************************************
*                                                Close A wModbus
*
* Description: 此函数用于关闭一个wModbus协议栈
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : MBTRUE         创建成功
*              MBFALSE        创建失败
* Note(s)    : 1) MB_Close() 
********************************************************************************************************
*/
UCHAR MB_Close( wMB *p_mb)
{
    p_mb->Parent = 0;
    p_mb->ComCnt = 0;
    
#if (MB_STAT_ENABLED == MBENABLED)
    p_mb->StatSendCtr = 0;
    p_mb->StatReceiveErrCtr = 0;
    p_mb->StatHandlersErrCtr = 0;
    p_mb->StatOKCtr = 0;             
#endif  

    p_mb->TxCpltPtr = 0;
    p_mb->RxCpltPtr = 0;
    
    return MB_MutexClose(p_mb);    
} 
/*
********************************************************************************************************
*                                                MB Port Set
*
* Description: 此函数用于初使化硬件端口参数
* Arguments  : p_mb           指向1个wModbus的指针
*              TxCpltPtr      数据发送回调函数
*              RxCpltPtr      数据接收回调函数
* Returns    : No
********************************************************************************************************
*/
void MB_PortSet(wMB *p_mb,MBPortTransmit TxCpltPtr,MBPortReceive RxCpltPtr)
{
    if(p_mb != (wMB *)MBNULL)
    {
        p_mb->TxCpltPtr = TxCpltPtr;
        p_mb->RxCpltPtr = RxCpltPtr;
    }
}


#if ( MB_ASCII_ENABLED == MBENABLED ) 
const UCHAR  MBChar_table[256] ={
'0' ,'1' ,'2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,'A' ,'B' ,'C' ,'D' ,'E' ,'F' ,
'G' ,'H' ,'I' ,'J' ,'K' ,'L' ,'M' ,'N' ,'O' ,'P' ,'Q' ,'R' ,'S' ,'T' ,'U' ,'V' ,
'W' ,'X' ,'Y' ,'Z' ,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
0x40,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x5b,0x5c,0x5d,0x5e,0x5f,
0x60,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x7b,0x7c,0x7d,0x7e,0x7f,
'0' ,'1' ,'2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,'A' ,'B' ,'C' ,'D' ,'E' ,'F' ,
'G' ,'H' ,'I' ,'J' ,'K' ,'L' ,'M' ,'N' ,'O' ,'P' ,'Q' ,'R' ,'S' ,'T' ,'U' ,'V' ,
'W' ,'X' ,'Y' ,'Z' ,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
0x40,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x5b,0x5c,0x5d,0x5e,0x5f,
0x60,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x7b,0x7c,0x7d,0x7e,0x7f
};
#endif 

/*
********************************************************************************************************
*                                                wModbus Receive manage
*
* Description: 此函数用于把一个wModbus协议栈接收到的数据进行接收处理。
* Arguments  : p_mb           指向1个wModbus的指针
* Returns    : UCHAR          返回处理结果
*                   MBTRUE              正确处理
*                   MBFALSE             数据出错    
* Note(s)    : 1) MB_Receive() 
********************************************************************************************************
*/
UCHAR MB_Receive( wMB   *p_mb,UCHAR* pRevBuf,USHORT RecvCnt )
{
    UCHAR   Status= MBFALSE;
    volatile SHORT mi;
    
    switch(p_mb->Mode)
    {
        #if ( MB_RTU_ENABLED == MBENABLED )        
        case MB_MODE_RTU:
            if( MB_CRC( pRevBuf, RecvCnt ) == 0 )
            {
                p_mb->RecvAddr = pRevBuf[0];
                p_mb->BufCnt = RecvCnt - 3 ;
                Status = MBTRUE;
            }       
            break;
        #endif
        #if ( MB_ASCII_ENABLED == MBENABLED )         
        case MB_MODE_ASCII:
            if(pRevBuf[0] == 0x3A )
            {
                p_mb->BufCnt = (RecvCnt - 3) >> 1 ;
                for(mi=0;mi<p_mb->BufCnt;mi++)
                {
                    pRevBuf[ mi ] = ( MBChar_table[(pRevBuf[2 * mi + 1])] << 4) 
                                          | ( MBChar_table[(pRevBuf[2 * mi + 2])]);
                }
                if(MB_LRC(pRevBuf,p_mb->BufCnt) == 0)
                {
                    p_mb->RecvAddr = pRevBuf[0]; 
                    p_mb->BufCnt = p_mb->BufCnt - 2 ;
                    Status = MBTRUE;                
                }         
            }
            break; 
        #endif
        #if ( MB_TCP_ENABLED == MBENABLED )        
        case MB_MODE_TCP:
            p_mb->RecvAddr = pRevBuf[6];
            p_mb->BufCnt = RecvCnt - MB_PDU_TCP_OFF ;
            Status = MBTRUE;     
            break;
        #endif
        default:break;        
    }    
    return Status;
}

/*
********************************************************************************************************
*                                                wModbus Send manage
*
* Description: 此函数用于把一个wModbus协议栈的数据进行发送处理。
* Arguments  : p_mb           指向1个wModbus的指针
* Note(s)    : 1) MB_Send() 
********************************************************************************************************
*/
USHORT MB_Send( wMB   *p_mb ,UCHAR* pSndBuf)
{
    volatile USHORT usCRC16;
    volatile SHORT  mi;
    volatile USHORT SendCnt;
	
    if((p_mb->MasterSlave == MBMASTER) || 
      (MB_BROADCASTRESPOND_ENABLED == MBENABLED)||
      (p_mb->RecvAddr != MB_ADDRESS_BROADCAST))
    {
		SendCnt = p_mb->BufCnt;
		
        switch(p_mb->Mode)
        {
            #if ( MB_RTU_ENABLED == MBENABLED )        
            case MB_MODE_RTU:
                pSndBuf[0] =  p_mb->NodeAddr;
                SendCnt ++;
                usCRC16 = MB_CRC( pSndBuf, SendCnt  );
                pSndBuf[SendCnt ++] = ( UCHAR )( usCRC16 & 0xFF );
                pSndBuf[SendCnt ++] = ( UCHAR )( usCRC16 >> 8 );
                break; 
            #endif  
            #if ( MB_ASCII_ENABLED == MBENABLED )           
            case MB_MODE_ASCII:
                pSndBuf[0] =  p_mb->NodeAddr;
                SendCnt ++;
                pSndBuf[SendCnt] = MB_LRC( pSndBuf, SendCnt );
                for(mi=SendCnt;mi>=0;mi--)
                {
                    pSndBuf[2 * mi + 2] = MBChar_table[( pSndBuf[ mi ] & 0x0F )];
                    pSndBuf[2 * mi + 1] = MBChar_table[( pSndBuf[ mi ] >>   4 )];
                }
                pSndBuf[0] = 0x3A;
                pSndBuf[SendCnt * 2 + 3] = 0x0D;        
                pSndBuf[SendCnt * 2 + 4] = 0x0A;
                SendCnt = SendCnt * 2 + 5;
                break;
            #endif            
            #if ( MB_TCP_ENABLED == MBENABLED )        
            case MB_MODE_TCP:
                pSndBuf[6] =  p_mb->NodeAddr;
                SendCnt ++; 
                pSndBuf[4] = 0;
                pSndBuf[5] = ( UCHAR )( SendCnt );        
                SendCnt += 6;
                break;    
            #endif             
            default:break;
        }
        
        #if (MB_STAT_ENABLED == MBENABLED)
            p_mb->StatSendCtr++;
        #endif
		p_mb->BufCnt = SendCnt;
		return SendCnt;
    }
    return 0;
}

/*
********************************************************************************************************
*                                                MB Poll
*
* Description: 此函数用于端口接收数据
* Arguments  : p_mb           指向1个wModbus的指针
* Note(s)    : 1) MB_Poll()  
********************************************************************************************************
*/
MB_Exception MB_Poll(wMB *p_mb,UCHAR* pBuf,LONG lTimeOut)
{ 	
	#if (MB_MASTER_ENABLED  == MBENABLED)
	if(p_mb->MasterSlave == MBMASTER)
	{
        if((p_mb->TxCpltPtr != MBNULL) && (p_mb->BufCnt > 0))
        {
            p_mb->TxCpltPtr(p_mb->Parent,pBuf,p_mb->BufCnt,0);               
        }
	}	
	#endif	
	
    if(p_mb->RxCpltPtr != MBNULL)
    {
        SHORT length = p_mb->RxCpltPtr(p_mb->Parent,pBuf,MB_PDU_SIZE_MAX,lTimeOut);
        if(length >= MB_PDU_SIZE_MIN)
        {          
            if(p_mb->MasterSlave == MBMASTER)
            {
				#if (MB_MASTER_ENABLED  == MBENABLED)
					return mMB_Deal( p_mb,pBuf,length);
				#endif				
            }
			else
			{
				#if (MB_SLAVE_ENABLED  == MBENABLED)
					USHORT Cnt = sMB_Deal( p_mb,pBuf,length);
					if((p_mb->TxCpltPtr != MBNULL) && (Cnt > 0))
					{
						p_mb->TxCpltPtr(p_mb->Parent,pBuf,Cnt,0);
					}
				#endif	
			}
        }
    }
    return MB_EX_NONE;
}

/*
********************************************************************************************************
*                                                 STAT Get
*
* Description: 此函数用于向获取统计信息
* Arguments  : p_mb           指向1个wModbus的指针
*              StatType       需要获取的数据类型
* Note(s)    : 1) MB_STATGet()
********************************************************************************************************
*/
#if (MB_STAT_ENABLED == MBENABLED)
ULONG MB_STATGet( wMB   *p_mb ,MB_StatType StatType)
{
    ULONG  Res = 0;
    
    switch(StatType)
    {
        case MB_STAT_SEND:
            Res = p_mb->StatSendCtr;break;
        case MB_STAT_RECEIVEERR:
            Res = p_mb->StatReceiveErrCtr;break;        
        case MB_STAT_HANDLERSERR:
            Res = p_mb->StatHandlersErrCtr;break;        
        case MB_STAT_OK:
            Res = p_mb->StatOKCtr;break;        
    }
    return Res;
}          
#endif 


/*
********************************************************************************************************
*                                                Set Bits To WordBuf
*
* Description: 此函数用于向WordBuf的数据表内设置NBits个Bits
* Arguments  : pWordBuf       指向WordBuf的数据指针
*              BitOffset      偏移地址
*              NBits          需要设定的bit个数
*              SetValue       需要设定的值
* Note(s)    : 1) MB_UtilSetBits() 用于Discrete 或是 coils的数据转化
*              2) NBits<=16
********************************************************************************************************
*/
void MB_UtilSetBits( USHORT *pWordBuf, USHORT BitOffset, UCHAR NBits,USHORT SetValue )
{
    ULONG          *pDoubleWordBuf;
    ULONG          Mask,WordOffset,NPreBits;
    ULONG          Value = SetValue;

    WordOffset = ( ULONG )( ( BitOffset ) / MB_BITS );
    NPreBits = ( ULONG )( BitOffset - WordOffset * MB_BITS );
    Mask = ( ULONG )( ( ( ULONG ) 1 << NBits ) - 1 ) << NPreBits;
    
    pDoubleWordBuf = (ULONG *)&pWordBuf[WordOffset];
    *pDoubleWordBuf = (ULONG)(( *pDoubleWordBuf & ( ~ Mask )) | ((Value << NPreBits) & Mask ));
}

/*
********************************************************************************************************
*                                                Get Bits From WordBuf
*
* Description: 此函数用于从WordBuf的数据表内获取NBits个Bits
* Arguments  : pWordBuf       指向WordBuf的数据指针
*              BitOffset      偏移地址
*              NBits          需要设定的bit个数
* Returns    : USHORT         获取到的数据值
* Note(s)    : 1) MB_UtilGetBits() 用于Discrete 或是 coils的数据转化
*              2) NBits<=16
********************************************************************************************************
*/
USHORT MB_UtilGetBits( USHORT *pWordBuf, USHORT BitOffset, UCHAR NBits )
{
    ULONG          *pDoubleWordBuf;
    ULONG          Mask,WordOffset,NPreBits;

    WordOffset = ( ULONG )( ( BitOffset ) / MB_BITS );
    NPreBits = ( ULONG )( BitOffset - WordOffset * MB_BITS );
    Mask = ( ULONG )( ( ( ULONG ) 1 << NBits ) - 1 );

    pDoubleWordBuf = (ULONG *)&pWordBuf[WordOffset];    
    return ((USHORT )((*pDoubleWordBuf >> NPreBits ) & Mask));
}

#endif
