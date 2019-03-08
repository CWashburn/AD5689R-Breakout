
/*  AD5689R Demo 
 *  Written by CWashburn 3/19
 * 
 * 
 * If you like what you see, visit my Tindie Store 
 * for more awesome boards from Hyperbolic Labs!
 * 
 * https://www.tindie.com/stores/Hyperbolic_Designs/
 * 
 * 
 * Pin Connections for 3-Wire SPI config:
 * 
 * AD5689R  |  Arduino
 * Pin      |  Pin
 * -----------------------
 * 
 * VDD      |  5V
 * GND      |  GND
 * SDO      |  N/C
 * VREF     |  N/C
 * LDAC     |  GND
 * SEL      |  GND
 * GAIN     |  GND
 * RST      |  5V
 * VIO      |  5V
 * SDIN     |  D11
 * SCLK     |  D13
 * SYNC     |  D10
 *  
 */



#include <SPI.h>

#define SYNC_PIN  10
#define RST_PIN   -1
#define REF_INT   0 
#define REF_EXT   1
#define DAC1      0
#define DAC2      1

#define CLK_FREQ     1000000




SPISettings SPI_CFG = SPISettings(CLK_FREQ, MSBFIRST, SPI_MODE1);




void setup(){

  pinMode(SYNC_PIN, OUTPUT);
  pinMode(RST_PIN,  OUTPUT);
  SPI.begin();

  resetDAC();
  setDACRefSource(REF_INT);
}


void loop(){


  for(float i = 0.0; i < 2.5; i += 0.001){
    setDACVoltage(DAC1, i);
    setDACVoltage(DAC2, 2.5-i);
    delay(1);
  } 
  for(float i = 2.5; i > 0.0; i -= 0.001){
    setDACVoltage(DAC1, i);
    setDACVoltage(DAC2, 2.5-i);
    delay(1);
  }
}


void setDACVoltage(bool channel, float voltage){

  voltage = constrain(voltage, 0.0, 2.5);
  uint16_t code = (uint16_t)(65535.0*voltage/2.5);

  SPI.beginTransaction(SPI_CFG);
  digitalWrite(SYNC_PIN, LOW);
  
  if(channel == 0)
    SPI.transfer(0x11);  
  else
    SPI.transfer(0x18);
    
  SPI.transfer(code >> 8);
  SPI.transfer(code);
  digitalWrite(SYNC_PIN, HIGH);
  SPI.endTransaction();
}





void setDACRefSource(uint8_t src){  

    SPI.beginTransaction(SPI_CFG);
    digitalWrite(SYNC_PIN, LOW);
    SPI.transfer(0b1110000);
    SPI.transfer(0x00);
    SPI.transfer((uint8_t)(src));
    digitalWrite(SYNC_PIN, HIGH);
    SPI.endTransaction();
}



void resetDAC(){
  
  if(RST_PIN < 0){
    SPI.beginTransaction(SPI_CFG);
    digitalWrite(SYNC_PIN, LOW);
    SPI.transfer(0x60);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    digitalWrite(SYNC_PIN, HIGH);
    SPI.endTransaction();
  }
  else{
    digitalWrite(RST_PIN, LOW);
    delayMicroseconds(1);
    digitalWrite(RST_PIN, HIGH);
  }    
}

