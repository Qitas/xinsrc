#include "rc522.h"
#include "GPIOLIKE51.h"
#include "SYSTICK.h"

u8 RFID_UID[5];
u8 RFID_Buffer[18];
u8 RFID_Password_Buffer[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

//
void delay_ns(u32 ns)
{
  u32 i;
  for(i=0;i<ns;i++)
  {
    __nop();
    __nop();
    __nop();
  }
}


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������MFRC522	
//MFRC522 ֧��10Mbit/S��SPIģʽ�����ӻ����½��ش������ݡ���λ��ǰ
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��

//SPI2_SCK 					PA5
//SPI2_MISO					PA6
//SPI2_MOSI 				PA7
//RCC522_NSS(SDA��			PA4
//RCC522_RST(CE)  			PA8
//RCC522_IRQ 		  		��û��

/***************************************************************************
    - ����������STM32f103 SPI�ӿ����õĳ�ʼ������
    - ����ģ�飺STM32 SPI����
  - �������ԣ��ⲿ��ʹ�û�ʹ��
  - ����˵������
  - ����˵������
    - ����ʵ�ֲ��裺
   (1)SPI2��û����ӳ���������NSS->PA4��SCK->PA5��MISO->PA6��MOSI->PA7������STM32Ҫ��������ģʽ�������ģʽ������NSS����
   (2)��ʼ��GPIO�ܽź�SPI�Ĳ������ã�����SPI��GPIO�ĳ�ʼ���ṹ��
   (3)������GPIO��PB13��PB14��PB15ʱ��������Ϊ�������,�ڸ��ù�������,��������ķ���,��ȫ���ڲ�����.����Ҫ������.
   (4)����FLASH��Ƭѡ�ź���PB2������Ϊ�ߵ�ƽ��Ҳ���ǲ�ѡ��FLASH
   (5)��GPIO��SPI2��ʱ�� 
   (6)����SPI1�Ĳ���SPI�ķ��򡢹���ģʽ������֡��ʽ��CPOL��CPHA��NSS�������Ӳ����SPIʱ�ӡ����ݵĴ���λ���Լ�CRC
   (7)����SPI�ṹ���ʼ��������ʼ��SPI�ṹ�塢��ʹ��SPI1
   (8)����������䷢��һ��0xff����ʵҲ���Բ���
    ������SPI2ģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25X16/24L01/JF24C 
***************************************************************************/
void SPI2_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;								//RESET
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft ;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  SPI_Cmd(SPI1, ENABLE);
	
	PAout(4)=0;
 	SPI1s_ReadWriteByte(0xaa);//��������	
	PAout(4)=1;
}




/***************************************************************************
    - ����������STM32f103 SPI��д�ֽں���
    - ����ģ�飺STM32 SPI����
  - �������ԣ��ⲿ��ʹ�û�ʹ��
  - ����˵����TxData:Ҫд����ֽ�
  - ����˵������ȡ�����ֽ� 
 - ����˵������������SPIͨ��ʱ���ڷ��ͺͽ���ʱ��ͬʱ���е�,����������һ���ֽڵ����ݺ�ҲӦ�����ܵ�һ���ֽڵ�����
   ��1��stm32�ȵȴ��ѷ��͵������Ƿ�����ɣ����û�з�����ɣ����ҽ���ѭ��200�Σ����ʾ���ʹ��󣬷����յ���ֵΪ0;
   ��2�����������ɣ�stm32��SPI1���߷���TxData
   ��3��stm32�ٵȴ����յ������Ƿ������ɣ����û�н�����ɣ����ҽ���ѭ��200�Σ����ʾ���մ����򷵻�ֵ0
   ��4�������������ˣ��򷵻�STm32��ȡ�����µ�����  
   
   stm32
   ------->�ȴ��ѷ��͵������Ƿ����
   OK
   ------->  
   stm32��������
   ------->�ȴ������յ������Ƿ����
   OK
   ------->
   stm32��ȡ����

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
***************************************************************************/

