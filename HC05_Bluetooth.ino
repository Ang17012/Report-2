#include <SoftwareSerial.h>
SoftwareSerial mySerial(3,2); // TX & RX

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

char t = '\0';

void Neutral() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void Drive() {
  analogWrite(enA, 120); //different speed because motor have different power
  analogWrite(enB, 85);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

}

void Reverse() {
  analogWrite(enA, 120);
  analogWrite(enB, 85);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}

void turnLeft() {
  analogWrite(enA, 150);
  analogWrite(enB, 150);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}

void turnRight() {
  analogWrite(enA, 150);
  analogWrite(enB, 150);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  mySerial.begin(9600);
}

void loop() {
  if (mySerial.available() > 0) {
    t = mySerial.read();
    
    switch (t) {
      case '1':
        Drive();
        break;
      case '2':
        Reverse();
        break;
      case '3':
        turnRight();
        break;
      case '4':
        turnLeft();
        break;
      default:
        Neutral(); // Stop the car if an unrecognized command is received
        break;
    }
  }
}

