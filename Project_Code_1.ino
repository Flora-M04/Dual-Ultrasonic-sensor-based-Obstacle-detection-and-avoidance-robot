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


//ultrasonic sensors
#define TRIG_1 33  //red, orange
#define ECHO_1 32
#define TRIG_2 15  //blue, purple
#define ECHO_2 2
float duration_us1, distance_cm1;
float duration_us2, distance_cm2;

//servo motor
const int servoPin = 4;
Servo servo;
int servoAngle = 0;  // 0 = front/back, 90 = left/right

bool movingForward = true;  // moving forward variable


void setup() {
  Serial.begin(115200);

  //ultrasonic sensors
  pinMode(TRIG_1, OUTPUT);
  pinMode(ECHO_1, INPUT);
  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);

  //dc motors
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


//functions for ultrasonic sensor control
long readDistanceCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

//functions for motor and robot direction control
void moveForward() {
  analogWrite(enALeft, 255);  // 0-255 for PWM speed control
  analogWrite(enBLeft, 255);
  digitalWrite(in1Left, HIGH);
  digitalWrite(in2Left, LOW);
  digitalWrite(in3Left, HIGH);
  digitalWrite(in4Left, LOW);

  analogWrite(enARight, 255);
  analogWrite(enBRight, 255);
  digitalWrite(in1Right, HIGH);
  digitalWrite(in2Right, LOW);
  digitalWrite(in3Right, HIGH);
  digitalWrite(in4Right, LOW);

  movingForward = true;
}

void moveBackward() {
  analogWrite(enALeft, 255);
  analogWrite(enBLeft, 255);
  digitalWrite(in1Left, LOW);
  digitalWrite(in2Left, HIGH);
  digitalWrite(in3Left, LOW);
  digitalWrite(in4Left, HIGH);

  analogWrite(enARight, 255);
  analogWrite(enBRight, 255);
  digitalWrite(in1Right, LOW);
  digitalWrite(in2Right, HIGH);
  digitalWrite(in3Right, LOW);
  digitalWrite(in4Right, HIGH);

  movingForward = false;
}

void turnRight() {
  analogWrite(enALeft, 200);
  analogWrite(enBLeft, 200);
  digitalWrite(in1Left, HIGH);
  digitalWrite(in2Left, LOW);
  digitalWrite(in3Left, HIGH);
  digitalWrite(in4Left, LOW);

  analogWrite(enARight, 200);
  analogWrite(enBRight, 200);
  digitalWrite(in1Right, LOW);
  digitalWrite(in2Right, HIGH);
  digitalWrite(in3Right, LOW);
  digitalWrite(in4Right, HIGH);
}

void turnLeft() {
  analogWrite(enALeft, 200);
  analogWrite(enBLeft, 200);
  digitalWrite(in1Left, LOW);
  digitalWrite(in2Left, HIGH);
  digitalWrite(in3Left, LOW);
  digitalWrite(in4Left, HIGH);

  analogWrite(enARight, 200);
  analogWrite(enBRight, 200);
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


// intructions to the robot
void loop() {
  ///delay(2000);
  if (movingForward) {
    long frontDistance = readDistanceCM(TRIG_1, ECHO_1);
    Serial.print("Front: ");
    Serial.println(frontDistance);

    if (frontDistance < 30) {
      stopMotors();
      delay(1000);

      // Scan left and right
      servo.write(90);  // Rotate to side view

      long rightDistance = readDistanceCM(TRIG_2, ECHO_2);
      long leftDistance = readDistanceCM(TRIG_1, ECHO_1);
      Serial.print("Right: ");
      Serial.println(rightDistance);
      Serial.print("Left: ");
      Serial.println(leftDistance);
      delay(4000);  // delay(3000);

      if (rightDistance < 30 && leftDistance > 30) {
        turnLeft();
        delay(2000);  //subject to change

      } 
      else if (leftDistance < 30 && rightDistance > 30) {
        turnRight();
        delay(2000);
      }

      else if (leftDistance > 30 && rightDistance > 30) {
        // side directions clear, check for better side
        if (leftDistance > rightDistance) {
          turnLeft();
          delay(2000);
        } 
        else if (leftDistance < rightDistance) {
          turnRight();
          delay(2000);
        }
      } 

      else if (leftDistance < 30 && rightDistance < 30) {
        // All directions blocked, go backward
        //delay(2000);
        servo.write(0);
        delay(2000);
        moveBackward();
        return;
      }

      stopMotors();
      servo.write(0);
      delay(2000);
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

      servo.write(90);

      long rightDistance = readDistanceCM(TRIG_2, ECHO_2);
      long leftDistance = readDistanceCM(TRIG_1, ECHO_1);
      Serial.print("Right: ");
      Serial.println(rightDistance);
      Serial.print("Left: ");
      Serial.println(leftDistance);
      delay(4000);

      if (rightDistance < 30 && leftDistance > 30) {
        turnLeft();
        delay(2000);
      } 
      else if (leftDistance < 30 && rightDistance > 30) {
        turnRight();
        delay(2000);
      } 
      else if (leftDistance > 30 && rightDistance > 30) {
        if (leftDistance > rightDistance) {
          turnLeft();
          delay(2000);
        } 
        else {
          turnRight();
          delay(2000);
        }
      } 
      else if(leftDistance < 30 && rightDistance < 30){
       // stopMotors();
       
        delay(3000);
        return;
      }

      stopMotors();
      servo.write(0);
      delay(1000);
      moveBackward();
    } 
    else {
      moveBackward();
    }
  }

  delay(1000);
}