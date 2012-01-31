#include <Servo.h>
// configured to work with Arduino Mega 2560

const int bank = 1; // this has to be individually set for each arduino!!
const int baudRate = 115200;
const int numServos = 18;
const int availServos = 18; // 8 for UNO, 18 for MEGA
word servoValues[numServos];
//int servoPins[] = {24,25,26,27,30,31,32,33,36,37,38,39,42,43,44,45,48,49}; //50,51 //< extra 2 servo pints
//int servoPins[] = {24,25,26,27,30,31,32,33,36,37,38,39,42,43,44,45,48,49};
//int servoPins[] = {24,26,30,32,36,38,42,44,48,25,27,31,33,37,39,43,45,49};
int servoPins[] = {24,26,30,32,36,38,42,44,48,49,45,43,39,37,33,31,27,25};

const word minServoPulse = 500;
const word maxServoPulse = 2400;

const int availAnalogIns = 9; // 6 for UNO, 9 for MEGA
const int numAnalogIns = 9;
word analogValues[numAnalogIns];
// bank 2: {0,2,4,5,7,8,10,12,14};
// bank 1: {0,2,4,5,7,8,10,12,15};
int analogPins[] = {0,2,4,5,7,8,10,12,15};

const int bufferSize = 38; // = 2 * numServos + 2 (two bytes per number, one extra number for initial command)
byte readBuffer[bufferSize];
int readIndex = 0;

Servo servo[availServos];

int irLedPins[] = {8,9,10,11,12};
int irLedIndex = 0;
const int irVinPin = 13;

int command = 0; // 0: do not return anything // 1: return next set of analog values

void setup()
{
  Serial.begin(115200);
  analogReference(INTERNAL2V56);
  
  // setup IR ranger related pins
  pinMode(irVinPin,OUTPUT);
  for(int i=0; i<5; i++){
    pinMode(irLedPins[i],OUTPUT);
  }
  
  // setup servos
  for(int i=0; i<availServos; i++){
    servo[i].attach(servoPins[i]);
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
        word readVal = word(readBuffer[i],readBuffer[i+1]);
        // servo safeguard, make sure that pulses are not outside bounds
        
        if(readVal > maxServoPulse){
          readVal = maxServoPulse;
        } else if(readVal < minServoPulse){
          readVal = minServoPulse;
        }
        servoValues[j] = readVal;
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
      
      //  update servos
      for(i=0; i<availServos; i++)
      {
        servo[i].writeMicroseconds(int(servoValues[i]));
      }
      
      
      if(command == 0){
                
        word bankId = word(bank);
        Serial.write(highByte(bankId));
        Serial.write(lowByte(bankId));
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
          analogValues[i] = word(analogRead(analogPins[i]));
        }
        
        word bankId = word(bank);
        Serial.write(highByte(bankId));
        Serial.write(lowByte(bankId));
        
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