u8 SPI1s_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
 		retry++;
 		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI1, TxData); 																//ͨ������SPIx����һ������
// 	PBout(10)=!PBout(10);
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
 		retry++;
 		if(retry>200)return 0;
	}	
	return SPI_I2S_ReceiveData(SPI1); 															//����ͨ��SPIx������յ�����			
}


//******************************************************************/
//��    �ܣ���RC522�Ĵ���
//����˵����Address[IN]:�Ĵ�����ַ
//��    �أ�������ֵ
//******************************************************************/
unsigned char ReadRawRC(unsigned char Address)
{
		u8 i=0;
		//delay_us(10);
	  PAout(4)=0;
    SPI1s_ReadWriteByte(((Address<<1)&0x7E)|0x80);
		i=SPI1s_ReadWriteByte(0);	
		SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_TXE);	
		//delay_us((u16)10);
		PAout(4)=1;			
		return i;	
}

//******************************************************************/
//��    �ܣ�дRC522�Ĵ���
//����˵����Address[IN]:�Ĵ�����ַ
//          value[IN]:д���ֵ
//******************************************************************/
void WriteRawRC(unsigned char Address, unsigned char value)
{
	PAout(4)=0;	
	//delay_us(10);
	SPI1s_ReadWriteByte((Address<<1)&0x7E);
	//delay_us(10);
	SPI1s_ReadWriteByte(value);
	//delay_us(10);

	SPI_I2S_ClearFlag(SPI1,SPI_I2S_FLAG_TXE);	
	//delay_us((u16)10);
	PAout(4)=1;				
}

//******************************************************************/
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
//******************************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
  char tmp = 0x0            ;
  tmp = ReadRawRC(reg)| mask;
  WriteRawRC(reg,tmp | mask);  // set bit mask
}

//******************************************************************/
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
//******************************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
  char tmp = 0x0              ;
  tmp = ReadRawRC(reg)&(~mask);
  WriteRawRC(reg, tmp)        ;  // clear bit mask
} 

//******************************************************************/
//��    �ܣ���λRC522
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char PcdReset()
{
  RC522_RST=1;                             ;
  delay_ns(1000)                             ;
  RC522_RST=0;                             ;
  delay_ns(1000)                             ;
  RC522_RST=1;                             ;
  delay_ns(1000)                           ;
  WriteRawRC(CommandReg,PCD_RESETPHASE);
  delay_ns(1000)                             ;
  WriteRawRC(ModeReg,0x3D)             ;//���巢�ͺͽ��ճ���ģʽ ��Mifare��ͨѶ��CRC��ʼֵ0x6363
  WriteRawRC(TReloadRegL,30)           ;//16λ��ʱ����λ 30
  WriteRawRC(TReloadRegH,0)            ;//16λ��ʱ����λ
  WriteRawRC(TModeReg,0x8D)            ;//�����ڲ���ʱ��������
  WriteRawRC(TPrescalerReg,0x3E)       ;//���ö�ʱ����Ƶϵ��  
  WriteRawRC(TxASKReg,0x40)            ;//���Ʒ����ź�Ϊ100%ASK
  return MI_OK                         ; 
}
////////////////////////////////////////////////////////////////////// 
//����RC522�Ĺ�����ʽ   
//////////////////////////////////////////////////////////////////////
char MF522PcdConfigISOType(unsigned char  type)
{
	if (type == 'A')	//ISO14443_A
		{
			ClearBitMask(Status2Reg,0x08);	//״̬2�Ĵ���         
			WriteRawRC(ModeReg,0x3D);	//3F  //��Mifare��ͨѶ��CRC��ʼֵ0x6363         
			WriteRawRC(RxSelReg,0x86);	//84   ѡ���ڲ����������ã��ڲ�ģ�ⲿ�ֵ����źţ��������ݺ��ӳ�6��λʱ�ӣ�����         
			WriteRawRC(RFCfgReg,0x7F);	//4F  ���ý�����  48dB�������         
			WriteRawRC(TReloadRegL,30);	//tmoLength);TReloadVal = 'h6a =tmoLength(dec)      
			WriteRawRC(TReloadRegH,0);	//ʵ��ֵ��OXD3E �ⲿ����Ҫ�����ö�ʱ���Ĵ���       
			WriteRawRC(TModeReg,0x8D);
			WriteRawRC(TPrescalerReg,0x3E);
			delay_ns(1000);
			PcdAntennaOn();
		}     
	else {return 0xFE;}  
	return MI_OK; 
}  

