#include "TOF10120.h"

#include <Wire.h>

//constants
const int POLLING_DELAY = 5;

const int LAZER_SENSOR_FROM_DISTANCE = 0;
const int LAZER_SENSOR_TO_DISTANCE = 1000;

//increments
volatile int pollingDelayInc = 0;

//state
volatile int distance = 1200;

//laser sensor subroutine
unsigned short lenth_val = 0;
unsigned char i2c_rx_buf[16];
unsigned char dirsend_flag=0;


void TOF10120::initSensor() {
  Wire.begin();
}

//call the method in timer subroutine
void TOF10120::clk() {
  if (pollingDelayInc < POLLING_DELAY) {
    pollingDelayInc++;
  }
}

bool TOF10120::isMovement() {
  int currentDistance = getDistance();
  return currentDistance >= LAZER_SENSOR_FROM_DISTANCE && currentDistance <= LAZER_SENSOR_TO_DISTANCE;
}

int TOF10120::getDistance() {
  if (pollingDelayInc >= POLLING_DELAY) {
    pollingDelayInc = 0;
    distance = ReadDistance();
//    Serial.print(distance);
//    Serial.println(" mm");
  }
  
  return distance;
}

int TOF10120::ReadDistance(){
    SensorRead(0x00,i2c_rx_buf,2);
    lenth_val=i2c_rx_buf[0];
    lenth_val=lenth_val<<8;
    lenth_val|=i2c_rx_buf[1];
    return lenth_val;
}

void TOF10120::SensorRead(unsigned char addr,unsigned char* datbuf,unsigned int cnt) {
  unsigned short result=0;
  // step 1: instruct sensor to read echoes
  Wire.beginTransmission(82); // transmit to device #82 (0x52)
  // the address specified in the datasheet is 164 (0xa4)
  // but i2c adressing uses the high 7 bits so it's 82
  Wire.write(byte(addr));      // sets distance data address (addr)
  Wire.endTransmission();      // stop transmitting
  // step 2: wait for readings to happen
  delay(1);                   // datasheet suggests at least 30uS
  // step 3: request reading from sensor
  Wire.requestFrom(82, cnt);    // request cnt bytes from slave device #82 (0x52)
  // step 5: receive reading from sensor
  if (cnt <= Wire.available()) { // if two bytes were received
    *datbuf++ = Wire.read();  // receive high byte (overwrites previous reading)
    *datbuf++ = Wire.read(); // receive low byte as lower 8 bits
  }
}
