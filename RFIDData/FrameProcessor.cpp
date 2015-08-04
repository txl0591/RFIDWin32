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

using namespace std;

static void DistributeReport (void* mFrameProcessor,int Id, int Cmd, int Ack, unsigned char* Data, int DataLen);
static void ResponsionReport (void* mFrameProcessor,int Id, int Cmd, int Ack, unsigned char* Data, int DataLen);
static void SendReportListener(void* Param, int Echo);

FrameProcessor::FrameProcessor(int com, OnEchoReportListener Listener)
{
	mOnEchoReportListener = Listener;
	if(0 == com)
	{
		mRFIDCom.RFIDComScan();
	}
	else
	{
		mRFIDCom.SetRFIDCom(com);
	}
	mRFIDCom.RFIDComOpen();
	mFrameSend.SetRFIDCom(&mRFIDCom);
	mFrameSend.SetSendReportListener(SendReportListener, this);
	mFrameRecv.SerialDataReportListener(DistributeReport, ResponsionReport, this);
	mFrameRecv.FrameRecvStart(&mRFIDCom);	
}

FrameProcessor::~FrameProcessor()
{
	
}

static void DistributeReport (void* Param, int Id, int Cmd, int Ack, unsigned char* Data, int DataLen)
{
	FrameProcessor* mFrameProcessor = (FrameProcessor*)Param;
	switch (Cmd)
	{
		case RFID_POWER_SYNC:
			mFrameProcessor->PowerOnInit();
			break;
	}
}

static void ResponsionReport (void* Param,int Id, int Cmd, int Ack, unsigned char* Data, int DataLen)
{
	unsigned char card[7];
	FrameProcessor* mFrameProcessor = (FrameProcessor*)Param;
	
	if(NULL == mFrameProcessor)
	{
		return;	
	}

	switch (Cmd)
	{
	case RFID_BEEP:
		break;

	case RFID_WRITE_MULT_BLOCK:
		mFrameProcessor->WriteCardOper();
		break;

	case RFID_READ_MULT_BLOCK:
		mFrameProcessor->ReadMultBlk(Data,DataLen);
		break;

	case RFID_CLR_PASSWD:
		if(Data[0] == ECHO_OK)
		{
			mFrameProcessor->setStateReport(RFID_CLEAR_PWD_SUCCESS, NULL, 0);
		}
		else
		{
			mFrameProcessor->setStateReport(RFID_CLEAR_PWD_ERROR, NULL, 0);
		}
		break;
		
	case RFID_WRITE_BLOCK:
		if(Data[0] == ECHO_OK)
		{
			if(mFrameProcessor->GetOperState() == RFID_WRITE_CRC)
			{
				mFrameProcessor->setStateReport(RFID_WRITE_CRCBLK_SUCCESS, NULL, 0);
			}
			else
			{
				mFrameProcessor->setStateReport(RFID_WRITE_SUCCESS, NULL, 0);
			}
			
		}
		else
		{
			if(mFrameProcessor->GetOperState() == RFID_WRITE_CRC)
			{
				mFrameProcessor->setStateReport(RFID_WRITE_CRCBLK_ERROR, NULL, 0);
			}
			else
			{
				mFrameProcessor->setStateReport(RFID_WRITE_ERROR, NULL, 0);
			}	
		}
		break;
		
	case RFID_READ_CARDID:
		if(Data[0] == ECHO_OK)
		{
			for (int i = 0; i < 6; i++)
			{
				card[i] = Data[1+i];
			}
			mFrameProcessor->setStateReport(RFID_GET_CARDID_SUCCESS, card, 6);
		}
		else
		{
			mFrameProcessor->setStateReport(RFID_GET_CARDID_ERROR, NULL, 0);
		}
		break;
		
	case RFID_GET_CRCBLK:
		if(Data[0] == ECHO_OK)
		{
			for (int i = 0; i < 7; i++)
			{
				card[i] = Data[1+i];
			}
			mFrameProcessor->setStateReport(RFID_GET_CARDTYPE_SUCCESS, card, 7);
		}
		else
		{
			mFrameProcessor->setStateReport(RFID_GET_CARDTYPE_ERROR, NULL, 0);
		}
		break;
	}
}

static void SendReportListener(void* Param, int Echo)
{
	FrameProcessor* mFrameProcessor = (FrameProcessor*)Param;

	if (NULL == mFrameProcessor)
	{
		return;
	}
	mFrameProcessor->setStateReport(RFID_COM_ERROR, NULL, 0);
}

