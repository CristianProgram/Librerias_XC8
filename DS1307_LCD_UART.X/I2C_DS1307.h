#include "I2C_DS1307.h"
void DS1307(void) {
        Hours=leer_DS1307(Reg_Read_DS1307,Reg_Hours);
        Hours=BCD2Dec(Hours);
        Minutes=leer_DS1307(Reg_Read_DS1307,Reg_Minutes);
        Minutes=BCD2Dec(Minutes);
        Seconds=leer_DS1307(Reg_Read_DS1307,Reg_Seconds);
        Seconds=BCD2Dec(Seconds);
        
        Month=leer_DS1307(Reg_Read_DS1307,Reg_Month);
        Month=BCD2Dec(Month);
        Date=leer_DS1307(Reg_Read_DS1307,Reg_Date);
        Date=BCD2Dec(Date);
        Year=leer_DS1307(Reg_Read_DS1307,Reg_Year);
        Year=BCD2Dec(Year);
                
        Day=leer_DS1307(Reg_Read_DS1307,Reg_Day);
        Day=BCD2Dec(Day);
        
        if(Hours==0){
            meridiano = " AM";
            Hours=12;    
        }
        if(Hours>12){
            Hours=Hours-12;
            meridiano = " PM";
        }
        else{
            meridiano = " AM"; 
        }
        
        txt_DS1307[0] = (Hours/10)%10 + 48;
        txt_DS1307[1] = (Hours/1)%10 + 48;
        txt_DS1307[2] = ':';
        txt_DS1307[3] = (Minutes/10)%10 + 48;
        txt_DS1307[4] = (Minutes/1)%10 + 48; 
        txt_DS1307[5] = ':';
        txt_DS1307[6] = (Seconds/10)%10 + 48;
        txt_DS1307[7] = (Seconds/1)%10 + 48;      
        txt_DS1307[8] = 0x00;
        txt_DS1307[9] = 0x00;   
        putsUSART(txt_DS1307);
        putsUSART(meridiano);
        putsUSART("\r\n");
        Lcd_Out2(1,0,txt_DS1307);
        Lcd_Out2(1,10,meridiano);
                
        txt_DS1307[0] = (Date/10)%10 + 48;
        txt_DS1307[1] = (Date/1)%10 + 48; 
        txt_DS1307[2] = '/';
        txt_DS1307[3] = (Month/10)%10 + 48;
        txt_DS1307[4] = (Month/1)%10 + 48;
        txt_DS1307[5] = '/';
        txt_DS1307[6] = '2';
        txt_DS1307[7] = '0'; 
        txt_DS1307[8] = (Year/10)%10 + 48;
        txt_DS1307[9] = (Year/1)%10 + 48;          
        putsUSART(txt_DS1307);
        dia_semana(Day);
        putsUSART("\r\n");  
        Lcd_Out2(2,0,txt_DS1307);     
        
}
void escribir_DS1307(unsigned short dirrecion_esclavo,
                     unsigned short dirrecion_memoria,
                     unsigned short dato){
    StartI2C();
    IdleI2C();
    WriteI2C(dirrecion_esclavo);
    IdleI2C();    
    WriteI2C(dirrecion_memoria);
    IdleI2C();    
    WriteI2C(dato);
    StopI2C();
}

unsigned short leer_DS1307(unsigned short dirrecion_esclavo,
                           unsigned short dirrecion_memoria){
    
  char Data_DS1307;
  StartI2C();
  IdleI2C();
  WriteI2C(dirrecion_esclavo-1);
  IdleI2C();
  WriteI2C(dirrecion_memoria);
  IdleI2C();
  StopI2C();

  RestartI2C();
  IdleI2C();
  WriteI2C(dirrecion_esclavo);
  IdleI2C();
  Data_DS1307=ReadI2C();
  IdleI2C();
  NotAckI2C();
  IdleI2C();
  StopI2C();
  return Data_DS1307;
}

void dia_semana(unsigned short dia_semana) {
  switch (dia_semana) {
    case 1:
    putsUSART(" DOMINGO");
    Lcd_Out2(2,11,"DOM");
    break;
    case 2:
    putsUSART(" LUNES");
    Lcd_Out2(2,11,"lUN");
    break;
    case 3:
    putsUSART(" MARTES");
    Lcd_Out2(2,11,"MAR");
    break;
    case 4:
    putsUSART(" MIERCOLES");
    Lcd_Out2(2,11,"MIE");
    break;
    case 5:
    putsUSART(" JUEVES");
    Lcd_Out2(2,11,"JUE");
    break;
    case 6:
    putsUSART(" VIERNES");
    Lcd_Out2(2,11,"VIE");
    break;
    case 7:
    putsUSART(" SABADO");
    Lcd_Out2(2,11,"SAB");
    break;
  }
}

void set_time_DS1307 (unsigned short set_segundos, unsigned short set_minutos,
                      unsigned short set_horas, unsigned short set_dias,
                      unsigned short set_date, unsigned short set_mes,
                      unsigned short set_anno){

  escribir_DS1307(Reg_Write_DS1307,Reg_Seconds,Dec2BCD(set_segundos));
  escribir_DS1307(Reg_Write_DS1307,Reg_Minutes,Dec2BCD(set_minutos));
  escribir_DS1307(Reg_Write_DS1307,Reg_Hours,Dec2BCD(set_horas));
  escribir_DS1307(Reg_Write_DS1307,Reg_Day,Dec2BCD(set_dias));
  escribir_DS1307(Reg_Write_DS1307,Reg_Date,Dec2BCD(set_date));
  escribir_DS1307(Reg_Write_DS1307,Reg_Month,Dec2BCD(set_mes));
  escribir_DS1307(Reg_Write_DS1307,Reg_Year,Dec2BCD(set_anno));
}

unsigned short BCD2Dec(short Temp_3){
     short Temp_1,Temp_2;
    Temp_2 = Temp_3 & 0x0F;
    Temp_1 = Temp_2;
    Temp_3 = 0xF0 & Temp_3;
    Temp_2 = Temp_3 >> 4;
    Temp_2 = 0x0F & Temp_2;
    Temp_1 = Temp_2*10 + Temp_1;
    return Temp_1;
}

unsigned short Dec2BCD(short Temp_3){
     short Temp_1, Temp_2;
    Temp_1 = Temp_3%10;
    Temp_1 = Temp_1 & 0x0F;
    Temp_3 = Temp_3/10;
    Temp_2 = Temp_3%10;
    Temp_2 = 0x0F & Temp_2;
    Temp_2 = Temp_2 << 4;
    Temp_2 = 0xF0 & Temp_2;
    Temp_1 = Temp_1 | Temp_2; return Temp_1;
}