#include <Servo.h>

Servo servo;

int potPin = A0;
int servoPin = 9;
int greenLED = 6;
int redLED = 7;

void setup() {
  servo.attach(servoPin);

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potPin);        // 0 ~ 1023
  int angle = map(potValue, 0, 1023, 0, 180);

  servo.write(angle);
  Serial.println(angle);

  //안전 로직
  if (angle >= 30 && angle <= 150) {
    //정상 범위
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
  } else {
    //위험 범위
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
  }

  delay(20);
}