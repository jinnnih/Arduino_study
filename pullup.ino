
const int ledpin = 13;
const int buttonPin =2;

int buttonState = 0;


void setup() {
  //pinMode (LED_BUILTIN, OUTPUT);
  pinMode (ledpin, OUTPUT);
  pinMode (buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH){
    digitalWrite(ledpin, HIGH); // LED 켜기
    //delay(1000); // 1초 = 1000 딜레이
  } else{
    digitalWrite(ledpin, LOW);  // LED 끄기
    //delay(500);
  }


}
