#include <Servo.h>

const int numServos = 12;
const int availServos = 12; // 8 for UNO, 18 for MEGA
word servoValues[numServos];
int servoPins[] = {2,3,4,5,6,7,8,9,10,11,12,13};
Servo servo[availServos];

int servoSpeed = 10; 
int servoPulse = 1000;
int servoMin = 750;
int servoCentre = 1450;
int servoMax = 2150;
int servoMaxSpeedThreshold = 2000;
int servoMinSpeedthreshold = 900;

int minSpeed = 1;
int maxSpeed = 20;

void setup()
{
  servoPulse = servoCentre;
  
  // setup servos
  for(int i=0; i<availServos; i++){
    servo[i].attach(servoPins[i]);
    servoValues[i] = servoPulse;
    servo[i].writeMicroseconds(servoCentre);
  }
  
  
  delay(3000);
  
  //Serial.begin(9600);
}

void loop()
{
      //delay(1000);
  /*
    while(servoPulse < 2000){
         servoPulse += 30;
          for(int i=0; i<availServos; i++)
            servo[i].writeMicroseconds(servoPulse);
          delay(25);
       }
       
       while(servoPulse > 2000){
         servoPulse -= 30;
          for(int i=0; i<availServos; i++)
            servo[i].writeMicroseconds(servoPulse);
          delay(25);
       }
  
    
  
  
      for(int i=0; i<availServos; i++)
        servo[i].writeMicroseconds(1000);

      delay(3000);

      for(int i=0; i<availServos; i++)
        servo[i].writeMicroseconds(1420);
        
      delay(3000);
         
      for(int i=0; i<availServos; i++)
        servo[i].writeMicroseconds(2000);
        
       delay(3000);
       
        for(int i=0; i<availServos; i++)
        servo[i].writeMicroseconds(1420);
        
        delay(3000);
       
       */

      //delay(3000);
      
      // int servoPulse = 2000;
       while(servoPulse > servoMin){
         servoPulse -= servoSpeed;
          for(int i=0; i<availServos; i++)
            servo[i].writeMicroseconds(servoPulse);
          delay(20);
       }
      
      //delay(3000);
      
       while(servoPulse < servoMax){
         servoPulse += servoSpeed;
          for(int i=0; i<availServos; i++)
            servo[i].writeMicroseconds(servoPulse);
          delay(20);
       }

}
