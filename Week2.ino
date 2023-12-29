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

#include <LiquidCrystal.h>
//select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

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

void turnLeft() {  
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  analogWrite(enA, 255);  
  analogWrite(enB, 255);  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

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

  if (*wheelDirection == 'L' || *wheelDirection == 'D') {  
    if (lastEncR != firstEncR) {
      changeR++;
      firstEncR = lastEncR;
    }
  }
  if (*wheelDirection == 'R') {  
    if (lastEncL != firstEncL) {
      changeL++;
      firstEncL = lastEncL;
    }
  }
  pulses = changeR + changeL;
  distance = ((pulses / 40.0) * 3.14159 * 3 * 2);
}

void displayDT() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
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

     if (R == 1 && L == 1) {
      Drive();
      *wheelDirection = 'D';
    }
    if (R == 1 && L == 0) {
      turnLeft();
      *wheelDirection = 'L';
    }

    if (R == 0 && L == 1) {
      turnRight();
      *wheelDirection = 'R';
    }
    if (R == 0 && L == 0) {
      Neutral();
      *wheelDirection = 'N';
    }
    measureDistance();
    displayDT();
}