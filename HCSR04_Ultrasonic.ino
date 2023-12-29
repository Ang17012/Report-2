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
int trigPin = 9;
int echoPin = 10;

float duration, distance;

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

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(Right, INPUT);  
  pinMode(Left, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
}

void loop() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  if (distance<10.0) {
    Neutral();
  }
  else{
    Drive();
  }
}



