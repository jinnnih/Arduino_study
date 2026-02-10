const int trigPin = 9;
const int echoPin = 10;
int AA = 6;
int AB = 7;


long duration;
int distance;
void setup() {
  pinMode(AA, OUTPUT);
  pinMode(AB, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);


 if (distance > 25) {
  analogWrite(AA, 200);  // 빠르게
  digitalWrite(AB, LOW);
}
else if (distance > 15) {
  analogWrite(AA, 100);  // 천천히
  digitalWrite(AB, LOW);
}
else {
  analogWrite(AA, 0);    // 정지
  digitalWrite(AB, LOW);
}


  if (distance == 0) return;
  delay(500);
}
