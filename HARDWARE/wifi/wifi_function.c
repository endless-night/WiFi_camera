#include "wifi_function.h"
#include "wifi_config.h"
#include "main.h"
#include "led.h"


/*
 * ��������ESP8266_Choose
 * ����  ��ʹ��/����WF-ESP8266ģ��
 * ����  ��enumChoose = ENABLE��ʹ��ģ��
 *         enumChoose = DISABLE������ģ��
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_Choose ( FunctionalState enumChoose )
{
	if ( enumChoose == ENABLE )
		ESP8266_CH_HIGH_LEVEL();
	
	else
		ESP8266_CH_LOW_LEVEL();
	
}


/*
 * ��������ESP8266_Rst
 * ����  ������WF-ESP8266ģ��
 * ����  ����
 * ����  : ��
 * ����  ����ESP8266_AT_Test����
 */
void ESP8266_Rst ( void )
{
	#if 0
	 ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
	
	#else
//	 ESP8266_RST_LOW_LEVEL();
//	 delay_ms ( 500 ); 
//	 ESP8266_RST_HIGH_LEVEL();
	 
	#endif

}


/*
 * ��������ESP8266_AT_Test
 * ����  ����WF-ESP8266ģ�����AT��������
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_AT_Test ( void )
{
//	ESP8266_RST_HIGH_LEVEL();
	
	delay_ms ( 1000 ); 
	
	while ( ! ESP8266_Cmd ( "AT", "OK", NULL, 1500 ) ) 
        printf("wait for reply!\r\n"); 
    
    printf("AT ok!\r\n");

}


/*
 * ��������ESP8266_Cmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�

	ESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		return true;
    
	do{
        if(waittime>1000)
        {
            delay_ms (1000);        
            waittime-=1000;
        }else
        {
            delay_ms (waittime);
            break;
        }
        
    }while(1);
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

//	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}


/*
 * ��������ESP8266_Net_Mode_Choose
 * ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
 * ����  ��enumMode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	switch ( enumMode )
	{
		case STA:
			return ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		
	  case AP:
		  return ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
		
		case STA_AP:
		  return ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
		
	  default:
		  return false;
  }
	
}


/*
 * ��������ESP8266_JoinAP
 * ����  ��WF-ESP8266ģ�������ⲿWiFi
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Cmd ( cCmd, "OK", NULL, 7000 );
	
}


/*
 * ��������ESP8266_BuildAP
 * ����  ��WF-ESP8266ģ�鴴��WiFi�ȵ�
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 *       ��enunPsdMode��WiFi���ܷ�ʽ�����ַ���
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, char * enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%s", pSSID, pPassWord, enunPsdMode );
	
	return ESP8266_Cmd ( cCmd, "OK", 0, 1000 );
	
}


/*
 * ��������ESP8266_Enable_MultipleId
 * ����  ��WF-ESP8266ģ������������
 * ����  ��enumEnUnvarnishTx�������Ƿ������
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Cmd ( cStr, "OK", 0, 500 );
	
}


/*
 * ��������ESP8266_Link_Server
 * ����  ��WF-ESP8266ģ�������ⲿ������
 * ����  ��enumE������Э��
 *       ��ip��������IP�ַ���
 *       ��ComNum���������˿��ַ���
 *       ��id��ģ�����ӷ�������ID
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )//������
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);    ///AT+CIPSTART="TCP","ip","80"

  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );//������

	return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 500 );
	
}

/*
 * ��������ESP8266_StartOrShutServer
 * ����  ��WF-ESP8266ģ�鿪����رշ�����ģʽ
 * ����  ��enumMode������/�ر�
 *       ��pPortNum���������˿ں��ַ���
 *       ��pTimeOver����������ʱʱ���ַ�������λ����
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 500 ) &&
						 ESP8266_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd ( cCmd1, "OK", 0, 500 );
	}
	
}


/*
 * ��������ESP8266_UnvarnishSend
 * ����  ������WF-ESP8266ģ�����͸������
 * ����  ����
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_UnvarnishSend ( void )
{
	return (
	  ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) &&
	  ESP8266_Cmd ( "AT+CIPSEND", "\r\n", ">", 500 ) );
	
}


/*
 * ��������ESP8266_SendString
 * ����  ��WF-ESP8266ģ�鷢���ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 *       ��pStr��Ҫ���͵��ַ���
 *       ��ulStrLength��Ҫ���͵��ַ������ֽ���
 *       ��ucId���ĸ�ID���͵��ַ���
 * ����  : 1�����ͳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
		
	if ( enumEnUnvarnishTx )
		ESP8266_Usart ( "%s", pStr );

	
	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength);
		
		ESP8266_Cmd ( cStr, "> ", 0, 500 );
		bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 500 );
    }
	
	return bRet;

}


/*
 * ��������ESP8266_ReceiveString
 * ����  ��WF-ESP8266ģ������ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 * ����  : ���յ����ַ����׵�ַ
 * ����  �����ⲿ����
 */
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, ">" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}
	
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}

	return pRecStr;
	
}


