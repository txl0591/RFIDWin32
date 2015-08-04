// RFIDData.h : RFIDData DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "FrameTypeDef.h"
#include "QueueSend.h"
#include "FrameData.h"
#include "FrameRecv.h"
#include "FrameSend.h"
#include "FrameProcessor.h"

typedef void(__stdcall *OnReportListener)(int Echo, int DataLen);

// 初始化 Listenter 为typedef void (*OnEchoReportListener)(int Echo, unsigned char* Data, int DataLen); 所有数据均由该回调回传
// Data 数据 DataLen数据长度
// Echo 应答值 

//读用户区成功失败
// RFID_READ_USER_SUCCESS		
// RFID_READ_USER_ERROR		

//读系统区
// RFID_READ_SYS_SUCCESS
// RFID_READ_SYS_ERROR

//写用户区
// RFID_WRITE_USER_SUCCESS		
// RFID_WRITE_USER_ERROR		

//写系统区
// RFID_WRITE_SYS_SUCCESS		
// RFID_WRITE_SYS_ERROR		

//获取卡号
// RFID_GET_CARDID_SUCCESS		
// RFID_GET_CARDID_ERROR		

//获取卡号以及卡状态
// RFID_GET_CARDTYPE_SUCCESS	
// RFID_GET_CARDTYPE_ERROR		

//写卡当前状态
// RFID_WRITE_CRCBLK_SUCCESS	
// RFID_WRITE_CRCBLK_ERROR		

/*RFIDState 操作返回状态
#define RFID_READ_ERROR				0x00
#define RFID_READ_USER_SUCCESS		0x01
#define RFID_READ_SYS_SUCCESS		0x02
#define RFID_READ_USER_ERROR		0x03
#define RFID_READ_SYS_ERROR			0x04
#define RFID_READ_VERSION			0x05
#define RFID_CLEAR_PWD_SUCCESS		0x06
#define RFID_CLEAR_PWD_ERROR		0x07        
#define RFID_WRITE_USER_SUCCESS		0x08
#define RFID_WRITE_SYS_SUCCESS		0x09
#define RFID_WRITE_USER_ERROR		0x0A
#define RFID_WRITE_SYS_ERROR		0x0B
#define RFID_WRITE_SUCCESS			0x0C
#define RFID_WRITE_ERROR			0x0D
#define RFID_GET_CARDID_SUCCESS		0x0E
#define RFID_GET_CARDID_ERROR		0x0F
#define RFID_GET_CARDTYPE_SUCCESS	0x10
#define RFID_GET_CARDTYPE_ERROR		0x11
#define RFID_WRITE_CRCBLK_SUCCESS	0x12
#define RFID_WRITE_CRCBLK_ERROR		0x13
#define RFID_OPER_SUCCESS			0x14
#define RFID_OPER_ERROR				0x15
#define RFID_COM_ERROR				0x16
*/
// com 串口号 Listenter回调
extern "C" _declspec(dllexport) void InitFrameProcessor(int com, OnReportListener Proc);
extern "C" _declspec(dllexport) void DeInitFrameProcessor();
// 声音成功失败
extern "C" _declspec(dllexport) void BeepOk();
extern "C" _declspec(dllexport) void BeepErr();
//读用户区系统区
extern "C" _declspec(dllexport) int ReadUserInfo(unsigned char* buf);
extern "C" _declspec(dllexport) int ReadSysInfo(unsigned char* buf);
//写用户区与系统区
extern "C" _declspec(dllexport) int WriteUserInfo(unsigned char *buf, int buflen);
extern "C" _declspec(dllexport) int WriteSysInfo(unsigned char *buf, int buflen);
//写卡当前状态
extern "C" _declspec(dllexport) int WriteCrcBlkToCard(int CardType);
// RFID_CARD_NONE	 无
// RFID_CARD_INIT	 卡初始化
// RFID_CARD_USER	 只写用户区写
// RFID_CARD_SYS	 只写系统区
// RFID_CARD_ALL	 用户区系统区已写

//获取卡号 Data 数据 6bit卡号
extern "C" _declspec(dllexport) int GetCardId(unsigned char* buf);

//获取卡号和类型 数据 6bit卡号+1bit卡类型
extern "C" _declspec(dllexport) int GetCardType(unsigned char* buf);

// 打开串口
extern "C" _declspec(dllexport) void OpenCom(int com);
