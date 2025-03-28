# 아두이노와 p5.js 웹 시리얼을 이용한 신호등 제어

## 아두이노 코드 설명

### 주요 기능

신호등 제어 시스템 유튜브 링크 : https://youtu.be/YOF2D1ZUqOs?si=luaq8cqdKk7rCMkN

1. 기본 신호등 사이클

LED가 정해진 순서대로 점등됨

빨간불 → 노란불 → 초록불 → 초록불 3번 깜빡임 → 노란불 → 빨간불


2. 비상 모드 (EMERGENCY MODE)

빨간불만 켜지고, 나머지 LED는 꺼짐

BUTTON_EMERGENCY 버튼을 누르면 활성화


3. 깜빡임 모드 (BLINKING MODE)

모든 LED가 동기적으로 깜빡임

BUTTON_BLINKING 버튼을 누르면 활성화



4. 신호등 ON/OFF 모드 (ALL OFF)

BUTTON_ONOFF 버튼을 누르면 모든 LED가 꺼지고 다시 누르면 정상 모드로 복귀

가변저항을 통한 LED 밝기 조절

POT_PIN을 통해 LED의 밝기를 조절 가능



5. p5.js 웹과의 시리얼 통신

시리얼 포트를 통해 신호등의 현재 상태를 p5.js로 전송

p5.js에서 신호등 점등 시간을 조절할 수 있도록 슬라이더 값 수신


## 아두이노 태스크 관리
Task1: LED 신호 주기적 점등

trafficLightCycle() 함수를 실행하여 신호등 점등

timeR, timeY, timeG 변수에 따라 각 LED가 켜지는 시간이 다름

비상 모드 및 깜빡임 모드가 아닌 경우 실행


Task2: LED OFF 제어

신호등이 OFF 모드일 경우, 모든 LED를 OFF

비상 모드에서는 빨간불만 켜짐

깜빡임 모드에서는 모든 LED가 깜빡임


Task3: 시리얼 입력 확인 및 적용

serialEvent()를 통해 시리얼 데이터를 수신

밝기(B:), 빨간불 시간(R:), 노란불 시간(Y:), 초록불 시간(G:) 값 조절



## p5.js 코드 설명

![p5js사진기](https://github.com/user-attachments/assets/323daf2a-f1ef-4580-a1bc-ab308a0bce34)

주요 기능
시리얼 메시지 모니터링


아두이노에서 시리얼 포트로 전송된 메시지를 받아와 분석

msg: LED ON 감지 → 빨간색 원 표시

msg: LED OFF 감지 → 회색 원 표시

LED 상태 시각화


R1 → 빨간 LED ON

R0 → 빨간 LED OFF

Y1 → 노란 LED ON

Y0 → 노란 LED OFF

G1 → 초록 LED ON

G0 → 초록 LED OFF

슬라이더를 통한 신호 주기 조절



슬라이더를 움직이면 LED의 점등 시간이 변경됨

B:(밝기), R:(빨간불 지속시간), Y:(노란불 지속시간), G:(초록불 지속시간) 데이터를 아두이노로 전송


## 회로 구성

### LED 연결 (신호 기본 High/Low)
![회로도](https://github.com/user-attachments/assets/c98121bb-7007-4a5b-929e-98e82475c5fe)
![회로도실물](https://github.com/user-attachments/assets/bbff05d7-411e-4a52-aa19-2066b7d2f286)


디지털 핀(D9, D10, D11)에서 LED를 제어

LOW(0) → LED OFF

HIGH(1) → LED ON

전류 제한 저항을 통해 LED 보호

브레드보드에서 공통 GND 연결




LED 색상   아두이노 핀   점등 시 HIGH / LOW

빨간 LED 🔴   D9   HIGH: ON / LOW: OFF

노란 LED 🟡   D10   HIGH: ON / LOW: OFF

초록 LED 🟢   D11   HIGH: ON / LOW: OFF



### 버튼 연결 (풀업 저항 사용)

디지털 핀(D2, D3, D4)에 연결된 버튼은 INPUT_PULLUP 모드 사용

기본적으로 HIGH(1) 상태를 유지하고, 버튼을 누르면 LOW(0)로 변환됨

아두이노 내부 풀업 저항(약 20kΩ) 사용

GND에 연결하여 버튼이 눌릴 때 LOW 신호 발생


버튼 기능   아두이노 핀   기본 상태 (미누름)   눌렸을 때 상태

비상 모드 🚨   D2   HIGH (1)   LOW (0)

깜빡임 모드 ⚠️   D3   HIGH (1)   LOW (0)

ON/OFF 버튼 🔘   D4   HIGH (1)   LOW (0)



### 가변저항 연결 (아날로그 입력)

가변저항(POT)의 중앙 핀 → A0 (아날로그 입력)

한쪽 핀 → 5V, 다른 쪽 핀 → GND

아날로그 값을 읽어 LED 밝기를 조절 (0 ~ 1023 범위)



### 전원 연결

아두이노 5V → 브레드보드 전원 레일

아두이노 GND → 브레드보드 접지 레일


## 요약

### 아두이노 역할

신호등 기본 주기 동작

비상 모드, 깜빡임 모드, 신호등 ON/OFF 기능

가변저항을 통한 밝기 조절

시리얼 통신으로 p5.js와 데이터 교환


### p5.js 역할

웹에서 시리얼 데이터 모니터링

신호등 상태를 시각적으로 표현

슬라이더를 통해 LED 점등 시간 및 밝기 조절


⚡ 추가 기술 개념

LED 기본 신호: HIGH(1) → 켜짐 / LOW(0) → 꺼짐

버튼 입력: INPUT_PULLUP 모드 사용 (기본 HIGH, 눌리면 LOW)

아날로그 입력: 가변저항을 통한 0~1023 범위의 데이터 수신 및 밝기 조절