void FrameProcessor::setStateReport(int Echo, unsigned char* Data, int DataLen)
{
	if(mOnEchoReportListener != NULL){
		mOnEchoReportListener(Echo, Data, DataLen);
	}
}

void FrameProcessor::BeepOk()
{
	unsigned char buffer = RFID_BEEP_OK;
	mFrameSend.SendAck(RFID_BEEP,&buffer,1);
}

void FrameProcessor::BeepErr()
{
	unsigned char buffer = RFID_BEEP_ERR;
	mFrameSend.SendAck(RFID_BEEP,&buffer,1);
}

void FrameProcessor::PowerOnInit()
{
    unsigned char buffer = ADDR_PC;
    mFrameSend.SendNoAck(RFID_POWER_SYNC,&buffer,1);
}

void FrameProcessor::GetCardId()
{
	unsigned char buffer = 0;
	mFrameSend.SendAck(RFID_READ_CARDID,&buffer,1);
}
    
void FrameProcessor::GetCardType()
{
	unsigned char buffer[2] = {0, RFID_PWD_A};
   	mFrameSend.SendAck(RFID_GET_CRCBLK, buffer, 2);
}


void FrameProcessor::ClearUserPwdToCard()
{
	unsigned char buffer[4];
	buffer[0] = 0x00;
	buffer[1] = RFID_USER_START;
	buffer[2] = RFID_USER_LEN;
	buffer[3] = RFID_PWD_A;
	mFrameSend.SendAck(RFID_CLR_PASSWD, buffer, 4);
}

void FrameProcessor::WriteUserPwdToCard()
{
	unsigned char buffer[4];
	buffer[0] = 0x00;
	buffer[1] = RFID_USER_START;
	buffer[2] = RFID_USER_LEN;
	buffer[3] = RFID_PWD_A;
	mFrameSend.SendAck(RFID_SET_PASSWD, buffer, 4);
}

void FrameProcessor::ClearSysPwdToCard()
{
	unsigned char buffer[4];
	buffer[0] = 0x00;
	buffer[1] = RFID_SYS_START;
	buffer[2] = RFID_SYS_LEN;
	buffer[3] = RFID_PWD_A;
	mFrameSend.SendAck(RFID_CLR_PASSWD, buffer, 4);
}

void FrameProcessor::WriteSysPwdToCard()
{
	unsigned char buffer[4];
	buffer[0] = 0x00;
	buffer[1] = RFID_SYS_START;
	buffer[2] = RFID_SYS_LEN;
	buffer[3] = RFID_PWD_A;
	mFrameSend.SendAck(RFID_SET_PASSWD, buffer, 4);
}

void FrameProcessor::ClearAllPwdToCard()
{
	unsigned char buffer[4];
	buffer[0] = 0x00;
	buffer[1] = RFID_CRC_START;
	buffer[2] = RFID_USER_LEN+RFID_CRC_LEN;
	buffer[3] = RFID_PWD_A;
	mFrameSend.SendAck(RFID_CLR_PASSWD, buffer, 4);
}

void FrameProcessor::WriteCrcBlkToCard(int CardType)
{
	unsigned char crc[18] ={0x2A,0x61,0x66,0x5E,0x66,0x39,0x6F,0x32,0x77,0x35,0x66,0x76,0x46,0x31,0x23,0x51, 0xFF,0xFF};
	unsigned char buffer[21];
	
	mOperState = RFID_WRITE_CRC;

	buffer[0] = 0x00;
	buffer[1] = RFID_CRC_START;
	buffer[2] = RFID_PWD_A;
	
	switch (CardType){
	case RFID_CARD_NONE:
		break;

	case RFID_CARD_INIT:
		crc[16] = 0x00;
		crc[17] = 0x00;
		break;

	case RFID_CARD_USER:
		crc[16] = 0x01;
		break;

	case RFID_CARD_SYS:
		crc[17] = 0x01;
		break;

	case RFID_CARD_ALL:
		crc[16] = 0x01;
		crc[17] = 0x01;
		break;
	}
	memcpy(&buffer[3],crc,18);
	mFrameSend.SendAck(RFID_WRITE_BLOCK, buffer, 21);
}


