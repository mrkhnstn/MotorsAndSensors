#include <Servo.h>

//const int lf = 10;
const int baudRate = 115200;

const int numServos = 18;
const int availServos = 8; // 8 for UNO, 18 for MEGA
word servoValues[numServos];

const int availAnalogIns = 6; // 6 for UNO, 9 for MEGA
const int numAnalogIns = 9;
word analogValues[numAnalogIns];

const int bufferSize = 38; // = 2 * numServos + 2 (two bytes per number, one extra number for initial command)
byte readBuffer[bufferSize];
int readIndex = 0;

Servo servo[availServos];

int irLedPins[] = {9,10,11,12,13};
int irLedIndex = 0;
const int irVinPin = 8;

int command = 0; // 0: do not return anything // 1: return next set of analog values

void setup()
{
  Serial.begin(115200);
  
  // setup IR ranger related pins
  pinMode(irVinPin,OUTPUT);
  for(int i=0; i<5; i++){
    pinMode(irLedPins[i],OUTPUT);
  }
  
  // setup servos
  for(int i=0; i<availServos; i++){
    servo[i].attach(0+i); // start from pin 0
    servoValues[i] = 1000;
    servo[i].writeMicroseconds(int(servoValues[i]));
  }
}

void loop()
{
  while (Serial.available() > 0) {   
    readBuffer[readIndex] = Serial.read();
    readIndex++;
    if(readIndex == bufferSize){ // read done
      readIndex = 0; // reset readIndex for next read operation
      
      // populate servo values with received values
      int i = 0;
      command = word(readBuffer[i],readBuffer[i+1]); // first number is command number
      i+=2; 
      int j = 0;
      while(i < bufferSize && j < numServos){
        servoValues[j] = word(readBuffer[i],readBuffer[i+1]);
        i+=2;
        j++;
      }

      /*
      // echo servo values
      for(i=0; i<numServos; i++)
      {
        Serial.write(highByte(servoValues[i]));
        Serial.write(lowByte(servoValues[i]));
      }
      */
      
      // TODO set servos
      for(i=0; i<availServos; i++)
      {
        servo[i].writeMicroseconds(int(servoValues[i]));
      }
      
      
      if(command == 0){
        // send 0s
       // send IR led index
        word temp = word(0);
        Serial.write(highByte(temp));
        Serial.write(lowByte(temp));
        
        // send analog values
         for(i=0; i<numAnalogIns; i++){
          Serial.write(highByte(temp));
          Serial.write(lowByte(temp));
        }
      }
      
      
      if(command == 1){
      
        // read analog
        for(i=0; i<availAnalogIns; i++){
          analogValues[i] = word(analogRead(i));
        }
        
        // send IR led index
        word temp = word(irLedIndex+1);
        Serial.write(highByte(temp));
        Serial.write(lowByte(temp));
        
        // send analog values
         for(i=0; i<numAnalogIns; i++){
          Serial.write(highByte(analogValues[i]));
          Serial.write(lowByte(analogValues[i]));
        }
        
        // reset IR rangers
        digitalWrite(irVinPin, LOW);
        for(i=0; i<5; i++){
          digitalWrite(irLedPins[i], LOW);
        }
        delay(6); // min reset signal is 5ms
        
        // setup for next IR Led
        irLedIndex++;
        if(irLedIndex == 5)
          irLedIndex = 0;
          
        digitalWrite(irVinPin, HIGH);
        digitalWrite(irLedPins[irLedIndex], HIGH);
      }
    }
  }
}
