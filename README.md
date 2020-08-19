# wModbus
Copyright   : Wu YuanFu. All rights reserved.Protected by international copyright laws.
Programmer  : Wu YuanFu
Version     : V2.50
LICENSING TERMS:
wModbus is provided in source form for FREE short-term evaluation, for educational use or for peaceful research.  If you plan or intend to use wModbus in a commercial application/ product then, you need to contact Wu YuanFu <240697738@qq.com> to properly license wModbus for its use in your application/product. The fact that the source is provided does NOT mean that you can use it commercially without paying a licensing fee.Knowledge of the source code may NOT be used to develop a similar product.

本说明书所包含的内容为发明人吴远福提供，其内容会根据硬件的改进和软件的升级进行更新，所有更新的内容都将纳入到新版本的手册中。
版权所有，保留所有权利。

	本说明书所涉及到wModbus协议库源码以及源程序仅供个人免费学习研究使用。如果未经版权所有者授权，禁止用于任何商业行为，还希望你的配合，谢谢！授权联系方式： 吴远福 240697738@qq.com。

1.	背景
Modbus是一种串行通信协议，是Modicon公司（现在的施耐德电气 Schneider Electric）于1979年为使用可编程逻辑控制器（PLC）通信而发表。Modbus已经成为工业领域通信协议的业界标准（De facto），并且现在是工业电子设备之间常用的连接方式。
FreeMODBUS 是一个奥地利人写的Modbus协议栈。它是一个针对嵌入式应用的一个免费的通用MODBUS协议的移植。目前免费版本仅有Slave功能，都支持RTU/ASCII/TCP模式。
FreeMODBUS网址：https://www.embedded-solutions.at/en/freemodbus/
uC/MODBUS 是由Micrium公司写的Modbus协议栈。它是一个针对嵌入式应用的一个收费的通用MODBUS协议的移植。目前有Slave和Master功能，都支持RTU/ASCII模式。
Micrium公司网址：https://www.micrium.com/
这两个协议栈最大的特点是每发送一个字节或是接收一个字节就需引入相关的中断处理程序，比如说主机发送01 03 00 02 00 01 25 CA，从机回传01 03 02 12 34 B5 33。不管是对于主机还是从机功能，这两个协议栈都至少需要中断15次。经过测试，在单一的串口处理系统中不会有什么问题，但是需多串口进行modbus通信或是其它类型的串口通信功能，就容易造成串口间的中断冲突，使得通信容易出错。经过测试，在单台嵌入式设备里同时实现主机及从机功能时，主机丢包率在30%以上。

2.	特点
1.	减少中断次数，同时减少单个中断程序时间，通信测试达到无错误。
2.	以一帧完整的报文为中断响应基础。当发送一帧完整的报文完成后响应一次中断，或是接收到到一帧完整的报文后也响应一次中断。
3.	在单嵌入式设备硬件允许的情况下，可同时实现任意多个主机与从机功能。
4.	进一步减少RAM内存占用，把接收数据串地址与发送数据串进行合并。
5.	提高代码利用率，增加代码可读性，尽可能的保持主机与从机风格统一性。
6.	更友好支持tcp功能及其它传输类型扩展。

