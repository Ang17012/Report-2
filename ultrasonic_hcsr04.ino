int trig = 9;
int echo = 10;

float duration, distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
}

void loop() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  if distance<10.0 {
    Neutral();
  }
  else{
    Drive();
  }
}



