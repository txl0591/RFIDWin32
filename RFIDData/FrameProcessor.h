#ifndef _FRAMEPROCESSOR_
#define _FRAMEPROCESSOR_

typedef void(*OnEchoReportListener)(int Echo, unsigned char* Data, int DataLen);

class FrameProcessor
{
public:
	FrameProcessor(int com, OnEchoReportListener Listener);
	~FrameProcessor();

	void FrameComOpen(int com);

	void FrameProcessorStop();	
	int GetOperState(){return mOperState;}
	
	void BeepOk();
	void BeepErr();
	
	void PowerOnInit();
	void GetCardId();
	void GetCardType();
	void ClearUserPwdToCard();
	void WriteUserPwdToCard();
	void ClearSysPwdToCard();
	void WriteSysPwdToCard();
	void ClearAllPwdToCard();
	void WriteCrcBlkToCard(int CardType);
	void setStateReport(int Echo, unsigned char* Data, int DataLen);

	void ReadMultBlk(unsigned char* Data, int DataLen);
	void ReadUserInfo();
	void ReadSysInfo();

	void WriteCardOper();
	void WriteUserInfo(unsigned char *buf, int buflen);
	void WriteSysInfo(unsigned char *buf, int buflen);

private: 
	OnEchoReportListener mOnEchoReportListener;
	FrameRecv mFrameRecv;
	FrameSend mFrameSend;
	RFIDCom mRFIDCom;
	unsigned char CardId[6];
	unsigned char Recvbuffer[1024];
	int RecvbufferLen;
	int mOperState;
	unsigned char mWritebuf[1024];
	int mWriteBufLen;
	int mWriteLen;
	int mWriteBlk;
};


#endif