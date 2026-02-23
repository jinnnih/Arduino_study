#ifdef ESP32  //esp32 조건 실행
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif   // if 조건문 닫기
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "5층";  //ssid라는 변수를 5층이라고 미리 맵핑을 해둔 것
const char* password = "48864886";  // 비밀번호 동일

const char* PARAM_INPUT_1 = "state";  

const int output = 2;   // 출력핀 2번 지정 
const int buttonPin = 4;  // 버튼핀 4번 지정

// Variables will change:
int ledState = LOW;          
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  //
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);  //포트 80 지정

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?state=1", true); }
  else { xhr.open("GET", "/update?state=0", true); }
  xhr.send();
}

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var inputChecked;
      var outputStateM;
      if( this.responseText == 1){ 
        inputChecked = true;
        outputStateM = "On";
      }
      else { 
        inputChecked = false;
        outputStateM = "Off";
      }
      document.getElementById("output").checked = inputChecked;
      document.getElementById("outputState").innerHTML = outputStateM;
    }
  };
  xhttp.open("GET", "/state", true);
  xhttp.send();
}, 1000 ) ;
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){    // processor 라는 함수 선언
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){     
    String buttons ="";  // "" < 안에 아무 내용이 없으면 초기화
    String outputStateValue = outputState();
    buttons+= "<h4>Output - GPIO 2 - State <span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String outputState(){  // led가 켜져있으면 checked 반환
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);   // 시리얼 통신을 위해 보드레이트 맞춤

  pinMode(output, OUTPUT);  // 아두이노에서 핀을 쓰려면 미리 셋업할때 선언을 해야함
  digitalWrite(output, LOW);  // led가 꺼져있는 상태로 실행
  pinMode(buttonPin, INPUT);  // 버튼핀을 사용하겠다고 선언
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {   // 와이파이가 연결되지 않았으면 딜레이 1초 주겠다
    delay(1000);
    Serial.println("Connecting to WiFi.."); // 연결하려고 찾고 있다
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP()); // 연결되고나서 연결된 아이피 표시

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {   //플라스크의 라우팅 함수 같은 역할
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(output, inputMessage.toInt());
      ledState = !ledState; // led 상태를 반대로 바꾸겠다.
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/state
  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(output)).c_str());
  });
  // Start server
  server.begin(); // 서버를 실행하겠다
}
  
void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);   //read 함수에 버튼 상태를 저장

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {  // 버튼이 이전상태와 같지 않으면
    // reset the debouncing timer
    lastDebounceTime = millis();    // 밀리스 함수, 딜레이 대신 사용 / 동시제어를 위한 밀리스 사용
  }

  if ((millis() - lastDebounceTime) > debounceDelay) { 
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {   
      buttonState = reading;      // 버튼스테이트 값에 넣어주기

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;     // 버튼스테이트가 하이면 led 상태를 바꾼다.
      }
    }
  }

  // set the LED:
  digitalWrite(output, ledState);     // 루프하기 전에 버튼 핀을 led로 만들겠다.

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;   // 마지막 버튼 상태를 저장
}
