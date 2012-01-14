#include <Servo.h>

const int numServos = 12;
const int availServos = 12; // 8 for UNO, 18 for MEGA
word servoValues[numServos];
int servoPins[] = {2,3,4,5,6,7,8,9,10,11,12,13};
Servo servo[availServos];


void setup()
{
  // setup servos
  for(int i=0; i<availServos; i++){
    servo[i].attach(servoPins[i]);
    servoValues[i] = 1000;
    servo[i].writeMicroseconds(int(servoValues[i]));
  }
  
  Serial.begin(9600);
}

void loop()
{
      //delay(1000);
  
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
       
       /*
       int speed = 2300;
       while(speed > 600){
         speed -= 70;
          for(int i=0; i<availServos; i++)
            servo[i].writeMicroseconds(speed);
          delay(25);
       }
       */

}
