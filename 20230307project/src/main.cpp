#include <Arduino.h> // Arduino 라이브러리 포함

#include <TaskScheduler.h> // TaskScheduler 라이브러리 포함

#include <PinChangeInterrupt.h>

const int buttonPin = 7;


Scheduler runner; // TaskScheduler 객체 생성

void blinkTaskCallback(); // blinkTaskCallback 함수 선언

// 2초 간격으로 무한 반복하는 blinkTask 생성, 콜백 함수는 blinkTaskCallback
Task blinkTask(2000, TASK_FOREVER, &blinkTaskCallback);

void buttonPressed() {
	static bool mode = false;
	mode ^= 1;
	if (mode) {
		blinkTask.setInterval(500); // blinkTask의 주기를 0.5초로 변경
	} else {
		blinkTask.setInterval(1000); // blinkTask의 주기를 1초로 변경
	}
}

void setup() {
	pinMode(buttonPin, INPUT_PULLUP);

	attachPCINT(digitalPinToPCINT(buttonPin), buttonPressed, CHANGE);

	pinMode(LED_BUILTIN, OUTPUT); // 내장 LED 핀을 출력 모드로 설정
    runner.init(); // TaskScheduler 초기화
    runner.addTask(blinkTask); // blinkTask를 TaskScheduler에 추가
    blinkTask.enable(); // blinkTask 활성화
}

void loop() {
    runner.execute(); // TaskScheduler 실행, 등록된 모든 태스크 실행
}

void blinkTaskCallback() {
    static bool ledState = false; // LED 상태를 저장하는 변수, 초기값은 false
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW); // LED 상태에 따라 켜거나 끔
    ledState = !ledState; // LED 상태를 반전시킴
}
