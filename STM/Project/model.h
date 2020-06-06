#ifndef EG_MODEL_H
#define EG_MODEL_H

#include <stdio.h>
#include <string.h>
//#include "stm32f10x.h"
extern const unsigned char MODEL_TOKEN[5];

void ModelInit(char *id);


//协议头
#pragma pack(1)
typedef struct Model_Header
{
	char				token[5];	//包头校验位
	unsigned short		len;		//包总长度
	unsigned char		type;		//协议编号
	unsigned char		id[12];		//设备芯片ID
}MODEL_HEADER_S;

//解析头
MODEL_HEADER_S *ModelCheckBuff(char *buff, int len, char **contentBuff, int *contentLen);

//创建返回包数据
//buff: 接受封包的缓存 （内存分配建议2K）
//type: 协议号
//id：设备ID(长度必须为 12字节)
//contentLen: 内容长度 （总长度必须小于4000字节）
int ModelMakeHeader(char *buff, long type, int contentLen);

//////////////////////////////////////////////////////////////////////////////////////
#pragma pack(1)
typedef struct Model_Code
{
	MODEL_HEADER_S	header;				//协议头信息
	unsigned char	code;
}MODEL_CODE_S;
int ModelMakeCodeAck(char *buff, long type, char code);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//获取设备ID请求，和注册局域网服务器 - 协议号:1
#pragma pack(1)
typedef struct DevIdReq
{
	MODEL_HEADER_S	header;							//协议头信息
	unsigned char   isRegServer;				//注册服务client
	unsigned short  port;								//端口号
	char						isIotOK;						//是否连接上IOT服务器
}DEV_ID_REQ_S;

//返回设备设备ID和授权信息 - 协议号:2
#pragma pack(1)
typedef struct DevIdAck
{
	MODEL_HEADER_S	header;				//协议头信息
	char			name[20];						//阿里云name
	char			secret[30];					//阿里云授权码
	char			isFinger;						//是否安装指纹模块
	char			type;								//设备类型
}DEV_ID_ACK_S;
//name:设备别名
//secret：设备授权码
long ModelMakeDevIdAck(char *retBuff, char *name, char *secret, char isFinger, char type);

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//ext_buff: 附加buff
//ext_buff_len: 附加buff长度
long ModelMakeInfoExtAck(char *retBuff, char *ext_buff, int ext_buff_len);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//获取转换器本身信息 -协议号:5

//返回转换器信息  - 协议号:6

//isWifi:
//hummer:
//:
//ip:
//:
//:代码版本

#pragma pack(1)
typedef struct DevInfoAck
{
	unsigned char		relay1;				//一号继电器是否打开(1为打开 0关闭)
	unsigned char		relay2;				//二号继电器是否打开(1为打开 0关闭)
	unsigned char		isWifi;				//WIFI模块是否安装(1为安装 0未安装)
	unsigned char		hummer;				//蜂鸣器是否正在响(1为打开 0关闭)
	unsigned char		isCardReader;	//读卡器模块是否安装(1为安装 0未安装)
	char						ip[16];				//设备内网IP
	unsigned char		isFinger;			//是否安装指纹模块
	char						groupId[50];	//分组ID
	char						codeVersion[12];	//固件版本
	unsigned char		clientType;		//设备类型
	unsigned char		RV[20];				//控制器软件版本
}DEV_INFO_ACK_S;

long ModelMakeInfoAck(char *retBuff, DEV_INFO_ACK_S *info);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//获取集线器信息 -协议号:7
//返回集线器信息 -协议号:8
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//继电器控制 - 协议号:51
#pragma pack(1)
typedef struct DevConRelayReq
{
	MODEL_HEADER_S	header;			//协议头信息
	unsigned char		relayId;		//继电器编号
	unsigned char   control;		//继电器控制(0断开1联通)
	unsigned int		duration;		//继电器打开时长
}DEV_CON_RELAY_REQ_S;
long ModelMakeConRelayReq(char *buff, unsigned char	relayId, unsigned char  control, unsigned int duration);

//返回空 - 协议号:52
long ModelMakeConRelayAck(char *retBuff);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//蜂鸣器控制 - 协议号:53
#pragma pack(1)
typedef struct DevConBuzzerReq
{
	MODEL_HEADER_S	header;			//协议头信息
	unsigned char   plan;			//蜂鸣器鸣叫方案，参照文档5.3
}DEV_CON_BUZZER_REQ_S;
//返回空 - 协议号:54
long ModelMakeConBuzzerAck(char *retBuff);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//组合交互控制 - 协议号:55
#pragma pack(1)
typedef struct DevConGroupReq
{
	MODEL_HEADER_S	header;			//协议头信息
	unsigned char   plan;				//组合控制方案
}DEV_CON_GROUP_REQ_S;
long ModelMakeConGroupReq(char *retBuff, unsigned char plan);

//返回空 - 协议号:56
long ModelMakeConGroupAck(char *retBuff);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//播放录音 - 协议号:57
#pragma pack(1)
typedef struct DevVoiceReq
{
	MODEL_HEADER_S	header;			//协议头信息
	unsigned char   num;				//播放地址
}DEV_VOICE_REQ_S;

//返回空 - 协议号:58
long ModelMakeVoiceAck(char *retBuff);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//同步力量器械状态 -协议号:73
//返回力量器械状态 -协议号:74
long ModelMakeStrengthStatusAck(char *retBuff, unsigned char code, unsigned int update_time, unsigned int show_index, unsigned int strength_count, unsigned char status);
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(1)
typedef struct DevStrengthPlReq
{
	MODEL_HEADER_S	header;			//协议头信息
	unsigned char   status;			//0关闭1打开
	unsigned int		space_time;	//数片数有效时间
	unsigned int		one_time;		//一次锻炼的有效间隔时间
	unsigned char		beep;				//是否开启归位提示音
}DEV_STRENGTH_PL_REQ_S;
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//获取单个指纹模板 -协议号:107
//返回单个指纹模板 -协议号:108
long ModelMakeFingerTmpAck(char *retBuff, char *tmp, int dataLen);
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//获取指纹器所有用户 -协议号:109
//返回指纹器所有用户 -协议号:110
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//外部设备控制- 协议号:253
//返回空 - 协议号:254
long ModelMakeExtAck(char *retBuff);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//推送RFID-151
long ModelMakeRfidReq(char *retBuff, char *rfid);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//推送指纹-153
long ModelMakeFingerReq(char *retBuff, short userID);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//推送按键 -155
long ModelMakeBtnReq(char *retBuff, char btnId, int num, unsigned int spend);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//健身数据推送 -157
long ModelMakeStrengthReq(char *retBuff, unsigned int value1, unsigned int value2, unsigned int value3, char *btn1, char *btn2);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//进入待机状态 -163
long ModelMakeInStandbyReq(char *retBuff, unsigned int show_index);
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//任意推送信息 - 191(必须满足json结构 "key":"value")
long ModelMakeUserPushReq(char *retBuff, char *user_info, int len);
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//推送232串口透传数据
long ModelMakePush232ExtReq(char *retBuff, char *user_info, int len);

//推送TTL串口透传数据
long ModelMakePushTTLExtReq(char *retBuff, char *user_info, int len);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//指纹登记返回结果 - 协议号:159
#pragma pack(1)
typedef struct FingerPushScan
{
	MODEL_HEADER_S	header;			//协议头信息
	unsigned short  user_id;		//用户ID
	unsigned char   status;			//1成功 0失败
	
}FINGER_PUSH_SCAN_S;
long ModelMakeFingerScanReq(char *retBuff, unsigned short user_id, unsigned char status);
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
