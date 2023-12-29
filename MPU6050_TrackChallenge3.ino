int IN1 = A1;  //forward (m1)
int IN2 = A3;  //backwards (m1)
int IN3 = 12;  // forward(m2)
int IN4 = 13;  // backwards (m2)
int enA = 3;
int enB = 11;
int Right = 1;
int Left = 2;
int R;
int L;
int flg = 0;
int flg2 = 1;

//encoders
long int pulses = 0;
double distance;
int EncR = A0;
int EncL = A2;
int firstEncR;
int firstEncL;
int lastEncR;
int lastEncL;
long int changeR = 0;
long int changeL = 0;
char wheelDirection[1] = { 'N' };

#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);
unsigned long lastUpdateTime = 0;
unsigned long updateInterval = 100;
float finalAngle;

#include <LiquidCrystal.h>
//select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void measureDistance() {

  if (analogRead(EncR) > 100) {
    lastEncR = 1;
  } else {
    lastEncR = 0;
  }
  if (analogRead(EncL) > 100) {
    lastEncL = 1;
  } else {
    lastEncL = 0;
  }

  if (*wheelDirection == 'L' || *wheelDirection == 'D') {  //turn left
    if (lastEncR != firstEncR) {
      changeR++;
      firstEncR = lastEncR;
    }
  }
  if (*wheelDirection == 'R') {  //turn right
    if (lastEncL != firstEncL) {
      changeL++;
      firstEncL = lastEncL;
    }
  }
  pulses = changeR + changeL;
  distance = ((pulses / 40.0) * 3.14159 * 3 * 2);
}



void Neutral() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void Drive() {           
  analogWrite(enA, 65);  
  analogWrite(enB, 65);  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void driveFast() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void driveRamp() {
  analogWrite(enA, 100);
  analogWrite(enB, 100);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Reverse() {
  analogWrite(enA, 100);
  analogWrite(enB, 100);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {  //turn to the left
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  analogWrite(enA, 255);  //255
  analogWrite(enB, 255);  //100
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void rotateRight() {
  analogWrite(enA, 0);    
  analogWrite(enB, 245);  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


void displayAngle() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Angle:");
  lcd.print(mpu.getAngleY());
}

void displayDT(float printTime) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance");
  lcd.print(distance);
  lcd.setCursor(0, 2);
  lcd.print("Time");
  lcd.print(printTime);
}



  void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(Right, INPUT);  
    pinMode(Left, INPUT);   
    pinMode(EncL, INPUT);
    pinMode(EncR, INPUT);

    lcd.begin(16, 2);
    lcd.setCursor(0, 0);

    //For encoder
    if (analogRead(EncR) > 100) {  //determine first tick of encoder
      firstEncR = 1;
    } else {
      firstEncR = 0;
    }
    if (analogRead(EncL) > 100) {
      firstEncL = 1;
    } else {
      firstEncL = 0;
    }
  }

void loop() {

  R = digitalRead(Right);
  L = digitalRead(Left);
  unsigned long currentTime = millis();
  float printTime = currentTime / 1000;
  unsigned long stopTime;

  if (currentTime - lastUpdateTime >= updateInterval) {
    mpu.update();
    lastUpdateTime = currentTime;
  }
  if (flg == 0 || flg2 == 0) {//line following before ramp

    if (R == 0 && L == 0) {
      Drive();
      *wheelDirection = 'D';
    }
    if (R == 0 && L == 1) {
      turnLeft();
      *wheelDirection = 'L';
    }

    if (R == 1 && L == 0) {
      turnRight();
      *wheelDirection = 'R';
    }
    if (R == 1 && L == 1) {
      Neutral();
      *wheelDirection = 'N';
    }
  }

  if (mpu.getAngleY() > 10 && flg == 0) {//line following climbing ramp
    if (R == 0 && L == 0) {
      driveFast();
      *wheelDirection = 'D';
    }
    if (R == 0 && L == 1) {
      turnLeft();
      *wheelDirection = 'L';
    }

    if (R == 1 && L == 0) {
      turnRight();
      *wheelDirection = 'R';
    }
    if (R == 1 && L == 1) {
      Neutral();
      *wheelDirection = 'N';
    }
    flg=1;
  }

  if (flg == 1 && flg2 == 1) {//at ramp
    displayAngle();
    if (mpu.getAngleY()<5) {
      Drive();//move forward to center of ramp
      delay(100);
      Neutral();//stop 4s
      delay(4000);
      rotateRight();//rotate 360
      if(mpu.getAngleZ()>360 && flg ==1){
        Neutral();
        delay(1000);//stop before drive down the ramp
        flg==3;
        flg2==4;
      }      
    }
  }
  if (flg2 == 4 && flg == 3) {//line following after ramp

    if (R == 0 && L == 0) {
      Drive();
      *wheelDirection = 'D';
    }
    if (R == 0 && L == 1) {
      turnLeft();
      *wheelDirection = 'L';
    }

    if (R == 1 && L == 0) {
      turnRight();
      *wheelDirection = 'R';
    }
    if (R == 1 && L == 1) {
      Neutral();
      *wheelDirection = 'N';
    }
    measureDistance();
    displayDT(printTime);//display final distance and time

    if ((distance == 150)) {//stop at 150cm
      Neutral();
      delay(2000);
      driveFast();
      delay(200);
      return;
    }
  }
}