#include "stdafx.h"
#include <iostream>
#include "FrameTypeDef.h"
#include "FrameData.h"
#include "FrameRecv.h"

using namespace std;

static DWORD WINAPI RecvThread(LPVOID Param);

FrameRecv::FrameRecv()
{
	mBuffer = new unsigned char[RECVBUFFER_LEN];
	mBufferReadLen = 0;
    mBufferFrameLen = BUFFERFRAMELEN;
    mReadState = COM_RECV_HEAD;
}

FrameRecv::~FrameRecv()
{
	delete[] mBuffer;
}

void FrameRecv::FrameRecvStart(RFIDCom* p)
{
	mRFIDCom = p;

	mThreadRun = true;	
	mThreadRecv = CreateThread(NULL, 0, RecvThread, this, 0, &mThreadRecvID); 
}

void FrameRecv::FrameRecvStop()
{
	mThreadRun = false;
	if(mThreadRecv != NULL)
	{
		CloseHandle(mThreadRecv);
		mThreadRecv = NULL;
	}
}

bool FrameRecv::ReadFrame()
{
    int i,j,aiindex = 0;
    bool mbool = false;
    unsigned char buffer[1024];

    if (mReadState == COM_RECV_HEAD){
        if(mBufferReadLen >= BUFFERFRAMELEN){
        	for (i = 0; i < mBufferReadLen; i++){
                if ((mBuffer[i]&0xFF) == 0xaa){
                	aiindex = i;
                	break;
                }
            }
        	
        	if (aiindex > 0){
            	mBufferReadLen -= aiindex;
            	if(mBufferReadLen > 0){
            		for (j = 0; j < mBufferReadLen; j++)
                    {
                        mBuffer[j] = mBuffer[aiindex];
                        aiindex++;
                    }
            	}
            }
        	
        	if(mBufferReadLen > BUFFERFRAMELEN){
        		if(mBuffer[BUFFERFRAMELEN-1] > 70){
        			mBufferReadLen = 0;
        			mBufferFrameLen = BUFFERFRAMELEN;
        		}
        		else{
        			mBufferFrameLen = BUFFERFRAMELEN+mBuffer[BUFFERFRAMELEN-1];
        			mReadState = COM_RECV_DATA;
        		}
        	}
        }
    } 
    
    if (mReadState == COM_RECV_DATA){
    	if (mBufferReadLen >= mBufferFrameLen){
			//mRFIDCom->Printf(_T("Recv  "),mBuffer,mBufferReadLen);
			SerialDataUnPack(mBuffer, mBufferFrameLen);
    		mBufferReadLen -= mBufferFrameLen;
    		if(mBufferReadLen > 0){
    			for (j = 0; j < mBufferReadLen; j++){
    				buffer[j] = mBuffer[mBufferFrameLen+j];
    			}
    			for(j = 0; j < RECVBUFFER_LEN;j++){
    				mBuffer[j] = 0;
    			}
    			for (j = 0; j < mBufferReadLen; j++){
    				mBuffer[j] = buffer[j];
    			}
    			mbool = true;
    		}
    		mReadState = COM_RECV_HEAD;
    		mBufferFrameLen = BUFFERFRAMELEN;
    	}
    }

    return mbool;
}

void FrameRecv::FrameRecvData(unsigned char* data, int datalen)
{
	bool mrunning = true;
	if(RECVBUFFER_LEN <= (mBufferReadLen+datalen))
	{
		while(mrunning){
        	mrunning = ReadFrame();
        }
	}
	
	if(datalen > 0){
		for (int i = 0; i < datalen; i++){
            mBuffer[mBufferReadLen+i] = data[i];
        }
		mBufferReadLen += datalen;
		mrunning = true;
        while(mrunning){
        	mrunning = ReadFrame();
        }
	}
}

void FrameRecv::RevcCom()
{
	if (mRFIDCom != NULL)
	{
		unsigned char buffer[200];
		int nlen = 1;
		int len = mRFIDCom->RFIDComRecv(buffer,nlen);
		
		if(len > 0)
		{
			FrameRecvData(buffer,len);
		}
	}
}

static DWORD WINAPI RecvThread(LPVOID Param)
{
	FrameRecv* nFrameRecv = (FrameRecv*)Param;
	while(nFrameRecv->IsThreadRun())
	{
		nFrameRecv->RevcCom();
		Sleep(1);
	}
	return 0;

}