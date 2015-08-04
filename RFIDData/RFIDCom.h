#ifndef _RFIDCOM_
#define _RFIDCOM_

#define USB_SERIAL_CH340 "USB-SERIAL CH340"

#define DEFAULE_COM USB_SERIAL_CH340

#define _COM_DEBUG_		1

class RFIDCom
{
public:
	RFIDCom();
	~RFIDCom();
	
	void SetRFIDCom(int com);
	void RFIDComScan();
	bool RFIDComIsOpen(){return mComState;}
	bool RFIDComOpen();
	bool RFIDComClose();
	bool RFIDComSend(unsigned char * buf, int buflen);
	int RFIDComRecv(unsigned char * buf, int buflen);
	void Printf(CString Str,unsigned char* buffer, int bufferlen);

private:
	CString mComName;
	HANDLE mComHandle;
	bool mComState;
	bool mComSend;
	int Com;
};

#endif