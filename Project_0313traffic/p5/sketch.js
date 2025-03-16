let port;
let connectBtn;
let sliderbrightness;
let sliderRLED;
let sliderYLED;
let sliderGLED;
let circleColorR = 'gray';
let circleColorY = 'gray';
let circleColorG = 'gray';
let modestr = 'NORMAL';
let brightness = 0;
let timeR = 2000, timeY = 500, timeG = 2000; // 초기 LED 지속 시간

function setup() {
  createCanvas(520, 300);
  background(220);

  port = createSerial();
  let usedPorts = usedSerialPorts();
  if (usedPorts.length > 0) {
    port.open(usedPorts[0], 9600);
  }

  connectBtn = createButton("Connect to Arduino");
  connectBtn.position(350, 60);
  connectBtn.mousePressed(connectBtnClick);

  // 전체 LED 밝기 조절 슬라이더
  sliderbrightness = createSlider(500, 5000, 2000, 10);
  sliderbrightness.position(10, 10);
  sliderbrightness.size(500);
  sliderbrightness.mouseReleased(changeSlider);

  // 개별 LED 지속 시간 조절 슬라이더 (우측으로 갈수록 LED가 오래 켜짐)
  sliderRLED = createSlider(500, 5000, timeR, 10);
  sliderRLED.position(10, 30);
  sliderRLED.size(100);
  sliderRLED.mouseReleased(changeSlider);

  sliderYLED = createSlider(500, 5000, timeY, 10);
  sliderYLED.position(200, 30);
  sliderYLED.size(100);
  sliderYLED.mouseReleased(changeSlider);

  sliderGLED = createSlider(500, 5000, timeG, 10);
  sliderGLED.position(400, 30);
  sliderGLED.size(100);
  sliderGLED.mouseReleased(changeSlider);

  textSize(18);
  fill(0);
}

function draw() {
  let n = port.available();
  if (n > 0) {
    let str = port.readUntil("\n");
    background(220);
    fill(0);
    text("msg: " + str, 10, 200);

    if (str.includes("B:")) {
      brightness = parseInt(str.split("B:")[1]);
    }

    if (str.includes("M:")) {
      modestr = str.split("M:")[1].split(",")[0];
    }

    if (str.includes("R1")) {
      circleColorR = 'red';
    } else if (str.includes("R0")) {
      circleColorR = 'gray';
    }

    if (str.includes("Y1")) {
      circleColorY = 'yellow';
    } else if (str.includes("Y0")) {
      circleColorY = 'gray';
    }

    if (str.includes("G1")) {
      circleColorG = 'green';
    } else if (str.includes("G0")) {
      circleColorG = 'gray';
    }
  }

  fill(circleColorR);
  circle(100, 100, 50);
  fill(circleColorY);
  circle(200, 100, 50);
  fill(circleColorG);
  circle(300, 100, 50);

  fill("red");
  text("MODE: " + modestr, 10, 150);
  fill("black");
  text("BRIGHTNESS: " + brightness, 10, 180);
}

function connectBtnClick() {
  port.opened() ? port.close() : port.open(9600);
}

// LED 지속 시간을 아두이노에 전송
function changeSlider() {
  port.write("B:" + sliderbrightness.value() + "\n");
  port.write("R:" + sliderRLED.value() + "\n");
  port.write("Y:" + sliderYLED.value() + "\n");
  port.write("G:" + sliderGLED.value() + "\n");
}