3.	功能
1.	wMODBUS协议栈 是针对通用的Modbus协议栈在嵌入式系统中应用的一个实现。
2.	wMODBUS协议栈 在硬件许可的情况下可以挂载任意多个主机与从机。支持多个主机与多个从机同时独立运行。
3.	wMODBUS协议栈 支持RTU/ASCII/RTU模式，支持串串口/网络模式。
4.	wMODBUS协议栈 支持如下功能码。
读输入寄存器 (0x04)
读保持寄存器 (0x03)
写单个寄存器 (0x06)
写多个寄存器 (0x10)
读/写多个寄存器 (0x17)
读取线圈状态 (0x01)
写单个线圈 (0x05)
写多个线圈 (0x0F)
读输入状态 (0x02)
5.	wMODBUS协议栈 支持实时操作系统及裸机移植。
6.	wMODBUS协议栈 提供hook回调函数功能。
7.	wMODBUS协议栈 提供通信统计功能。
8.	wMODBUS协议栈 主机功能可支持与任意非连续地址的从机里进行通信。
9.	wMODBUS协议栈 主机功能为应用提供多种请求模式，用户可以选择阻塞还是非阻塞模式，自定义超时时间等，方便应用层灵活调用。
4.	文件结构
1.	mbconfig.h是wModbus的配置文件，可以对wModbus的功能进行裁减。
2.	mbevent.c 这个文件是与嵌入式操作系统相关的代码，针对不同的操作系统，代码需要进行相应的改写，主要是需要实现事件等待函数，事件发送函数等。从而使得中断函数可以对侦听主程序的数据传送功能。
3.	mb.c mb.h mbtype.h这两个文件是主机和从机共用的代码，这些函数已经写好，与嵌入式操作系统以及CPU都无关，用户无需再进行任何修改。主要是实现的了协议栈的创建，使能，接收拆包，发送打包等功能。
4.	smb.c smbbuf.c smbbuf.h 这五个文件是从机需使用的代码，这些函数已经写好，与嵌入式操作系统以及CPU都无关，用户无需再进行任何修改。主要是实现了从机的侦听与数据处理功能。
5.	mmb.c 这个文件是主机需使用的代码，这些函数已经写好，与嵌入式操作系统以及CPU都无关，用户无需再进行任何修改。主要是实现了主机的侦听与数据处理功能。
5.	使用方法
发送回调函数
UCHAR  PortTransmit (void *Parent, UCHAR *pData, USHORT Size,ULONG lTimeOut)
{
	依照lTimeOut时间进行阻塞发送
	……
return MBTRUE;成功发送返回MBTRUE，否则返回MBFALSE
}

接收回调函数
SHORT  PortReceive (void *Parent, UCHAR *pData, USHORT Size,ULONG lTimeOut)
{
	依照lTimeOut时间进行阻塞接收
……
	Return 接收数据的实际长度,如果未接收到数据返回0，如果接收错误，返回-1。
}


从机任务函数 - ucos为例
uint8_t  buf[256];//定义操作数据内存
static OS_MUTEX   mutex;

void  Task(void *p_arg)
{
	wMB  mb;//定义wModbus协议栈
	int Id;//端口号

	//初使化传输路径 - 串口/网口 （依据Id）

	//初使化wModbus协议栈
	MB_Init(&mb,MB_MODE_RTU,1,0,&mutex); //站号为1的从机

	//设置回调函数
	MB_PortSet(&mb,PortTransmit,PortReceive); 

	while(1)
	{//轮询接收并处理数据
	     if(MB_Poll(&mb, buf,0) <=0)
	     { 
	      	printf("MB Port failed!\n");
	  	 }
	}
}

从机任务函数 - linux为例
int tcp_echo(int client_fd)
{
    wMB mtcp;
	pthread_mutex_t mmutex;
    char				buff[BUFF_SIZE]	= {0};
    ssize_t				len				= 0;

	MB_Init( &mtcp,MB_MODE_TCP,1,0,&mmutex);
	while(1)
	{
		len	= read(client_fd, buff, sizeof(buff));
		if (len < 1) {
			goto err;

		}
		USHORT Res = sMB_Deal( &mtcp,(UCHAR*)buff,len);
                
		if(Res > 0)
		{
			(void)write(client_fd, buff, (size_t)Res);
		}
		
	}

    return EXIT_SUCCESS;
 err:
    return EXIT_FAILURE;
}

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

主机任务函数
uint8_t  buf[256];//定义操作数据内存
static OS_MUTEX   mutex;
void  Task(void *p_arg)
{
	wMB  mb;//定义wModbus协议栈
	int Id;//端口号

	//初使化传输路径 - 串口/网口 （依据Id）

	//初使化wModbus协议栈
	MB_Init( & mb,MB_MODE_RTU,0,&mutex); //站号为0则为主机

	//设置回调函数
	MB_PortSet(&mb,PortTransmit,PortReceive); 

	//直接操作功能函数 – 阻塞等待数据返回
	mMB_FN01_Read_CoilsRegister()
	mMB_FN02_Read_DiscreteRegister()
	……
}




