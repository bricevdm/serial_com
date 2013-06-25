/*************************************************************************
* File Name          : RobotController.ino
* Author             : Brice & Fred
* Updated            : 
* Version            : V1.0.0
* Date               : 6/24/2013
* Description        : Receive and parse instructions from serial
* License            : CC-BY-SA 3.0
**************************************************************************/

#include <Me_BaseShield.h>
#include <SoftwareSerial.h>
#include <Me_Bluetooth.h>

#include <Me_BaseShieldMotorDriver.h>
#include <Me_ServoDriver.h>
#include <Servo.h>

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

Me_ServoDriver servoDriver(PORT_1);//can ONLY be PORT_1,PORT_2
Me_BaseShieldMotorDriver baseShieldMotorDriver;// use M1 and M2 ports on BaseShield
Me_Bluetooth bluetooth(3);

int moveSpeed = 70;
int rotateSpeed = 50;


int minClawAngle = 90;
int maxClawAngle = 100;
int currentClawAngle = 90;

int previousArmAngle = 90;
int currentArmAngle = 90;

enum cmd {CMD_NONE, CMD_MOVE, CMD_STOP, CMD_CLAW, CMD_ARM, CMD_COUNT};


int cmdLen[256] = {
  1, // CMD_NONE
  3, // CMD_MOVE
  1, // CMD_STOP
  2, // CMD_CLAW
  2  // CMD_ARM
};
int curCmdLen = 0;
uint8_t bufferInput[256] = {0};

uint8_t bufferTestInput[256] = {CMD_MOVE,50,50};
int bufferTestLength = 3;
int bufferTestPos = 0;

void setup()
{
    pinMode(led, OUTPUT);
    
    for(int i=0;i<10;i++)
    {
      digitalWrite(led, !digitalRead(led));   // toggles the LED on/off
      delay(300);               // wait
    }
    
    Serial.begin(9600);
    Serial.println("Serial connection");
    baseShieldMotorDriver.begin();
    bluetooth.begin(9600);
    Serial.println("Bluetooth Start!");
    
    servoDriver.Servo1_begin();
    servoDriver.Servo2_begin();
    
    //servoDriver.writeServo1(minClawAngle);
    //servoDriver.writeServo2(currentArmAngle);
}

void loop()
{
    int input = -1;
   
   /*
    if(bluetooth.available())
    {
      input = bluetooth.read();
      //mapInputChar(key);
    }*/
   
    if(Serial.available())
    {
        input = Serial.read();
        //mapInputChar(key);
    }
    
    if (input < 0) 
    {
      return;
    }
    //Serial.write(input);
    
    /*input = bufferTestInput[bufferTestPos++];
    if( bufferTestPos >= bufferTestLength){
      bufferTestPos = 0;
    }*/
    
    if ( curCmdLen == cmdLen[bufferInput[0]] )
    {
      switch(bufferInput[0])
      {
        case CMD_MOVE:
          runM((int)bufferInput[1] - 128, -(int)bufferInput[2] + 128);
          break;
        case CMD_STOP:
          runStop();
          break;
        case CMD_CLAW:
          setClaw( (int)bufferInput[1] );
          break;
        case CMD_ARM:
          setArm( (int)bufferInput[1] );
          break;
      }
      
      curCmdLen = 0;
    }
    
    if (curCmdLen == 0) 
    {
      if (input < CMD_COUNT)
      {
        bufferInput[curCmdLen++] = input;
      }
    }
    else
    {
      bufferInput[curCmdLen++] = input;
    }
}

void runM(int m1, int m2)
{
    //Serial.println("run motors");
    baseShieldMotorDriver.runMotors(m1,m2);
}

void runStop()
{
    //Serial.println("run stop");
    baseShieldMotorDriver.stopMotors();
}

void setClaw(int input)
{
  servoDriver.writeServo1(input);
  Serial.write(servoDriver.readServo1());
}

void setArm(int input)
{
  servoDriver.writeServo2(input);
  Serial.write(servoDriver.readServo2());
}


/*
789
4 6
123
*/

/*
void mapInputChar (char x)
{
      
    int r = rotateSpeed;
    int m = moveSpeed;
    
    switch (x)
    {
        case '7':runM( m,-r);break;
        case '8':runM( m,-m);break;
        case '9':runM( r,-m);break;
        case '6':runM( r, r);break;
        case '3':runM(-r, m);break;
        case '2':runM(-m, m);break;
        case '1':runM(-m, r);break;
        case '4':runM(-r,-r);break;
        
        default:runStop();
    }
    
    servoDriver.writeServo1(currentClawAngle);
    
    if (previousArmAngle!=currentArmAngle)
      servoDriver.writeServo2(currentArmAngle);
      
    previousArmAngle = currentArmAngle;
}*/
