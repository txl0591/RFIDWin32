#ifndef FrameTypeDef
#define FrameTypeDef

/* FrameDef */
#define FRAME_START		0
#define FRAME_VERSION	1
#define FRAME_CTRL		2
#define FRAME_DST		3
#define FRAME_SRC		4
#define FRAME_LEN		5
#define FRAME_ID		6
#define FRAME_CMD		7
#define FRAME_DATA		8

/* FrameAddr */
#define ADDR_NONE		0xFF
#define ADDR_RFID		0
#define ADDR_PC			1
#define ADDR_ANDROID	2

/* FrameType */
#define FRAME_SEND		0x00
#define FRAME_ACK		0x80

/* FrameAck */
#define ACK_NEED		0
#define ACK_NONEED		1

/* FrameCommond */
#define RFID_ANTENNA			0x00
#define RFID_CLR_PASSWD			0x11
#define RFID_SET_PASSWD			0x12
#define RFID_GET_PASSWD			0x13

#define RFID_SET_CRCBLK			0x14
#define RFID_GET_CRCBLK			0x15

#define RFID_WRITE_BLOCK		0x21
#define RFID_READ_BLOCK			0x22
#define RFID_WRITE_MULT_BLOCK   0x23
#define RFID_READ_MULT_BLOCK	0x24
#define RFID_READ_CARDID		0x25

#define RFID_GET_VERSION		0x30
#define RFID_POWER_SYNC			0x31
#define RFID_BEEP				0x32
#define RFID_RECOVER_SYS		0x33

/* FrameCommondEcho */
#define ECHO_OK 			 0x00
#define ECHO_ERR 			 0x01
#define ECHO_ERR_SIZE 		 0x02
#define ECHO_ERR_COLLISION   0x03
#define ECHO_ERR_PASSWD 	 0x04
#define ECHO_ERR_WRITEDATA 	 0x05
#define ECHO_ERR_READDATA 	 0x06

/* RFIDBeep */
#define RFID_BEEP_OK		0x00
#define RFID_BEEP_ERR		0x01

/* RFIDAntenna */
#define RFID_ANTENNA_ON		0x00
#define RFID_ANTENNA_OFF	0x01

/* RFIDMult */
#define READ_MULT_START		0x01
#define READ_MULT_CENTER	0x10
#define READ_MULT_END		0x11
 
/* RFIDPwdMode */
#define RFID_PWD_A			0x60
#define RFID_PWD_B			0x61

/* RFIDPwdType */
#define RFID_PWD_DEFAULT	0x01
#define RFID_PWD_GET		0x00
    
 /* RFIDUserBlk */ 
#define RFID_USER_START		2
#define RFID_USER_LEN		11

/* RFIDSysBlk */
#define RFID_SYS_START		13
#define RFID_SYS_LEN		3

/* RFIDCrcBlk */
#define RFID_CRC_START		1
#define RFID_CRC_LEN		1


/*RFIDUserType*/
#define RFID_ADMIN			0x00
#define RFID_USER			0x01
#define RFID_GUEST			0x02
    
/*RFIDState 操作返回状态*/
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
    
/*RFIDOper*/
#define RFID_OPER_NONE		0x00
#define RFID_READ_USER		0x01
#define RFID_READ_SYS		0x02
#define RFID_WRITE_USER		0x03
#define RFID_WRITE_SYS		0x04
#define RFID_READ_SYSINFO	0x05
#define RFID_CLEAR_PWD		0x06
#define RFID_WRITE_CRC		0x07
#define RFID_READCARD_ID	0x08
#define RFID_READCARD_TYPE	0x09
    
/*RFID_CARD_TYPE 卡类型*/
#define RFID_CARD_NONE	 0x00
#define RFID_CARD_INIT	 0x01
#define RFID_CARD_USER	 0x02
#define RFID_CARD_SYS	 0x03
#define RFID_CARD_ALL	 0x04	
    
#endif