void FrameProcessor::ReadMultBlk(unsigned char* Data, int DataLen)
{
	int i;
	if(Data[0] == ECHO_OK){
		switch (Data[1]){
		case READ_MULT_START:
			RecvbufferLen = 0;
			break;
				
		case READ_MULT_CENTER:
			for(i = 0; i < 16; i++){
				Recvbuffer[RecvbufferLen] = Data[2+i];
				RecvbufferLen++;
			}
			break;
			
		case READ_MULT_END:
			if(mOperState ==  RFID_READ_USER){
				setStateReport(RFID_READ_USER_SUCCESS, Recvbuffer, RecvbufferLen);
			}else{
				setStateReport(RFID_READ_SYS_SUCCESS, Recvbuffer, RecvbufferLen);
			}
			RecvbufferLen = 0;
			mOperState = RFID_OPER_NONE;
			break;	
		}
	}
	else{
		if(mOperState ==  RFID_READ_USER){
			setStateReport(RFID_READ_USER_ERROR, NULL, 0);
		}else{
			setStateReport(RFID_READ_SYS_ERROR, NULL, 0);
		}
		mOperState = RFID_OPER_NONE;
	}
}


void FrameProcessor::ReadUserInfo()
{
	unsigned char buffer[5];
	mOperState = RFID_READ_USER;
	buffer[0] = 0x00;
	buffer[1] = RFID_USER_START;
	buffer[2] = RFID_USER_LEN;
	buffer[3] = RFID_PWD_A;
	buffer[4] = RFID_PWD_GET;
	mFrameSend.SendAck(RFID_READ_MULT_BLOCK, buffer, 5);
}

void FrameProcessor::ReadSysInfo()
{
	unsigned char buffer[5];
	mOperState = RFID_READ_SYS;

	buffer[0] = 0x00;
	buffer[1] = RFID_SYS_START;
	buffer[2] = RFID_SYS_LEN;
	buffer[3] = RFID_PWD_A;
	buffer[4] = RFID_PWD_GET;
	mFrameSend.SendAck(RFID_READ_MULT_BLOCK, buffer, 5);
}

void FrameProcessor::WriteCardOper()
{
	if(mWriteLen < mWriteBufLen)
	{
		int blk = mWriteLen/48;
		int blkindex = (mWriteLen%48)/16;
		
		unsigned char buffer[20];
    	buffer[0] = 0x00;
    	buffer[1] = (mWriteBlk+blk);
    	buffer[2] = blkindex;
    	buffer[3] = RFID_PWD_A;
		memcpy(&buffer[4],mWritebuf+mWriteLen,16);
    	mWriteLen+=16;
    	mFrameSend.SendAck(RFID_WRITE_MULT_BLOCK, buffer, 20);
	}
	else
	{
		mOperState = RFID_OPER_NONE;
		if(mWriteBufLen == mWriteLen)
		{
			if (mOperState == RFID_WRITE_USER)
			{
				setStateReport(RFID_WRITE_USER_SUCCESS, NULL, 0);	
			}
			else
			{
				setStateReport(RFID_WRITE_SYS_SUCCESS, NULL, 0);	
			}
		}
		else
		{
			if (mOperState == RFID_WRITE_USER)
			{
				setStateReport(RFID_WRITE_USER_ERROR, NULL, 0);	
			}
			else
			{
				setStateReport(RFID_WRITE_SYS_ERROR, NULL, 0);	
			}
		}
	}
}

void FrameProcessor::WriteUserInfo(unsigned char *buf, int buflen)
{
	mOperState = RFID_WRITE_USER;
	mWriteBlk = RFID_USER_START;
	memcpy(mWritebuf,buf, buflen);
	mWriteBufLen = buflen;
	mWriteLen = 0;
	WriteCardOper();
}

void FrameProcessor::WriteSysInfo(unsigned char *buf, int buflen)
{
	mOperState = RFID_WRITE_SYS;
	mWriteBlk = RFID_SYS_START;
	memcpy(mWritebuf,buf, buflen);
	mWriteBufLen = buflen;
	mWriteLen = 0;
	WriteCardOper();
}

void FrameProcessor::FrameProcessorStop()
{	
	mFrameSend.FrameSendStop();
	mFrameRecv.FrameRecvStop();
	mRFIDCom.RFIDComClose();
}

void FrameProcessor::FrameComOpen(int com)
{
	mRFIDCom.SetRFIDCom(com);
	mRFIDCom.RFIDComOpen();
}