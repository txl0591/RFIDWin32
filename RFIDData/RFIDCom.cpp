#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
#include <iomanip>
#include <iostream>
#include<string.h>
#include <objbase.h>
#include <initguid.h>
#include <Setupapi.h>
#include "RFIDCom.h"

using namespace std;

RFIDCom::RFIDCom()
{
	mComHandle = INVALID_HANDLE_VALUE;
	mComState = FALSE;
	Com = 0;
}

void RFIDCom::SetRFIDCom(int com)
{
	mComHandle = INVALID_HANDLE_VALUE;
	mComState = FALSE;
	char name[6];
	memset(name, 0,sizeof(name));
	sprintf(name,"COM%d",com);
	mComName = name;
}

RFIDCom::~RFIDCom()
{
	CloseHandle(mComHandle);
}

void RFIDCom::RFIDComScan()
{
	/*
	int m_nSerialPortNum(0);
	CString strSerialList[256]; 
	CArray<SSerInfo,SSerInfo&> asi;
	EnumSerialPorts(asi,TRUE);
	m_nSerialPortNum = (int)asi.GetSize();
	for (int i = 0; i < asi.GetSize(); i++)
	{
		CString str = asi[i].strPortDesc;
		if(str == DEFAULE_COM)
		{
			int len = str.GetLength();
			CString name = asi[i].strFriendlyName;
			int nlen = name.GetLength();
			int comlen = nlen-len-3;
			mComName = name.Mid(len+2,comlen);
			break;
		}
		

	}
	*/
}

bool RFIDCom::RFIDComClose()
{
	if(mComHandle)
	{
		CloseHandle(mComHandle);
		mComHandle = INVALID_HANDLE_VALUE;
	}
	return true;
}

bool RFIDCom::RFIDComOpen()
{	
	RFIDComClose();
	mComHandle = CreateFile(mComName,
						GENERIC_READ|GENERIC_WRITE, //�������д
						0, //��ռ��ʽ
						NULL,
						OPEN_EXISTING, //�򿪶����Ǵ���
						0, //ͬ����ʽ
						NULL);

	if(mComHandle ==INVALID_HANDLE_VALUE)
	{
		cout<<"�򿪴���ʧ��"<<endl;
		CloseHandle(mComHandle);
		mComState = FALSE;
	}
	else
	{
		mComState = TRUE;
		SetupComm(mComHandle,100,100); 
	
		COMMTIMEOUTS TimeOuts;
		//�趨����ʱ
		TimeOuts.ReadIntervalTimeout=MAXDWORD;
		TimeOuts.ReadTotalTimeoutMultiplier=0;
		TimeOuts.ReadTotalTimeoutConstant=0;
		//�ڶ�һ�����뻺���������ݺ���������������أ�
		//�������Ƿ������Ҫ����ַ���


		//�趨д��ʱ
		TimeOuts.WriteTotalTimeoutMultiplier=100;
		TimeOuts.WriteTotalTimeoutConstant=500;
		SetCommTimeouts(mComHandle,&TimeOuts); //���ó�ʱ

		DCB dcb;
		GetCommState(mComHandle,&dcb);
		dcb.BaudRate=115200; //������Ϊ9600
		dcb.ByteSize=8; //ÿ���ֽ���8λ
		dcb.Parity=NOPARITY; //����żУ��λ
		dcb.StopBits=TWOSTOPBITS; //����ֹͣλ
		SetCommState(mComHandle,&dcb);

		PurgeComm(mComHandle,PURGE_TXCLEAR|PURGE_RXCLEAR);
		cout<<"�򿪴��ڳɹ�"<<endl;
	}

	return mComState;
}

bool RFIDCom::RFIDComSend(unsigned char * buf, int buflen)
{
	bool ret = FALSE;
	DWORD len = buflen;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	if(mComHandle == INVALID_HANDLE_VALUE)
	{
		cout<<"���ڻ�δ��"<<endl;
		return ret;
	}
	ClearCommError(mComHandle,&dwErrorFlags,&ComStat);
	bWriteStat=WriteFile(mComHandle,buf,len, &len,NULL);
	if(!bWriteStat)
	{
		cout<<"д����ʧ��"<<endl;
		mComState = FALSE;
	}	
	else
	{
		return TRUE;
	}
	
	#if _COM_DEBUG_
	Printf(_T("Send Com "),buf,len);
	#endif

	return ret;
}

int RFIDCom::RFIDComRecv(unsigned char * buf, int buflen)
{
	char str[100];
	memset(str,0,100);
	DWORD wCount = buflen;
	BOOL bReadStat;

	if(mComHandle == INVALID_HANDLE_VALUE)
	{
		cout<<"���ڻ�δ��"<<endl;
		return 0;
	}

	bReadStat = ReadFile(mComHandle,str,wCount,&wCount,NULL);
	if(bReadStat)
	{
		if(wCount > 0)
		{
			memcpy(buf,str,wCount);
		}
		return wCount;
	}
	else
	{
		return 0;
	}
}

void RFIDCom::Printf(CString Str,unsigned char* buffer, int bufferlen)
{
	cout<<Str<<" Len["<<bufferlen<<"] ";
	for (int i = 0; i < bufferlen; i++)
	{
		cout<<" 0x"<<setw(2)<<setfill('0')<<hex<<(int)buffer[i];
	}
	cout<<" "<<endl;
}