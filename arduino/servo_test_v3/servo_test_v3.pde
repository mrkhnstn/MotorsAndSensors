#include <Servo.h>

const int numServos = 12;
const int availServos = 12; // 8 for UNO, 18 for MEGA
word servoValues[numServos];
int servoPins[] = {2,3,4,5,6,7,8,9,10,11,12,13};
Servo servo[availServos];

int servoSpeed = 10; 
int servoPulse = 1000;
int servoMin = 798;
int servoCentre = 1450;
int servoMax = 2150;
int servoMaxSpeedThreshold = 2000;
int servoMinSpeedThreshold = 1650;

int tempMin = servoMin;
int tempMax = servoCentre;

int minSpeed = 1;
int maxSpeed = 20;
int acceleration = 1;

void setup()
{
  servoPulse = servoCentre;
  
  // setup servos
  for(int i=0; i<availServos; i++){
    servo[i].attach(servoPins[i]);
    servoValues[i] = servoPulse;
    servo[i].writeMicroseconds(servoCentre);
  }
  
  //servoMin = servoCentre;
  //servoMax = servoCentre;
  //servoMaxSpeedThreshold = servoMax - 100;
  //servoMinSpeedThreshold = servoMin + 100;
  
  delay(3000);
  
  //Serial.begin(9600);
}

void loop()
{
      delay(3000);

      moveLeft(servoMin);

       delay(3000);
       
       moveRight(servoCentre);
       
       delay (3000);
       
       moveRight(servoMax);
       
       delay(3000);
       
       moveLeft(servoCentre);
       
       delay (3000);
       
       moveLeft(servoMin);
      

}



void moveLeft(int target){
 servoSpeed = 1;
    
    servoMinSpeedThreshold = target + 200;
    
    while(servoPulse > target){
              
         // adjust speed

          if(servoPulse > servoMinSpeedThreshold){
          // accelerate
           if(servoSpeed < maxSpeed){
             servoSpeed += acceleration; 
             if(servoSpeed > maxSpeed){
               servoSpeed = maxSpeed;
             } 
           }
         } else {
           // decelerate
           if(servoSpeed > minSpeed){
             servoSpeed -= acceleration;
             if(servoSpeed < minSpeed)
               servoSpeed = minSpeed;
           }
         }
         
         // adjust pulse
         servoPulse -= servoSpeed;
         
          for(int i=0; i<availServos; i++)
            servo[i].writeMicroseconds(servoPulse);
          delay(20);
       }
}

void moveRight(int target){
  servoSpeed = 1;
  servoMaxSpeedThreshold = target - 200;
       while(servoPulse < target){
        
         if(servoPulse < servoMaxSpeedThreshold){
          // accelerate
           if(servoSpeed < maxSpeed){
             servoSpeed += acceleration; 
             if(servoSpeed > maxSpeed){
               servoSpeed = maxSpeed;
             } 
           }
         } else {
           // decelerate
           if(servoSpeed > minSpeed){
             servoSpeed -= acceleration;
             if(servoSpeed < minSpeed)
               servoSpeed = minSpeed;
           }
         }
         
         // adjust pulse
         servoPulse += servoSpeed;
     

          for(int i=0; i<availServos; i++)
            servo[i].writeMicroseconds(servoPulse);
          delay(20);
       }
}
