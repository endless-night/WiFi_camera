#ifndef __USART_H
#define __USART_H
#include "sys.h" 
#include "wifi_config.h"
#include <stdio.h>
#include <stdarg.h>

#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
void uart_init(u32 bound);
void USART_printf( USART_TypeDef* USARTx, char *Data, ... );

#endif


