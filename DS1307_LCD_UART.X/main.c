 /*
 * File:   main.c
 * Author: Cristian Julian Rojas Sierra
 *
 * Created on 13 de agosto de 2018, 10:18 PM
 * 
 * DS1307 se visualiza en una lcd y a su vez en un puerto serial
 * 
 */
#define _XTAL_FREQ 16000000
//**************************Constantes******************************************
unsigned short const Reg_Write_DS1307 = 0xD0;      //  -
unsigned short const Reg_Read_DS1307 = 0xD1;       //  -
unsigned short const Reg_Seconds = 0x00;    //00-59
unsigned short const Reg_Minutes = 0x01;    //00-59
unsigned short const Reg_Hours = 0x02;      //1-12 +AM/PM 00-23
unsigned short const Reg_Day = 0x03;        //01-07
unsigned short const Reg_Date = 0x04;       //01-31
unsigned short const Reg_Month = 0x05;      //01-12
unsigned short const Reg_Year = 0x06;       //00-99
unsigned short const Reg_control = 0x07;    //  - 
unsigned short const Reg_control = 0x07;    //  - 
unsigned long const Baud_USART = 9600;      //Baudios Del Puerto Serial
unsigned long const Bit_Rate_I2C = 100000;  //velocidad del I2C
unsigned long const spbrg = (_XTAL_FREQ/(16 * Baud_USART)) - 1;
//**************************Variables Chart*************************************
unsigned char txt_DS1307[12];
unsigned char* meridiano;
//**************************Variables INT***************************************
unsigned int Menu = 0;
unsigned int Salir=0;
//**************************Variables short*************************************
unsigned short Seconds = 0;
unsigned short Minutes = 0;
unsigned short Hours = 0;
unsigned short Day = 0;
unsigned short Date = 0;
unsigned short Month = 0;
unsigned short Year = 0;
//**************************Declaracion de Funciones****************************
void DS1307(void);
void Leer_Pulsador(void);
unsigned short BCD2Dec(short Temp_3);
unsigned short Dec2BCD(short Temp_3);
void dia_semana(unsigned short dia_semana);
void set_time_DS1307 (unsigned short set_segundos, unsigned short set_minutos,
                      unsigned short set_horas, unsigned short set_dias,
                      unsigned short set_date, unsigned short set_mes,
                      unsigned short set_anno);
void escribir_DS1307(unsigned short dirrecion_esclavo,
                     unsigned short dirrecion_memoria,
                     unsigned short dato);

unsigned short leer_DS1307(unsigned short dirrecion_esclavo,
                           unsigned short dirrecion_memoria);
//**************************Librerias*******************************************
#include <xc.h>
#include "Configuracion_Bits.h"
#include <stdlib.h>
#include <stdio.h>
#include <p18f4550.h>
#include <plib/i2c.h>
#include <plib/usart.h>
#include "flex_lcd.h"
#include "I2C_DS1307.h"
#include "I2C_ADS1115"
//******************************************************************************
void main(void) {
//**************************Setup***********************************************
    CMCON = 0x07;
    ADCON1 = 0xFF;
 TRISDbits.RD1=1;

    OpenI2C(MASTER, SLEW_OFF);// Initialize I2C module
    SSPADD =(_XTAL_FREQ/Bit_Rate_I2C)/4-1;      //SSPADD = (FOSC/ Bit Rate) /4 - 1
    IdleI2C();
    StartI2C();  
    
    OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF &
               USART_ASYNCH_MODE & USART_EIGHT_BIT &
               USART_CONT_RX & USART_BRGH_HIGH &
               USART_ADDEN_OFF,spbrg);
    
    Lcd_Init();

    Lcd_Cmd(LCD_CLEAR);
    Lcd_Cmd(LCD_CURSOR_OFF);        
    

    set_time_DS1307 (0,47,19,7,18,8,18);
     
   
//******************************************************************************
 
    while(1){       
       
        while(Menu==0){      
            Lcd_Out2(1,0,"DS1307+ADS1115"); 
            Lcd_Out2(2,0,"Cristian Julian");
            
            Leer_Pulsador();
        }                        
        while(Menu==1){      
            
            Leer_Pulsador();
        }
        while(Menu==2){      
            DS1307();
            
            Leer_Pulsador();
        }        
        
    }
}


void Leer_Pulsador(void){
    if (PORTDbits.RD1==1){
        Salir=1;
        while(Salir==1){ 
            if (PORTDbits.RD1==0){
                Menu++;
                Salir=0;
                if (Menu==3){
                        Menu=1;
                }
                Lcd_Cmd(LCD_CLEAR);
            }
        }
    }    
}