//******************************************************************/
//�������߷���  
//ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
//******************************************************************/
unsigned char PcdAntennaOn(void)
{
  unsigned char i;
  WriteRawRC(TxASKReg,0x40)       ;
  delay_us((u16)10)                       ;
  i = ReadRawRC(TxControlReg)     ;
  if(!(i&0x03))
    SetBitMask(TxControlReg, 0x03);
  i = ReadRawRC(TxASKReg);
	return i;
}


//******************************************************************/
//�ر����߷���
//******************************************************************/
void PcdAntennaOff()
{
  ClearBitMask(TxControlReg, 0x03);
}

//******************************************************************/
//��    �ܣ�ͨ��RC522��ISO14443��ͨѶ
//����˵����Command[IN]:RC522������
//          pInData[IN]:ͨ��RC522���͵���Ƭ������
//          InLenByte[IN]:�������ݵ��ֽڳ���
//          pOutData[OUT]:���յ��Ŀ�Ƭ��������
//          *pOutLenBit[OUT]:�������ݵ�λ����
//******************************************************************/
char PcdComMF522(unsigned char Command  ,unsigned char *pInData , 
                 unsigned char InLenByte,unsigned char *pOutData, 
                 unsigned int  *pOutLenBit                       )
{
  char status = MI_ERR                          ;
  unsigned char irqEn   = 0x00                  ;
  unsigned char waitFor = 0x00                  ;
  unsigned char lastBits                        ;
  unsigned char n                               ;
  unsigned int  i                               ;
  switch (Command)
  {
    case PCD_AUTHENT:
      irqEn   = 0x12                            ;
      waitFor = 0x10                            ;
      break                                     ;
    case PCD_TRANSCEIVE:
      irqEn   = 0x77                            ;
      waitFor = 0x30                            ;
      break                                     ;
    default:
      break                                     ;
  }
  WriteRawRC(ComIEnReg,irqEn|0x80)              ; //
  ClearBitMask(ComIrqReg,0x80)                  ;
  WriteRawRC(CommandReg,PCD_IDLE)               ;
  SetBitMask(FIFOLevelReg,0x80)                 ; // ���FIFO 
  for(i=0; i<InLenByte; i++)
    WriteRawRC(FIFODataReg,pInData[i])          ; // ����д��FIFO 
  WriteRawRC(CommandReg, Command)               ; // ����д������Ĵ���
  if(Command == PCD_TRANSCEIVE)
    SetBitMask(BitFramingReg,0x80)              ; // ��ʼ����     
  i = 6000                                      ; //����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
  do 
  {
    n = ReadRawRC(ComIrqReg)                    ;
    i--                                         ;
  }
  while((i!=0)&&!(n&0x01)&&!(n&waitFor))        ;
  ClearBitMask(BitFramingReg,0x80)              ;
  if(i!=0)
  {
    if(!(ReadRawRC(ErrorReg)&0x1B))
    {
      status = MI_OK                            ;
      if (n&irqEn&0x01)
        status = MI_NOTAGERR                    ;
      if(Command==PCD_TRANSCEIVE)
      {
        n = ReadRawRC(FIFOLevelReg)             ;
        lastBits = ReadRawRC(ControlReg)&0x07   ;
        if(lastBits)
          *pOutLenBit = (n-1)*8 + lastBits      ;
        else
          *pOutLenBit = n*8                     ;
        if(n==0)
          n = 1                                 ;
        if(n>MAXRLEN)
          n = MAXRLEN                           ;
        for (i=0; i<n; i++)
          pOutData[i] = ReadRawRC(FIFODataReg)  ; 
      }
    }
    else
      status = MI_ERR                           ;        
  }
  SetBitMask(ControlReg,0x80)                   ;// stop timer now
  WriteRawRC(CommandReg,PCD_IDLE)               ; 
  return status;
}

