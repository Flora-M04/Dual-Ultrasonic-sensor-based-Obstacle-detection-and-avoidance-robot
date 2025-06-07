#include <ESP32Servo.h>

// Define ESP32 GPIO pins for Left Driver
const int enALeft = 23;
const int in1Left = 22;
const int in2Left = 21;
const int in3Left = 19;
const int in4Left = 18;
const int enBLeft = 5;

// Define ESP32 GPIO pins for Right Driver
const int enARight = 25;
const int in1Right = 26;
const int in2Right = 27;
const int in3Right = 14;
const int in4Right = 12;
const int enBRight = 13;

// Ultrasonic sensor (assume only front sensor is on servo)
#define TRIG_1 33  // front sensor
#define ECHO_1 32
#define TRIG_2 15  // rear sensor (fixed)
#define ECHO_2 2

// Servo
const int servoPin = 4;
Servo servo;

bool movingForward = true;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_1, OUTPUT);
  pinMode(ECHO_1, INPUT);
  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);

  pinMode(enALeft, OUTPUT);
  pinMode(in1Left, OUTPUT);
  pinMode(in2Left, OUTPUT);
  pinMode(in3Left, OUTPUT);
  pinMode(in4Left, OUTPUT);
  pinMode(enBLeft, OUTPUT);

  pinMode(enARight, OUTPUT);
  pinMode(in1Right, OUTPUT);
  pinMode(in2Right, OUTPUT);
  pinMode(in3Right, OUTPUT);
  pinMode(in4Right, OUTPUT);
  pinMode(enBRight, OUTPUT);

  servo.attach(servoPin);
  servo.write(0);
  delay(1000);
}

long readDistanceCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  return duration * 0.034 / 2;
}

// Motor control functions
void enableMotors() {
  digitalWrite(enALeft, HIGH);
  digitalWrite(enBLeft, HIGH);
  digitalWrite(enARight, HIGH);
  digitalWrite(enBRight, HIGH);
}

void moveForward() {
  enableMotors();
  digitalWrite(in1Left, HIGH);
  digitalWrite(in2Left, LOW);
  digitalWrite(in3Left, HIGH);
  digitalWrite(in4Left, LOW);

  digitalWrite(in1Right, HIGH);
  digitalWrite(in2Right, LOW);
  digitalWrite(in3Right, HIGH);
  digitalWrite(in4Right, LOW);

  movingForward = true;
}

void moveBackward() {
  enableMotors();
  digitalWrite(in1Left, LOW);
  digitalWrite(in2Left, HIGH);
  digitalWrite(in3Left, LOW);
  digitalWrite(in4Left, HIGH);

  digitalWrite(in1Right, LOW);
  digitalWrite(in2Right, HIGH);
  digitalWrite(in3Right, LOW);
  digitalWrite(in4Right, HIGH);

  movingForward = false;
}

void turnRight() {
  enableMotors();
  digitalWrite(in1Left, HIGH);
  digitalWrite(in2Left, LOW);
  digitalWrite(in3Left, HIGH);
  digitalWrite(in4Left, LOW);

  digitalWrite(in1Right, LOW);
  digitalWrite(in2Right, HIGH);
  digitalWrite(in3Right, LOW);
  digitalWrite(in4Right, HIGH);
}

void turnLeft() {
  enableMotors();
  digitalWrite(in1Left, LOW);
  digitalWrite(in2Left, HIGH);
  digitalWrite(in3Left, LOW);
  digitalWrite(in4Left, HIGH);

  digitalWrite(in1Right, HIGH);
  digitalWrite(in2Right, LOW);
  digitalWrite(in3Right, HIGH);
  digitalWrite(in4Right, LOW);
}

void stopMotors() {
  digitalWrite(enALeft, LOW);
  digitalWrite(enBLeft, LOW);
  digitalWrite(enARight, LOW);
  digitalWrite(enBRight, LOW);
}

// Main robot behavior
void loop() {
  if (movingForward) {
    long frontDistance = readDistanceCM(TRIG_1, ECHO_1);
    Serial.print("Front: ");
    Serial.println(frontDistance);

    if (frontDistance < 30) {
      stopMotors();
      delay(100);

      // Scan left and right using front sensor on servo
      servo.write(95);  // turn to right
      //delay(1);
        delay(1000);
        delay(1000);
 
       long rightDistance = readDistanceCM(TRIG_2, ECHO_2);

      // servo.write(180);  // turn to left
      // delay(800);
      long leftDistance = readDistanceCM(TRIG_1, ECHO_1);

      Serial.print("Right: ");
      Serial.println(rightDistance);
      Serial.print("Left: ");
      Serial.println(leftDistance);


      if (rightDistance < 30 && leftDistance > 30) {
        turnLeft();
        delay(1000);
      } 
      else if (leftDistance < 30 && rightDistance > 30) {
        turnRight();
        delay(1000);
      } 

      //prioritise the distance 
      else if (leftDistance > 30 && rightDistance > 30) {
        if (leftDistance > rightDistance) {
          turnLeft();
          delay(1000);
        } 
        else {
          turnRight();
          delay(1000);
        }
      } 
      else {
        servo.write(0);
        delay(1000);
        moveBackward();
        return;
      }

      stopMotors();
      servo.write(0);
      delay(1000);
      moveForward();
    } 
    else {
      moveForward();
    }
  }

  else {
    // Moving backward logic
    long backDistance = readDistanceCM(TRIG_2, ECHO_2);
    Serial.print("Back: ");
    Serial.println(backDistance);

    if (backDistance < 30) {
      stopMotors();
      delay(1000);

      servo.write(95);  // turn to right
       delay(3000);
      long rightDistance = readDistanceCM(TRIG_2, ECHO_2);

      // servo.write(180);  // turn to left
      // delay(800);
      long leftDistance = readDistanceCM(TRIG_1, ECHO_1);

      Serial.print("Right: ");
      Serial.println(rightDistance);
      Serial.print("Left: ");
      Serial.println(leftDistance);

      //delay(3000);

      if (rightDistance < 30 && leftDistance > 30) {
        turnLeft();
        delay(1000);
      } 
      else if (leftDistance < 30 && rightDistance > 30) {
        turnRight();
        delay(1000);
      }
       else if (leftDistance > 30 && rightDistance > 30) {
        if (leftDistance > rightDistance) {
          turnLeft();
          delay(1000);
        } 
        else {
          turnRight();
          delay(1000);
        }
      } 
      else {
        stopMotors();
        delay(1000);
        moveForward();
        return;
      }

      stopMotors();
      servo.write(0);
      delay(500);
      moveBackward();
    } 
    else {
      moveBackward();
    }
  }

  delay(300);
}
