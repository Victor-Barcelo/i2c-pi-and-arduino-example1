#include <Wire.h>
#include <Time.h>

#define SLAVE_ADDRESS 0x07
#define DEBUG true
#define DEBUG_SHOW_TIME false

const int LIGHTSENSORPIN = 0;
const int TEMPSENSORPIN = 1;
bool isTimeSet = false;

void setup(){
    Serial.begin(9600);
    Wire.begin(SLAVE_ADDRESS);
    Wire.onRequest(sendData);
    Wire.onReceive(receiveData);
    if(DEBUG) Serial.println("Ready!");
}

void loop(){
    // delay(100);
    if(isTimeSet && DEBUG_SHOW_TIME){
        digitalClockDisplay(); 
    }
}

void sendData(){
  byte lightData[2];
  getLightData(lightData);
  byte temperatureData[2];
  getTemperatureData(temperatureData);
  byte dataSend[] = {temperatureData[0],temperatureData[1],lightData[0],lightData[1]};
  Wire.write(dataSend, 4);
}

void receiveData(int byteCount){
  int cmd = Wire.read();
  if(cmd == 0) return;
  if(cmd == 1){
    unsigned long bytes[4];
    int i = 0;
    while(Wire.available()){
      bytes[i] = Wire.read();
      i++;
    }
      setActualTime(bytes);
  }
}

void setActualTime(unsigned long bytes[4]){
  unsigned long result = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);
  setTime(result);
  isTimeSet = true;
}

void getLightData(byte * data){
  int light = readLight(); 
  if(DEBUG) Serial.println(light);
  data[0] = lowByte(light);
  data[1] = highByte(light);
}

void getTemperatureData(byte * data){
  int temperature = readTemperature(); 
  if(DEBUG) Serial.println(temperature);
  data[0] = lowByte(temperature);
  data[1] = highByte(temperature);
}

int readLight(){
  int rawLight = analogRead(LIGHTSENSORPIN);
  return rawLight;
}

int readTemperature(){
  int rawTemp = analogRead(TEMPSENSORPIN);
  int celsiusTemp = (125*rawTemp)>>8;
  return celsiusTemp;
}

void digitalClockDisplay() {
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(dayStr(weekday()));
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(monthStr(month()));
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits) {
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}