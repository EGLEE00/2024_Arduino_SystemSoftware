#include <SoftwareSerial.h> // SoftwareSerial 라이브러리 포함

SoftwareSerial mySerial(2, 3); // RX = 2, TX = 3

String value = " ";
int iValue;
int num = 5, speed = 100;

// 적외선 센서 핀 설정
const int IR_LEFT = 12; // 왼쪽 센서: D12
const int IR_RIGHT = 13; // 오른쪽 센서: D13 

const int mq3Pin = A5; // MQ-3 센서핀

void setup() {
  mySerial.begin(9600); // SoftwareSerial 시작
  Serial.begin(9600); 

  // 모터 핀 설정
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT); 

  // 적외선 센서 핀 설정
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
}

void loop() {
  // RC 명령 수신
  if (mySerial.available()) {
    value = mySerial.readStringUntil('q'); // 데이터 읽기
    iValue = value.toInt(); // 문자열을 숫자로 변환

    // 받은 값에 따라 속도와 동작 설정
    if (iValue > 10)
      speed = iValue; // 숫자가 10보다 크면 속도로 사용
    else
      num = iValue; // 그렇지 않으면 동작 번호로 사용
  }

  if (num == 10) { // 음주 측정 모드
    handleAlcoholMeasurement();
  } else {
    // RC 또는 라인트레이서 모드 실행
    if (num == 0) {
      lineTracerMode(); // 라인트레이서 모드
    } else {
      rcMode(); // RC 모드
    }
  }
}

int measureAlcohol() {
  return analogRead(mq3Pin); // MQ-3 센서 값 읽기
}

void displayOnLCD(const char* message) {
  // LCD 출력 코드 (LCD 라이브러리 초기화 및 사용 필요)
  Serial.println(message); // 현재는 Serial 출력으로 대체
}

void handleAlcoholMeasurement() {
  delay(3000); // 3초 대기

  int alcoholValue = measureAlcohol(); // 알코올 값 측정
  if (alcoholValue <= 400) {
    displayOnLCD("Normal");
  } else if (alcoholValue <= 699) {
    displayOnLCD("Warning");
  } else {
    displayOnLCD("License Revoke");
  }
}

void rcMode() {
 // 동작 설정
    if (num == 1) { // 앞 오
      analogWrite(11, speed);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, speed);
    } else if (num == 2) { // 앞
      analogWrite(11, speed);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, speed);
    } else if (num == 3) { // 앞 왼
      analogWrite(11, speed);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, speed);
    } else if (num == 4) { // 정지 왼
      analogWrite(11, speed);
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, LOW);
      analogWrite(6, speed);
    } else if (num == 5) { // 중지
      analogWrite(11, speed);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, speed);
    } else if (num == 6) { // 정지 오
      analogWrite(11, speed);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(8, LOW);
      digitalWrite(7, HIGH);
      analogWrite(6, speed);
    } else if (num == 7) { // 뒤 오
      analogWrite(11, speed);
      digitalWrite(10, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(7, HIGH);
      analogWrite(6, speed);
    } else if (num == 8) { // 후진 
      analogWrite(11, speed);
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(7, HIGH);
      analogWrite(6, speed);
    } else if (num == 9) { // 뒤 왼
      analogWrite(11, speed);
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(7, HIGH);
      analogWrite(6, speed);
    }
}

void lineTracerMode() {
  int leftValue = digitalRead(IR_LEFT);   // 왼쪽 센서 값 읽기
  int rightValue = digitalRead(IR_RIGHT); // 오른쪽 센서 값 읽기

  if (leftValue == 0 && rightValue == 0) { // 선이 가운데
    analogWrite(11, speed);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);

    analogWrite(6, speed);
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);

  } else if (leftValue == 1 && rightValue == 0) { // 선이 왼쪽
    analogWrite(11, speed);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
    analogWrite(6, speed);

    delay(500);
  } else if (leftValue == 0 && rightValue == 1) { // 선이 오른쪽
    analogWrite(11, speed);
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(6, speed);

    delay(500);
  } else if (leftValue == 1 && rightValue == 1){ // 선이 없을 경우 정지
    analogWrite(11, speed);
    analogWrite(6, speed);
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
  }
}
