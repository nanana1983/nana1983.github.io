#include <Arduino.h>
#include <TaskScheduler.h>
#include <PinChangeInterrupt.h>

// 스케줄러 객체 생성 (타이머 및 태스크 관리)
Scheduler runner;

// LED 핀 정의
const int LED_R = 9; //빨
const int LED_Y = 10; //노
const int LED_G = 11; //초초
const int POT_PIN = A0; // 가변저항 핀

// 버튼 핀 정의
const int BUTTON_EMERGENCY = 2; //빨간불만
const int BUTTON_BLINKING = 3; //모두깜빡
const int BUTTON_ONOFF = 4; //전원

// 함수 선언
void emergencyMode();
void blinkingMode();
void onOffMode();
void serialEvent();
void trafficLightCycle();

// 상태 변수 정의
volatile bool emergency = false;  // 비상 모드
volatile bool blinking = false;   // 깜빡임 모드
volatile bool onOff = true;       // 신호등 ON/OFF
volatile bool wasOff = false;     // 이전에 OFF 상태였는지 여부

// 가변저항 밝기 및 LED 점등 시간 변수
int brightness = 0;
int timeR = 2000, timeY = 500, timeG = 2000; // LED 점등 시간
String mode = "NORMAL";  // 현재 모드

void setup() {
  Serial.begin(9600); // 시리얼 통신 시작

  // 핀 모드 설정
  pinMode(LED_R, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(POT_PIN, INPUT);

  pinMode(BUTTON_EMERGENCY, INPUT_PULLUP);
  pinMode(BUTTON_BLINKING, INPUT_PULLUP);
  pinMode(BUTTON_ONOFF, INPUT_PULLUP);

  // 버튼 인터럽트 연결
  attachPCINT(digitalPinToPCINT(BUTTON_EMERGENCY), emergencyMode, FALLING);
  attachPCINT(digitalPinToPCINT(BUTTON_BLINKING), blinkingMode, FALLING);
  attachPCINT(digitalPinToPCINT(BUTTON_ONOFF), onOffMode, FALLING);
}

void loop() {
  serialEvent(); // 시리얼 데이터 처리
  runner.execute(); // 스케줄러 실행

  // 가변저항 값 읽기 및 시리얼 전송
  brightness = analogRead(POT_PIN);
  Serial.print("B:");
  Serial.println(brightness);

  // 현재 모드 전송
  Serial.print("M:");
  Serial.println(mode);

  if (onOff) {  // 신호등이 켜져 있을 경우
    if (wasOff) {  // 이전에 OFF였다면 모든 LED를 OFF 상태로 초기화
      wasOff = false;
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_Y, LOW);
      digitalWrite(LED_G, LOW);
    }

    if (mode == "EMERGENCY") { // 비상 모드 (빨간불만 ON)
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_Y, LOW);
      digitalWrite(LED_G, LOW);
      Serial.println("R1");
      Serial.println("Y0");
      Serial.println("G0");
    } else if (mode == "BLINKING") { // 깜빡임 모드 (모든 불이 깜빡임)
      digitalWrite(LED_R, !digitalRead(LED_R));
      digitalWrite(LED_Y, !digitalRead(LED_Y));
      digitalWrite(LED_G, !digitalRead(LED_G));

      // 현재 LED 상태를 시리얼로 전송
      Serial.println(digitalRead(LED_R) ? "R1" : "R0");
      Serial.println(digitalRead(LED_Y) ? "Y1" : "Y0");
      Serial.println(digitalRead(LED_G) ? "G1" : "G0");

      delay(500); // 깜빡임 속도 조절
    } else { // 기본 신호등 사이클 실행
      trafficLightCycle();
    }
  } else {  // 신호등이 꺼진 경우 (모든 LED OFF)
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_G, LOW);
    Serial.println("R0");
    Serial.println("Y0");
    Serial.println("G0");
    wasOff = true;
  }

  delay(500); // 루프 속도 조절
}

// 🟢 신호등 기본 사이클
void trafficLightCycle() {
  // 빨간불 점등
  analogWrite(LED_R, brightness);
  Serial.println("R1");
  delay(timeR);
  analogWrite(LED_R, 0);
  Serial.println("R0");

  // 노란불 점등
  analogWrite(LED_Y, brightness);
  Serial.println("Y1");
  delay(timeY);
  analogWrite(LED_Y, 0);
  Serial.println("Y0");

  // 초록불 점등
  analogWrite(LED_G, brightness);
  Serial.println("G1");
  delay(timeG);

  // 초록불 깜빡임 (3번)
  for (int i = 0; i < 3; i++) {
    analogWrite(LED_G, 0);
    Serial.println("G0");
    delay(166);
    analogWrite(LED_G, brightness);
    Serial.println("G1");
    delay(166);
  }

  // 초록불 OFF
  analogWrite(LED_G, 0);
  Serial.println("G0");

  // 노란불 점등 (초록불에서 빨간불로 변경하기 전)
  analogWrite(LED_Y, brightness);
  Serial.println("Y1");
  delay(timeY);
  analogWrite(LED_Y, 0);
  Serial.println("Y0");
}

// 🛑 비상 모드 (빨간불 ON, 다른 불 OFF)
void emergencyMode() {
  emergency = !emergency;
  if (emergency) {
    mode = "EMERGENCY";
    blinking = false; // 깜빡임 모드 해제
  } else {
    mode = "NORMAL";
  }
}

// ⚠️ 깜빡임 모드 (모든 불이 깜빡임)
void blinkingMode() {
  blinking = !blinking;
  if (blinking) {
    mode = "BLINKING";
    emergency = false; // 비상 모드 해제
  } else {
    mode = "NORMAL";
  }
}

// 🔘 신호등 ON/OFF 모드
void onOffMode() {
  onOff = !onOff;

  if (!onOff) { // OFF 상태로 변경
    mode = "ALLOFF";
  } else { // ON 상태일 때 NORMAL 모드로 복귀
    emergency = false;
    blinking = false;
    mode = "NORMAL";
  }
}

// 🔄 시리얼 데이터 처리 (슬라이더 값 수신)
void serialEvent() {
  while (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    data.trim();

    if (data.startsWith("B:")) { // 밝기 값 변경
      brightness = data.substring(2).toInt();
    } else if (data.startsWith("R:")) { // 빨간불 점등 시간 변경
      timeR = data.substring(2).toInt();
    } else if (data.startsWith("Y:")) { // 노란불 점등 시간 변경
      timeY = data.substring(2).toInt();
    } else if (data.startsWith("G:")) { // 초록불 점등 시간 변경
      timeG = data.substring(2).toInt();
    }
  }
}
