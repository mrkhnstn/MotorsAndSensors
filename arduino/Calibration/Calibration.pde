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

const word minServoPulse = 550;
const word maxServoPulse = 2350;

const int availAnalogIns = 9; // 6 for UNO, 9 for MEGA
const int numAnalogIns = 9;
word analogValues[numAnalogIns];
int analogPins[] = {0,2,4,6,7,9,11,13,15};

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
   //  update servos
   for(int i=0; i<availServos; i++)
   {
     servo[i].writeMicroseconds(1450);
   }
   delay(30);
}
