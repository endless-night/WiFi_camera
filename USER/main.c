#include "main.h"
/*******ov2640��������*******
    OV_D0-7   ��   PD8-15   
    OV_SCL    ��   PB6
    OV_SDA    ��   PB7
 �� OV_VSYNC  ��   PC7
    OV_PWDN   ��   PC6
    OV_RST    ��   PC13
 �� OV_HREF   ��   PC0
    OV_PCLK   ��   PC1

****************************/

extern u8* ov2640_framebuf;				//֡����
extern volatile u8 check,sending,taking_photo;

u8 key0_state=0,key1_state=0;

int main(void)
{	
    u8 i,key;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
    delay_init();	    	 //��ʱ������ʼ��	
    LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ� 
    
//    WWDG_Init(0X7F,0X5F,WWDG_Prescaler_8);   
    mem_init();
    uart_init(115200);
 //   KEY_Init();
 //   WiFi_RST_INIT();    
    
    LED1=0;
    PC_Usart("LED1\r\n");

    ov2640_framebuf=mymalloc(JPEGSIZE); 
    if(!ov2640_framebuf)	//�ڴ�������
 	{	  
        LED1=0;        
        PC_Usart("Overflow!\r\n");
        while(1);
        
				  
	}   
    

    if(OV2640_Init())
    {
        LED1=0;
        PC_Usart("OV2640 init fail : %d\r\n",OV2640_Init());
        while(1);
        
    }
    for(i=0;i<10;i++)
        delay_ms(1000);
    TIM3_Int_Init(1999,7199);//200ms
    ESP8266_init();
    
    PC_Usart("Initation ok\r\n");
    delay_ms(1000);
    ConnectToServer();
    while(1)
    {

        if(check==1)
        {
            key=KEY_Scan();
         //   PC_Usart("key:%d\r\n",key);
            key0_state <<= 1;
            key1_state <<= 1;
            if(key==KEY0_PRES)
            {
                key0_state |= 1;
                PC_Usart("KEY0_PRES\r\n");
            }
            else
            {
                key0_state |= 0;
            }

            if(key==KEY1_PRES)
            {
                key1_state |= 1;
                PC_Usart("KEY1_PRES\r\n");
                
            }
            else
            {
                key1_state |= 0;
            }
            
            if((key1_state&0x0f)==0x01)
            {
                while(KEY1==0){
                    LED1=LED0=1;
                    delay_ms(100);
                    LED1=LED0=0;
                    delay_ms(100);
                    LED1=LED0=1;
                    delay_ms(1000);
                }
                PC_Usart("taking_photo!\r\n");
                taking_photo=1;
                LED1=0;
                while(taking_photo)	
                    ov2640_jpg_photo();
                
                            
            }
            
            if((key0_state&0x0f)==0x0f){//������4s
                PC_Usart("start airkiss...\r\n");
                TIM_Cmd(TIM3, DISABLE);
                
                LED1=LED0=1;
                delay_ms(200);
                LED1=LED0=0;
                delay_ms(100);
                LED1=1;
                
                strEsp8266_Fram_Record.InfBit.FramLength=0;
                ////�˳�͸��ģʽ
                do{
                    ESP8266_SendString ( ENABLE, "+++", 0, Single_ID);
                    delay_ms(1000);
                    ESP8266_Cmd ( "AT+CIPMODE=0", "OK", 0, 800 );
                }while( !ESP8266_Cmd ( "AT+CIPMODE=0", "OK", 0, 800 ) );            //0,��͸����1��͸��
                
                LED0=0;
                
                while(!ESP8266_Cmd ( "AT+CWSMARTSTART=2", "OK", 0, 2000 ));//����airkissģʽ
                delay_ms(1000);
                while(ESP8266_Cmd ( "AT+CIFSR", "OK", NULL, 1000 )){
                    PC_Usart("%s\r\n",strEsp8266_Fram_Record .Data_RX_BUF);
                    if(!strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "0.0.0.0" )&&strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STAIP" ))
                        break;
                    delay_ms(1000);
                    delay_ms(1000);
                    
                }
                PC_Usart("airkiss ok...\r\n");
                
                LED0=1;
                ConnectToServer();
                TIM_Cmd(TIM3, ENABLE);  
            }
            
     
            if(sending==0&&taking_photo==0)
            {
                PostToWeb_cmd();
                
            }
            check=0;
        
        }
    } 
}   