/*
 * ��������ESP8266_WebFetch_Test
 * ����  ��WF-ESP8266ģ�����ץȡ��ҳ����
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_init( void )
{
//	ESP8266_Choose ( ENABLE );	

 

//	ESP8266_AT_Test ();
	

	ESP8266_Net_Mode_Choose ( STA );
  

	ESP8266_Cmd ( "AT+CIPMUX=0", "OK", 0, 500 );            //������ 


}
/*****************************************************************************************************/
/*�ӷ�������������*/
void Web_To_ESP(void)
{ 	
//	u16 i;
	bool state;
//	ESP8266_Cmd ( "AT", "OK", NULL, 200 );
	ESP8266_Cmd ( "AT+CIPMUX=0", "OK", 0, 500 );            //������ 
	delay_ms(1000);
	do
	{
		strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�
		memset(strEsp8266_Fram_Record .Data_RX_BUF,'\0',RX_BUF_MAX_LEN);
		ESP8266_Usart ( "%s\r\n", "AT+CIPSTART=\"TCP\",\"1.labscnu.applinzi.com\",80" );	
		delay_ms (500);                 //��ʱ	
		strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
		PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
        state= (( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "OK" )|( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "ALREAY CONNECT" ));
		if(( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "ERROR" ))
			state=false; 		
	}
	while(state==false);
    ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 );            //0,��͸����1��͸��
	ESP8266_Cmd ( "AT+CIPSEND", "\r\n", ">", 500 );
	ESP8266_SendString ( ENABLE, "GET /LibrarySeatServer/seatInfo?room=1 HTTP/1.1\r\n",     NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "Host: 192.168.191.2:8080\r\n",                            NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "User-Agent: abc\r\n",                                     NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "Connection: close\r\n",                                   NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "\r\n",                                                    NULL, Single_ID );
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	delay_ms(1200);
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	PC_Usart ("%s",strEsp8266_Fram_Record .Data_RX_BUF);
}	
///***************************************************************************************/
////��½�����ߺ�������һ��ˢ��½���ڶ���ˢ����
////������usr_Id             ��1,2,3,4,5,.......
void login(u8 usr_Id)
{
	char cStr[100]={0};
	u16 i;
	bool state;
	switch(usr_Id)
	{
		case 1:
			sprintf ( cStr, "GET /LibrarySeatServer/usrInfo?%s HTTP/1.1\r\n", "usr=june&password=19920211&req_type=SWIPE" );//�û�june
			break;
		case 2:
			sprintf ( cStr, "GET /LibrarySeatServer/usrInfo?%s HTTP/1.1\r\n", "usr=solar&password=19920211&req_type=SWIPE" );//�û�solar
			break;
		default:
			break;
	}
	
	ESP8266_Cmd ( "AT+CIPMUX=0", "OK", 0, 500 );//������
	delay_ms(500);
	do
	{
		strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�
		for(i=0;i<1024;i++)
			strEsp8266_Fram_Record .Data_RX_BUF[i]=0;                //�������
		ESP8266_Usart ( "%s\r\n", "AT+CIPSTART=\"TCP\",\"192.168.191.3\",80");		
		delay_ms ( 1000);                 //��ʱ	
		strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
		PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
        state= ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "OK" )||( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "ALREAY CONNECT" );
		if(( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "ERROR" ))
			state=false; 
	
	}
	while(state==false);
    ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 );            //0,��͸����1��͸��
	ESP8266_Cmd ( "AT+CIPSEND", "\r\n", ">", 500 );
	ESP8266_SendString ( ENABLE, cStr,                                NULL, Single_ID );//������
	ESP8266_SendString ( ENABLE, "Host: 192.168.191.3:8080\r\n",      NULL, Single_ID );//����Ϊ����ͷ
	ESP8266_SendString ( ENABLE, "User-Agent: abc\r\n",               NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "Content-Length: 38\r\n",            NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "Connection: close\r\n",             NULL, Single_ID );
	ESP8266_SendString ( ENABLE, "\r\n",                              NULL, Single_ID );
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	delay_ms(3000);
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	PC_Usart ("%s",strEsp8266_Fram_Record .Data_RX_BUF);
	PC_Usart ("\r\n");
	if((strstr(strEsp8266_Fram_Record .Data_RX_BUF,"OK"))!=NULL)
		PC_Usart ("log in successfully\r\n");
}	