//******************************************************************/
//��    �ܣ�Ѱ��                                                    /
//����˵��: req_code[IN]:Ѱ����ʽ                                   /
//                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�           /
//                0x26 = Ѱδ��������״̬�Ŀ�                       /
//                pTagType[OUT]����Ƭ���ʹ���                       /
//                0x4400 = Mifare_UltraLight                        /
//                0x0400 = Mifare_One(S50)                          /
//                0x0200 = Mifare_One(S70)                          /
//                0x0800 = Mifare_Pro(X)                            /
//                0x4403 = Mifare_DESFire                           /
//��    ��: �ɹ�����MI_OK                                           /
//******************************************************************/
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
  char status                                        ;  
  unsigned int  unLen                                ;
  unsigned char ucComMF522Buf[MAXRLEN]               ; 

  ClearBitMask(Status2Reg,0x08)                      ;//���MRCrypto1on��Ҫ���������
  WriteRawRC(BitFramingReg,0x07)                     ;//startsend=0,rxalign=0,��FIFO�д�ŵ�λ�ã�TXlastbit=7
  SetBitMask(TxControlReg,0x03)                      ;//TX2rfen=1,TX1RFen=1,���ݵ��Ƶ�13.56MHZ���ز��ź�
 
  ucComMF522Buf[0] = req_code                        ;

  status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf,1, ucComMF522Buf, &unLen);
  if ((status == MI_OK) && (unLen == 0x10))
  {    
    *pTagType     = ucComMF522Buf[0]                 ;
    *(pTagType+1) = ucComMF522Buf[1]                 ;
  }
  else
    status = MI_ERR                                  ;
  return status                                      ;
}

//******************************************************************/
//��    �ܣ�����ײ                                                  /
//����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�                             /
//��    ��: �ɹ�����MI_OK                                           /
//******************************************************************/
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ClearBitMask(Status2Reg,0x08);//���MRCrypto1on��Ҫ���������
    WriteRawRC(BitFramingReg,0x00);//��ʾ���һ���ֽ�����λ������
    ClearBitMask(CollReg,0x80);//CollRegCollReg0��ͻ�������ͻλ������
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);//CollRegCollReg ��106kbps���õķ���ͻ����¸�λ��1
    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�ѡ����Ƭ
//����˵��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);//����MFcryon

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

//******************************************************************/
//��    �ܣ���֤��Ƭ����
//����˵��: auth_mode[IN]: ������֤ģʽ
//                 0x60 = ��֤A��Կ
//                 0x61 = ��֤B��Կ 
//          addr[IN]�����ַ
//          pKey[IN]������
//          pSnr[IN]����Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char PcdAuthState(unsigned char auth_mode,unsigned char addr,
                  unsigned char *pKey,unsigned char *pSnr    )
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;//��֤A��Կ
    ucComMF522Buf[1] = addr;//addr[IN]�����ַ
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

//******************************************************************/
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          pData[OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status                                          ;
    unsigned int  unLen                                  ;
    unsigned char i,ucComMF522Buf[MAXRLEN]               ; 

    ucComMF522Buf[0] = PICC_READ                         ;
    ucComMF522Buf[1] = addr                              ;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2])       ;   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,
                         ucComMF522Buf,&unLen           );
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
            *(pData+i) = ucComMF522Buf[i];   
    }
    else
      status = MI_ERR;       
    return status;
}

