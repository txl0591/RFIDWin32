// RFIDData.h : RFIDData DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
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

// ��ʼ�� Listenter Ϊtypedef void (*OnEchoReportListener)(int Echo, unsigned char* Data, int DataLen); �������ݾ��ɸûص��ش�
// Data ���� DataLen���ݳ���
// Echo Ӧ��ֵ 

//���û����ɹ�ʧ��
// RFID_READ_USER_SUCCESS		
// RFID_READ_USER_ERROR		

//��ϵͳ��
// RFID_READ_SYS_SUCCESS
// RFID_READ_SYS_ERROR

//д�û���
// RFID_WRITE_USER_SUCCESS		
// RFID_WRITE_USER_ERROR		

//дϵͳ��
// RFID_WRITE_SYS_SUCCESS		
// RFID_WRITE_SYS_ERROR		

//��ȡ����
// RFID_GET_CARDID_SUCCESS		
// RFID_GET_CARDID_ERROR		

//��ȡ�����Լ���״̬
// RFID_GET_CARDTYPE_SUCCESS	
// RFID_GET_CARDTYPE_ERROR		

//д����ǰ״̬
// RFID_WRITE_CRCBLK_SUCCESS	
// RFID_WRITE_CRCBLK_ERROR		

/*RFIDState ��������״̬
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
// com ���ں� Listenter�ص�
extern "C" _declspec(dllexport) void InitFrameProcessor(int com, OnReportListener Proc);
extern "C" _declspec(dllexport) void DeInitFrameProcessor();
// �����ɹ�ʧ��
extern "C" _declspec(dllexport) void BeepOk();
extern "C" _declspec(dllexport) void BeepErr();
//���û���ϵͳ��
extern "C" _declspec(dllexport) int ReadUserInfo(unsigned char* buf);
extern "C" _declspec(dllexport) int ReadSysInfo(unsigned char* buf);
//д�û�����ϵͳ��
extern "C" _declspec(dllexport) int WriteUserInfo(unsigned char *buf, int buflen);
extern "C" _declspec(dllexport) int WriteSysInfo(unsigned char *buf, int buflen);
//д����ǰ״̬
extern "C" _declspec(dllexport) int WriteCrcBlkToCard(int CardType);
// RFID_CARD_NONE	 ��
// RFID_CARD_INIT	 ����ʼ��
// RFID_CARD_USER	 ֻд�û���д
// RFID_CARD_SYS	 ֻдϵͳ��
// RFID_CARD_ALL	 �û���ϵͳ����д

//��ȡ���� Data ���� 6bit����
extern "C" _declspec(dllexport) int GetCardId(unsigned char* buf);

//��ȡ���ź����� ���� 6bit����+1bit������
extern "C" _declspec(dllexport) int GetCardType(unsigned char* buf);

// �򿪴���
extern "C" _declspec(dllexport) void OpenCom(int com);
