#ifndef __12C_ADS1115_h
#define __12C_ADS1115_h 
//******************************************
//  Libreria para el ADC ADS1115           *
//                                         * 
//  By Cristian Julian Rojas Sierra        *
//******************************************

unsigned short Config_ADS1115 = 0;
unsigned short m_gain_ADS1115 = 0;
unsigned short m_Samples_ADS1115=0;
//**************************Constantes ADS1115**********************************
#define Reg_Write_ADS1115   0x90  
#define Reg_Read_ADS1115    0x91  

#define POINTER_CONVERT     0x00
#define POINTER_CONFIG      0x01
#define POINTER_LOWTHRESH   0x02
#define POINTER_HITHRESH    0x03

#define  OS_MASK            0b0000000000000000
#define  OS_SINGLE          0b1000000000000000

#define  MUX_DIFF_0_1       0b0000000000000000
#define  MUX_DIFF_0_3       0b0001000000000000
#define  MUX_DIFF_1_3       0b0010000000000000
#define  MUX_DIFF_2_3       0b0011000000000000
#define  MUX_SINGLE_0       0b0100000000000000
#define  MUX_SINGLE_1       0b0101000000000000
#define  MUX_SINGLE_2       0b0110000000000000
#define  MUX_SINGLE_3       0b0111000000000000

#define  PGA_6_114          0b0000000000000000
#define  PGA_4_096          0b0000001000000000
#define  PGA_2_048          0b0000010000000000
#define  PGA_1_024          0b0000011000000000
#define  PGA_0_512          0b0000100000000000
#define  PGA_0_256          0b0000101000000000

#define  MODE_CONTIN        0b0000000000000000
#define  MODE_SINGLE        0b0000000100000000

#define  SPS_8              0b0000000000000000
#define  SPS_16             0b0000000000100000
#define  SPS_32             0b0000000001000000
#define  SPS_64             0b0000000001100000
#define  SPS_128            0b0000000010000000
#define  SPS_250            0b0000000010100000
#define  SPS_475            0b0000000011000000
#define  SPS_860            0b0000000011100000

#define  CMODE_TRAD         0b0000000000000000
#define  CMODE_WINDOW       0b0000000000010000

#define  CPOL_ACTVLOW       0b0000000000000000
#define  CPOL_ACTVHI        0b0000000000001000

#define  CLAT_NONLAT        0b0000000000000000
#define  CLAT_LATCH         0b0000000000000100

#define  CQUE_1CONV         0b0000000000000000
#define  CQUE_2CONV         0b0000000000000001
#define  CQUE_4CONV         0b0000000000000010
#define  CQUE_NONE          0b0000000000000011
//******************************************************************************
unsigned int leer_ADS1115();
void setGain(unsigned short gain);
void setSamples(unsigned short Samples);
unsigned short readADC_SingleEnded(unsigned short channel);
void escribir_ADS1115(unsigned short dirrecion_esclavo,
                      unsigned short Registro_Puntero,
                      unsigned short Registro_Configuracion);
//******************************************************************************

unsigned int leer_ADS1115(){
    
    unsigned int Data_ADS1115_MSB=0;
    unsigned int Data_ADS1115_LSB=0;
    unsigned int ADC_RESULTADO_ADS1115=0;
    
    StartI2C();
    IdleI2C();
    WriteI2C(Reg_Write_ADS1115);
    IdleI2C();    
    WriteI2C(POINTER_CONVERT); 
    StopI2C();    
    RestartI2C();
    IdleI2C();
    WriteI2C(Reg_Read_ADS1115);
    IdleI2C();
    Data_ADS1115_MSB=ReadI2C();
    IdleI2C();
    AckI2C();
    Data_ADS1115_LSB=ReadI2C();
    IdleI2C(); 
    NotAckI2C();
    IdleI2C();
    StopI2C();
    ADC_RESULTADO_ADS1115 =(Data_ADS1115_MSB<<8)|Data_ADS1115_LSB;
    ADC_RESULTADO_ADS1115=(ADC_RESULTADO_ADS1115);
    if(ADC_RESULTADO_ADS1115>=32767){
        ADC_RESULTADO_ADS1115=0;
    }
    return ADC_RESULTADO_ADS1115;
}
 
void escribir_ADS1115(unsigned short dirrecion_esclavo,
                      unsigned short Registro_Puntero,
                      unsigned short Registro_Configuracion){
    StartI2C();
    IdleI2C();
    WriteI2C(dirrecion_esclavo);
    IdleI2C();   
    WriteI2C(Registro_Puntero);
    IdleI2C();   
    WriteI2C((Registro_Configuracion>>8));
    IdleI2C();    
    WriteI2C((Registro_Configuracion&0xFF));
    StopI2C();
}
void setGain(unsigned short gain){
    m_gain_ADS1115 = gain;
}
void setSamples(unsigned short Samples){
    m_Samples_ADS1115 = Samples;
}

unsigned short readADC_SingleEnded(unsigned short channel) {
     
    if (channel > 3){
        return 0;
    }
    
    unsigned short Config_ADS1115 = CQUE_NONE    | // Disable the comparator (default val)
                                    CLAT_NONLAT  | // Non-latching (default val)
                                    CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                                    CMODE_TRAD   | // Traditional comparator (default val)                                  
                                    MODE_SINGLE;   // Single-shot mode (default)
     
    Config_ADS1115 |= m_gain_ADS1115;  
    Config_ADS1115 |= m_Samples_ADS1115; 
    
    switch (channel){
    case (0):
      Config_ADS1115 |= MUX_SINGLE_0;
      break;
    case (1):
      Config_ADS1115 |= MUX_SINGLE_1;
      break;
    case (2):
      Config_ADS1115 |= MUX_SINGLE_2;
      break;
    case (3):
      Config_ADS1115 |= MUX_SINGLE_3;
      break;
  }
  Config_ADS1115 |= OS_SINGLE;
  escribir_ADS1115(Reg_Write_ADS1115, POINTER_CONFIG,Config_ADS1115);
  __delay_ms(8);
  return leer_ADS1115();
 }
#endif