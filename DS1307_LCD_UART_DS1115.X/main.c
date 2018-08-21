/*
 * File:   main.c
 * Author: Cristian Julian Rojas Sierra
 *
 * Created on 18 de agosto de 2018, 10:08 PM
 * 
 * DS1307 y el ADS1115 se visualiza en una LCD 2X16 y a su vez en el purto serial
 * 
 */
#define _XTAL_FREQ 16000000
//**************************Librerias*******************************************
//#include <stdint.h>
#include <xc.h>
#include <stdio.h>
#include <p18f4550.h>
#include <plib/i2c.h>
#include <plib/usart.h>
#include "Configuracion_Bits.h"
#include "flex_lcd.h"
#include "I2C_DS1307.h"
#include "I2C_ADS1115.h" 
//**************************Constantes *****************************************
unsigned long const Baud_USART = 9600;      //Baudios Del Puerto Serial
unsigned long const Bit_Rate_I2C = 1000000;  //velocidad del I2C
unsigned long const spbrg = (_XTAL_FREQ/(16 * Baud_USART)) - 1;
//**************************Variables Chart*************************************
unsigned char buffer_1[20];
//**************************Variables INT***************************************
unsigned int Menu = 0;
unsigned int Salir=0;
unsigned int ADC_INT=0;
float ADC_FLOAT=0;
//**************************Declaracion de Funciones****************************
void Leer_Pulsador(void);
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
    setGain(PGA_6_114);
    setSamples(SPS_8);

    /*Configuracion De Hora    
    Salir=1;
    while(Salir==1){       
        if (PORTDbits.RD1==0){
            set_time_DS1307 (0,58,20,1,20,8,18);//Seg,Min,Hr,Sem,Dia,Mes,Año
            Salir=0;
        }
    }
   */              
//******************************************************************************
    while(1){       
       
        while(Menu==0){      
            Lcd_Out(1,0,"Cristian Julian "); 
            Lcd_Out(2,0,"Rojas Sierra    ");
            Leer_Pulsador();
        }                        
        while(Menu==1){      
            ADC_INT=readADC_SingleEnded(0);
            ADC_FLOAT=(ADC_INT*6.114)/32767.0;         
            sprintf(buffer_1,"Volt 1 = %0.3fv        ",ADC_FLOAT);                         
            Lcd_Out2(1,0,buffer_1);
            
            ADC_INT=readADC_SingleEnded(1);
            ADC_FLOAT=(ADC_INT*6.114)/32767.0;         
            sprintf(buffer_1,"Volt 2 = %0.3fv        ",ADC_FLOAT);                         
            Lcd_Out2(2,0,buffer_1);
            Leer_Pulsador();  
        }
        while(Menu==2){   
                    
            ADC_INT=readADC_SingleEnded(2);
            ADC_FLOAT=(ADC_INT*6.114)/32767.0;         
            sprintf(buffer_1,"Volt 3 = %0.3fv        ",ADC_FLOAT);                         
            Lcd_Out2(1,0,buffer_1);
            
            ADC_INT=readADC_SingleEnded(3);
            ADC_FLOAT=(ADC_INT*6.114)/32767.0;         
            sprintf(buffer_1,"Volt 4 = %0.3fv       ",ADC_FLOAT);                         
            Lcd_Out2(2,0,buffer_1);
            Leer_Pulsador();    
        }
        while(Menu==3){      
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
                if (Menu==4){
                        Menu=1;
                }
                Lcd_Cmd(LCD_CLEAR);
            }
        }
    }    
}

