#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"


//�̵���0
//����������  PA15��JTAG�� Ҫ�ص�JTAG����
#define BEEP_GPIO_RCC           RCC_APB2Periph_GPIOA
#define BEEP_GPIO_PORT          GPIOA
#define BEEP_GPIO_PIN      			GPIO_Pin_15

//����������
//#define 	BEEP_ON 							eg_beep_switch(1);
//#define 	BEEP_OFF 							eg_beep_switch(0);
#define 	BEEP_ON 				GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
#define 	BEEP_OFF 				GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);

void eg_beep_GPIO_config(void);
//void eg_beep_switch(char on);
char eg_beep_status(void);



#endif
