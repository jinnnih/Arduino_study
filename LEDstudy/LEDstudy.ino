const int ledAnalog = 10;   // 센서로 제어할 LED핀1
const int ledButton = 12;  // 버튼으로 제어할 LED핀2
const int buttonPin = 2;
const int sensorPin = A0;

int buttonState = 0;
int sensorValue = 0;

void setup() {
  //pinMode (LED_BUILTIN, OUTPUT);
  pinMode(ledAnalog, OUTPUT);
  pinMode(ledButton, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);  // 9600 baud를 쓰겠다는 뜻
}

void loop() {
  buttonState = digitalRead(buttonPin);
  sensorValue = analogRead(sensorPin);

  // LED 1 : 아날로그 값으로 제어
  if (sensorValue >= 500) {
    digitalWrite(ledAnalog, HIGH);
  } else {
    digitalWrite(ledAnalog, LOW);
  }
  
  // LED 2 : 버튼으로 제어
  if (buttonState == HIGH) {
    digitalWrite(ledButton, HIGH);
  } else {
    digitalWrite(ledButton, LOW);
  }

  // 시리얼 모니터에 출력
  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print("\nButton: ");
  Serial.println(buttonState);

  delay(100);


}