void ConnectToServer()
{
    bool state;
    
    /////���ӷ�����
    if(ESP8266_Cmd ( "AT", "OK", NULL, 1500 ))
    {
        do
        {
            strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�
            memset(strEsp8266_Fram_Record .Data_RX_BUF,'\0',RX_BUF_MAX_LEN);	
            state=ESP8266_Cmd ( "AT+CIPSTART=\"TCP\",\"1.labscnu.applinzi.com\",80", "OK", "ALREADY", 2500 );
        }
        while(state==false);
    }
    
    PC_Usart("Already Connected!\r\n");
    ////����͸��ģʽ
    
    ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 800 );            //0,��͸����1��͸��
	ESP8266_Cmd ( "AT+CIPSEND", "\r\n", ">", 500 );
 
}

volatile u8 sending=0;
extern volatile u8 taking_photo;
//post��������
void  PostToWeb_tou (u8 * photo,int len)
{
//    bool state;
    char str[30];
    int i;
    sending=1;
    
    strEsp8266_Fram_Record .InfBit .FramLength = 0;
    ////���Ϳ���
    ESP8266_SendString ( ENABLE, "POST /index.php/home/image/uploads/imageName/6-420 HTTP/1.1\r\n"
                                 "Host:1.labscnu.applinzi.com\r\n"
                                 "Content-Type:multipart/form-data;boundary=--ianshi\r\n"
                                 //"Connection:close\r\n"
                                    ,0, Single_ID );
    sprintf(str,"Content-Length:%d\r\n\r\n",len+124);
    ESP8266_SendString ( ENABLE, str,0, Single_ID );
    ESP8266_SendString ( ENABLE, "----ianshi\r\n"
                                 "Content-Disposition: form-data; name=\"6-420\"; filename=\"6-420.jpg\"\r\n"
                                 "Content-Type: image/jpeg\r\n\r\n",0, Single_ID );
//    ESP8266_SendString ( ENABLE, photo, 0, Single_ID);
    for(i=0;i<len;i++)
    {
        WiFi_Usart->DR=photo[i];	
        while((WiFi_Usart->SR&0x40)==0);
    }
    ESP8266_SendString ( ENABLE, "\r\n----ianshi--\r\n", 0, Single_ID);

    PC_Usart ("sent\r\n");
    LED1=1;

    taking_photo=0;
    sending=0;
}


void  PostToWeb_cmd(void)
{   
    char str[180]={"POST /index.php/home/image HTTP/1.1\r\n"
                "Host:1.labscnu.applinzi.com\r\n"
                "Content-Length:19\r\n"
                "Content-Type:application/json\r\n"
               // "Connection:close\r\n"
                "\r\n"
                "{\"roomNum\":\"6-420\"}"};
    sending=1;
                
    strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';    
    PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
    
    if(strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "3002"))
    {

        PC_Usart("taking_photo!\r\n");
        taking_photo=1;
        LED1=0;
        while(taking_photo)	
            ov2640_jpg_photo();
    }
    
    PC_Usart("sending quest!\r\n");
    
    strEsp8266_Fram_Record .InfBit .FramLength = 0; 
    ////����ѯ��
    ESP8266_SendString ( ENABLE, str, 157, Single_ID);

    sending=0;
    
}



