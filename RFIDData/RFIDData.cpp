// RFIDData.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "RFIDData.h"

FrameProcessor* mFrameProcessor; 
OnReportListener mOnEchoReportListener = NULL;
int mRFIDState = RFID_OPER_NONE;
unsigned char * mRecvBuf = NULL;

static void EchoReportListener(int Echo, unsigned char* Data, int DataLen)
{	
	switch (Echo)
	{
	case RFID_READ_USER_SUCCESS:
	case RFID_READ_SYS_SUCCESS:
	case RFID_GET_CARDID_SUCCESS:
	case RFID_GET_CARDTYPE_SUCCESS:
		if (mRecvBuf != NULL && Data != NULL && DataLen > 0)
		{
			memcpy(mRecvBuf, Data, DataLen);
		}
		break;
	}


	switch (Echo)
	{
		case RFID_READ_USER_SUCCESS:
		case RFID_READ_SYS_SUCCESS:
		case RFID_READ_USER_ERROR:
		case RFID_READ_SYS_ERROR:
		case RFID_WRITE_USER_SUCCESS:
		case RFID_WRITE_USER_ERROR:
		case RFID_WRITE_SYS_SUCCESS:
		case RFID_WRITE_SYS_ERROR:
		case RFID_GET_CARDID_SUCCESS:
		case RFID_GET_CARDID_ERROR:
		case RFID_GET_CARDTYPE_SUCCESS:
		case RFID_GET_CARDTYPE_ERROR:
		case RFID_WRITE_CRCBLK_SUCCESS:
		case RFID_WRITE_CRCBLK_ERROR:
		case RFID_COM_ERROR:
			mRFIDState = RFID_OPER_NONE;
			break;
	}

	if (NULL != mOnEchoReportListener)
	{
		mOnEchoReportListener(Echo, DataLen);
	}
}


void InitFrameProcessor(int com, OnReportListener Proc)
{
	mFrameProcessor = new FrameProcessor(com,EchoReportListener); 
	mOnEchoReportListener = Proc;
}

void BeepOk()
{
	mFrameProcessor->BeepOk();
}

void BeepErr()
{
	mFrameProcessor->BeepErr();
}

int ReadUserInfo(unsigned char* buf)
{
	if (mRFIDState != RFID_OPER_NONE)
	{
		return RFID_OPER_ERROR;
	}
	mRecvBuf = buf;
	mRFIDState = RFID_READ_USER;
	mFrameProcessor->ReadUserInfo();
	return RFID_OPER_SUCCESS;
}

int ReadSysInfo(unsigned char* buf)
{
	if (mRFIDState != RFID_OPER_NONE)
	{
		return RFID_OPER_ERROR;
	}
	mRecvBuf = buf;
	mRFIDState = RFID_READ_SYS;
	mFrameProcessor->ReadSysInfo();
	return RFID_OPER_SUCCESS;
}

int WriteUserInfo(unsigned char *buf, int buflen)
{
	if (mRFIDState != RFID_OPER_NONE)
	{
		return RFID_OPER_ERROR;
	}
	mRFIDState = RFID_WRITE_USER;
	mFrameProcessor->WriteUserInfo(buf, buflen);
	return RFID_OPER_SUCCESS;
}

int WriteSysInfo(unsigned char *buf, int buflen)
{
	if (mRFIDState != RFID_OPER_NONE)
	{
		return RFID_OPER_ERROR;
	}
	mRFIDState = RFID_WRITE_SYS;
	mFrameProcessor->WriteSysInfo(buf, buflen);
	return RFID_OPER_SUCCESS;
}

int WriteCrcBlkToCard(int CardType)
{
	if (mRFIDState != RFID_OPER_NONE)
	{
		return RFID_OPER_ERROR;
	}
	mRFIDState = RFID_WRITE_CRC;
	mFrameProcessor->WriteCrcBlkToCard(CardType);
	return RFID_OPER_SUCCESS;

}

int GetCardId(unsigned char* buf)
{
	if (mRFIDState != RFID_OPER_NONE)
	{
		return RFID_OPER_ERROR;
	}
	mRecvBuf = buf;
	mRFIDState = RFID_READCARD_ID;
	mFrameProcessor->GetCardId();
	return RFID_OPER_SUCCESS;
}
    
int GetCardType(unsigned char* buf)
{
	if (mRFIDState != RFID_OPER_NONE)
	{
		return RFID_OPER_ERROR;
	}
	mRecvBuf = buf;
	mRFIDState = RFID_READCARD_TYPE;
	mFrameProcessor->GetCardType();
	return RFID_OPER_SUCCESS;
}

void OpenCom(int com)
{
	mFrameProcessor->FrameComOpen(com);
}

void DeInitFrameProcessor()
{
	mFrameProcessor->FrameProcessorStop();
	if(mFrameProcessor)
	{
		//delete[] mFrameProcessor;
		mFrameProcessor = NULL;
	}
}