//******************************************************************/
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          pData[OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char Read_Block(unsigned char Block,unsigned char *Buf)
{
  char result                                             ;
  result = PcdAuthState(0x60,Block,RFID_Password_Buffer,RFID_UID)   ;
  if(result!=MI_OK)
    return result                                         ;
  result = PcdRead(Block,Buf)                             ;
//  return result; // 2011.01.03
  
  if(result!=MI_OK)     return   result                   ;
  if(Block!=0x00&&des_on)
  {
//     Des_Decrypt((char *)Buf    ,KK,(char *)Buf    )       ;
//     Des_Decrypt((char *)&Buf[8],KK,(char *)&Buf[8])       ;  
  }
  return SUCCESS                                          ; 
}

//******************************************************************/
//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          pData[IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char PcdWrite(unsigned char addr,unsigned char *pData)
{
  char status                                             ;
  unsigned int  unLen                                     ;
  unsigned char i,ucComMF522Buf[MAXRLEN]                  ; 
    
  ucComMF522Buf[0] = PICC_WRITE                           ;
  ucComMF522Buf[1] = addr                                 ;
  CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2])          ;
  status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,
                       ucComMF522Buf,&unLen          )    ;
  if(  ( status != MI_OK)||(unLen != 4)
     ||((ucComMF522Buf[0]&0x0F)!= 0x0A))
    status = MI_ERR                                       ;           
  if (status == MI_OK)
  {
    for (i=0; i<16; i++)
      ucComMF522Buf[i] = *(pData+i)                       ;  
    CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16])      ;
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,
                         18,ucComMF522Buf,&unLen     )    ;
    if(  (status != MI_OK)||(unLen != 4 )
       ||((ucComMF522Buf[0]&0x0F)!= 0x0A))
      status = MI_ERR                                     ;   
  }
  return status                                           ;
}
//******************************************************************/
//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          pData[IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/

char Write_Block(unsigned char Block)
{
  char result                                             ;
  if(des_on)
  {
//     Des_Encrypt((char *)RF_Buffer    ,KK,   (char *)RF_Buffer )                ;// for debug
//     Des_Encrypt((char *)&RF_Buffer[8],KK,   (char *)&RF_Buffer[8]  )                ;// for debug  
  }
  result = PcdAuthState(0x60,Block,RFID_Password_Buffer,RFID_UID)   ;
  if(result!=MI_OK)
    return result                                         ;  
  result = PcdWrite(Block,RFID_Buffer)                      ;
  return result                                           ;  
}

//******************************************************************/
//��    �ܣ��ۿ�ͳ�ֵ
//����˵��: dd_mode[IN]��������
//               0xC0 = �ۿ�
//               0xC1 = ��ֵ
//          addr[IN]��Ǯ����ַ
//          pValue[IN]��4�ֽ���(��)ֵ����λ��ǰ
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue)
{
    u8 status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pValue+i);   }
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    return status;
}

//******************************************************************/
//��    �ܣ�����Ǯ��
//����˵��: sourceaddr[IN]��Դ��ַ
//          goaladdr[IN]��Ŀ���ַ
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr)
{
    u8 status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status != MI_OK)
    {    return MI_ERR;   }
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }

    return status;
}


//******************************************************************/
//��    �ܣ����Ƭ��������״̬
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char PcdHalt(void)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return status;
}

//******************************************************************/
//��    �ܣ����Ƭ��������״̬
//��    ��: �ɹ�����MI_OK
//******************************************************************/
char MIF_Halt(void)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return status ;  
}



//******************************************************************/
//��MF522����CRC16����
//******************************************************************/
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);//ȡ����ǰ����
    SetBitMask(FIFOLevelReg,0x80);//FlushBuffer ���ErrReg �ı�־λ
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));//��CRCIRq�������ݱ�������ϸ�λ��λ
    pOutData[0] = ReadRawRC(CRCResultRegL);//��ʾ���������CRCֵ
    pOutData[1] = ReadRawRC(CRCResultRegM);
}
