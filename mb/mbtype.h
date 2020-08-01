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

/* Filename    : mbtype.h*/

#include "mbconfig.h" 

#ifndef _MB_TYPE_H
#define _MB_TYPE_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

#define MBNULL        				            ( 0 )
#define MBFALSE        				            ( 0 )
#define MBTRUE       					        ( 1 )
#define MBSLAVE        					        ( 1 )
#define MBMASTER       					        ( 0 )
#define MBREAD                                  ( 0 )
#define MBWRITE                                 ( 1 )

#define MB_FUNC_NONE                          	( 0x00 )
#define MB_FN01_READ_COILS                    	( 0x01 )
#define MB_FN02_READ_DISCRETE_INPUTS          	( 0x02 )
#define MB_FN03_READ_HOLDING_REGISTER         	( 0x03 )
#define MB_FN04_READ_INPUT_REGISTER           	( 0x04 )
#define MB_FN05_WRITE_SINGLE_COIL             	( 0x05 )
#define MB_FN06_WRITE_REGISTER                	( 0x06 )
#define MB_FN11_DIAG_GET_COM_EVENT_CNT        	( 0x0B )
#define MB_FN12_DIAG_GET_COM_EVENT_LOG        	( 0x0C )
#define MB_FN15_WRITE_MULTIPLE_COILS          	( 0x0F )
#define MB_FN16_WRITE_MULTIPLE_REGISTERS      	( 0x10 )
#define MB_FN17_OTHER_REPORT_SLAVEID          	( 0x11 )
#define MB_FN23_READWRITE_MULTIPLE_REGISTERS  	( 0x17 )
#define MB_FUNC_ERROR                         	( 128 )

#define MB_PDU_FN01_READCOUNT_MAX               ( 0x07D0 )
#define MB_PDU_FN02_READCOUNT_MAX            	( 0x07D0 )
#define MB_PDU_FN03_READCOUNT_MAX            	( 0x007D )
#define MB_PDU_FN04_READCOUNT_MAX            	( 0x007D )
#define MB_PDU_FN15_WRITECOUNT_MAX            	( 0x07B0 )
#define MB_PDU_FN16_WRITECOUNT_MAX            	( 0x007B )
#define MB_PDU_FN23_READCOUNT_MAX             	( 0x007D )
#define MB_PDU_FN23_WRITECOUNT_MAX            	( 0x0079 )
#define MB_PDU_FNxx_COUNT_MIN            		( 0x0001 )

#define MB_BITS                                 ( 16 )
#define MB_PDU_SIZE_MIN    					    ( 4  )
#define MB_PDU_SIZE_MAX     				    ( 256)   

#define MB_PDU_RTU_OFF      				    ( 1  )
#define MB_PDU_TCP_OFF      				    ( 7  ) 
#define MB_PDU_FUNC_OFF         				( 0  )    

#define MB_PDU_REQ_ADDR_OFF                		( 1  )
#define MB_PDU_REQ_CNT_OFF             			( 3  )
#define MB_PDU_REQ_2ndADDR_OFF     				( 5  )
#define MB_PDU_REQ_2ndCNT_OFF             	    ( 7  )
#define MB_PDU_FN01_BYTECNT_OFF                 ( 1  )
#define MB_PDU_FN01_VALUE_OFF                   ( 2  )
#define MB_PDU_FN02_BYTECNT_OFF                 ( 1  )
#define MB_PDU_FN02_VALUE_OFF                   ( 2  )
#define MB_PDU_FN03_BYTECNT_OFF                 ( 1  )
#define MB_PDU_FN03_VALUE_OFF                   ( 2  )
#define MB_PDU_FN04_BYTECNT_OFF                 ( 1  )
#define MB_PDU_FN04_VALUE_OFF                   ( 2  )
#define MB_PDU_FN05_VALUE_OFF                   ( 3  )
#define MB_PDU_FN06_VALUE_OFF                   ( 3  )
#define MB_PDU_FN15_BYTECNT_OFF                 ( 5  )
#define MB_PDU_FN15_VALUE_OFF                   ( 6  )
#define MB_PDU_FN16_BYTECNT_OFF                 ( 5  )
#define MB_PDU_FN16_VALUE_OFF                   ( 6  )
#define MB_PDU_FN23_WRITEBYTECNT_OFF            ( 9  )
#define MB_PDU_FN23_WRITEVALUE_OFF              ( 10 )
#define MB_PDU_FN23_READBYTECNT_OFF             ( 1  )
#define MB_PDU_FN23_READVALUE_OFF               ( 2  )

