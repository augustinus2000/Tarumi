/*
 * Tarumi Motor Controller
 *
 * Receives motion commands from the Raspberry Pi over USB serial.
 * Command format: <command>,<speed>\n
 *
 * Commands:
 *   f : forward
 *   b : backward
 *   l : turn left
 *   r : turn right
 *   s : stop
 *   z : forward-left
 *   x : forward-right
 *   c : backward-left
 *   v : backward-right
 */

const int LPWM1 = 5;
const int RPWM1 = 6;

const int LPWM2 = 9;
const int RPWM2 = 10;

void setup() {
  // 핀 설정
  pinMode(LPWM1, OUTPUT);
  pinMode(RPWM1, OUTPUT);

  pinMode(LPWM2, OUTPUT);
  pinMode(RPWM2, OUTPUT);

  Serial.begin(9600);  // USB serial communication with Raspberry Pi
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');  // '\n'으로 끝나는 문자열 읽기
    
    // 첫 번째 쉼표 위치 찾기
    int firstCommaIndex = command.indexOf(',');

    // 쉼표 있는지 확인
    if (firstCommaIndex != -1) { // 쉼표 위치가 찾아졌으면
      String charSignal = command.substring(0, firstCommaIndex);  // 첫 번째 쉼표 앞 문자
      String numSignal1 = command.substring(firstCommaIndex + 1); // 첫 번째 쉼표 뒤 숫자
      
      char receivedChar = charSignal.charAt(0); // 문자로 변환
      int receivedNum1 = numSignal1.toInt();    // 첫 번째 숫자 변환

    // 문자에 따라 모터 제어
    if (receivedChar == 'f') {  // 전진
    moveForward(receivedNum1);
    }
    else if (receivedChar == 'b') {  // 후진
    moveBackward(receivedNum1);
    }
    else if (receivedChar == 'l') {  // 좌회전
    turnLeft(receivedNum1);
    }
    else if (receivedChar == 'r') {  // 우회전
    turnRight(receivedNum1);
    }
    else if (receivedChar == 's') {  // 정지
    stopMotors();
    }
    else if (receivedChar == 'z') {  // 전진 + 좌회전
    forwardLeft(receivedNum1);
    }
    else if (receivedChar == 'x') {  // 전진 + 우회전
    forwardRight(receivedNum1);
    }
    else if (receivedChar == 'c') {  // 후진 + 좌회전
    backwardLeft(receivedNum1);
    }
    else if (receivedChar == 'v') {  // 후진 + 우회전
    backwardRight(receivedNum1);
    }
    else {
    stopMotors();
      }
    }
  }
}

// 전진
void moveForward(int speed) {
  analogWrite(LPWM1, speed);      // 모터1 전진 속도 설정
  analogWrite(RPWM1, LOW);        // 모터1 후진 x
  analogWrite(LPWM2, speed);      // 모터2 전진 속도 설정
  analogWrite(RPWM2, LOW);        // 모터2 후진 x
}

// 후진
void moveBackward(int speed) {
  analogWrite(LPWM1, LOW);        // 모터1 전진 x
  analogWrite(RPWM1, speed);      // 모터1 후진 속도 설정
  analogWrite(LPWM2, LOW);        // 모터2 전진 x
  analogWrite(RPWM2, speed);      // 모터2 후진 속도 설정
}

// 정지 + 좌회전
void turnLeft(int speed) {
  analogWrite(LPWM1, speed);  // 모터1 느리게 전진
  analogWrite(RPWM1, LOW);    // 모터1 후진 x
  analogWrite(LPWM2, LOW);    // 모터2 전진 x
  analogWrite(RPWM2, LOW);    // 모터2 후진 x
}

// 정지 + 우회전
void turnRight(int speed) {
  analogWrite(LPWM1, LOW);      // 모터1 전진 x
  analogWrite(RPWM1, LOW);      // 모터1 후진 x
  analogWrite(LPWM2, speed);    // 모터2 느리게 전진
  analogWrite(RPWM2, LOW);      // 모터2 후진 x
}

// 정지
void stopMotors() {
  analogWrite(LPWM1, LOW);   // 모터1 전진 x
  analogWrite(RPWM1, LOW);   // 모터1 후진 x
  analogWrite(LPWM2, LOW);   // 모터2 전진 x
  analogWrite(RPWM2, LOW);   // 모터2 후진 x
}

// 전진 + 좌회전
void forwardLeft(int speed) {
  analogWrite(LPWM1, speed);      // 오른쪽 바퀴 빠르게 전진
  analogWrite(RPWM1, LOW);              
  analogWrite(LPWM2, speed-10); // 왼쪽 바퀴 느리게 전진
  analogWrite(RPWM2, LOW);
}

// 전진 + 우회전
void forwardRight(int speed) {
  analogWrite(LPWM1, speed-10);   // 오른쪽 바퀴 느리게 전진
  analogWrite(RPWM1, LOW);              
  analogWrite(LPWM2, speed);    // 왼쪽 바퀴 빠르게 전진
  analogWrite(RPWM2, LOW);
}

// 후진 + 좌회전
void backwardLeft(int speed) {
  analogWrite(LPWM1, LOW);
  analogWrite(RPWM1, speed);       // 오른쪽 바퀴 빠르게 후진
  analogWrite(LPWM2, LOW);
  analogWrite(RPWM2, speed-10);  // 왼쪽 바퀴 느리게 후진
}

// 후진 + 우회전
void backwardRight(int speed) {
  analogWrite(LPWM1, LOW);
  analogWrite(RPWM1, speed-10);    // 오른쪽 바퀴 느리게 후진
  analogWrite(LPWM2, LOW);
  analogWrite(RPWM2, speed);     // 왼쪽 바퀴 빠르게 후진
}
