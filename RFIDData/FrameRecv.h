#ifndef _FRAMERECV_
#define _FRAMERECV_

#include "RFIDCom.h"

#define COM_RECV_HEAD	0x00
#define COM_RECV_DATA	0x01
#define RECVBUFFER_LEN  (1024*5)
#define BUFFERFRAMELEN  6

class FrameRecv:public FrameData
{
public:
	FrameRecv();
	~FrameRecv();
	
	void FrameRecvStart(RFIDCom* p);
	void FrameRecvStop();
	void FrameRecvData(unsigned char* data, int datalen);
	bool IsThreadRun(){return mThreadRun;}
	void RevcCom();
private:
	bool ReadFrame();

private :
	unsigned char* mBuffer;
	int mBufferReadLen;
    int mBufferFrameLen;
    int mReadState;

	HANDLE mThreadRecv;
	DWORD mThreadRecvID;
	bool mThreadRun;
	RFIDCom* mRFIDCom;
};


#endif