typedef     unsigned  	char            UCHAR;
typedef     signed   	char            CHAR;
typedef     unsigned    short  int      USHORT;
typedef     signed      short  int      SHORT;
typedef     unsigned  	int             ULONG;
typedef     signed  	int             LONG;

typedef enum
{
    MB_MODE_RTU                         =1,				
    MB_MODE_ASCII                       =2,									      
    MB_MODE_TCP                         =3,				     
} MB_ModeType;

typedef enum
{
    MB_STAT_SEND                        =1,
    MB_STAT_RECEIVEERR                  =2,
    MB_STAT_HANDLERSERR                 =3,
    MB_STAT_OK                          =4,    
} MB_StatType;

typedef enum
{
    MB_EX_NONE                          = 0x00,                 /**/
    MB_EX_ILLEGAL_FUNCTION              = 0x01,                 /*不合法的功能代码*/
    MB_EX_ILLEGAL_DATA_ADDRESS          = 0x02,                 /*不合法数据地址*/	
    MB_EX_ILLEGAL_DATA_VALUE            = 0x03,                 /*不合法数据*/	
    MB_EX_SLAVE_DEVICE_FAILURE          = 0x04,                 /*从机设备故障*/	
    MB_EX_ACKNOWLEDGE                   = 0x05,                 /*确认*/	
    MB_EX_SLAVE_BUSY                    = 0x06,                 /*从机设备忙*/
    MB_EX_NEGATIVE                      = 0x07,                 /*否定*/
    MB_EX_MEMORY_PARITY_ERROR           = 0x08,                 /*内存奇偶校验错误*/		
    MB_EX_GATEWAY_PATH_FAILED           = 0x0A,                 /**/		
    MB_EX_GATEWAY_TGT_FAILED            = 0x0B,                 /**/
	
	MB_EX_NO_MEMORY               		= 0x11,					//无可用内存
    MB_EX_REVDATAERR                 	= 0x12,					//数据校验出错
} MB_Exception;

typedef  UCHAR(*MBPortTransmit )(void *Parent, UCHAR *pData, USHORT Size,ULONG lTimeOut);//发送回调函数
typedef  SHORT(*MBPortReceive  )(void *Parent, UCHAR *pData, USHORT Size,ULONG lTimeOut);//接收回调函数

typedef  struct    wmb       wMB;
struct  wmb{
    UCHAR              NodeAddr;								/*node 地址*/
    UCHAR              MasterSlave;								/*主从类型*/
    MB_ModeType        Mode;									/*传输模式*/
    UCHAR              ReadWrite;								/*读写类型*/ 
    UCHAR              Value_off;               
    UCHAR              RecvAddr;                
    UCHAR              BufCnt;        
	USHORT             RegCnt;               
    USHORT             RegAddress;
    USHORT             ComCnt;    
    void               *Parent; 								/*参数*/
    void               *Mutex;    
    MBPortReceive      RxCpltPtr;								/*接收回调函数*/   
    MBPortTransmit     TxCpltPtr;								/*发送回调函数*/  

#if (MB_MASTER_ENABLED  == MBENABLED) 
    USHORT             BitOffset;
    USHORT             *RegDataPtr;          
#endif

#if (MB_STAT_ENABLED == MBENABLED)
	ULONG              StatSendCtr;								/*发送量统计*/
    ULONG              StatReceiveErrCtr;						/*接收错误量统计*/
    ULONG              StatHandlersErrCtr;						/*处理错误量统计*/
    ULONG              StatOKCtr;								/*正确量统计*/             
#endif 
};
 
#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
