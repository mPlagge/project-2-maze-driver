// library voor servo's
#include <Servo.h>

// Create objects for the weels
Servo ServoLeft;
Servo ServoRight;

// Servo pin's
#define IRPin D2
#define servoPinLeft D1
#define servoPinRight D3
#define redPin D4
#define greenPin D5
#define bluePin D6
#define LDRPin D1

bool frontIR;
bool rightIR;

/*************************************************************************/

void setup() {
  // Serial stuff
  //Serial.begin(9600);
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, HIGH);
  digitalWrite(bluePin, HIGH);
  // Bind servo objects to pins
  ServoRight.attach(servoPinRight);
  ServoLeft.attach(servoPinLeft);

  //find the first line
  if (!frontIR && !rightIR) {
    findRightLine();
  }
}
/*************************************************************************/


void loop() {
 /* if (!frontIR && !rightIR) {
    turnRight();
  }
  if (frontIR  && rightIR) {
    turnLeft();
  }
*/
  //wheels(1000);
  wheelsTurnRight(1000);

}

/*************************************************************************/

void turnLeft() {
  bool frontIR = infraRoodFront();
  bool rightIR = infraRoodRight();
  while (frontIR && rightIR) {
    wheelsTurnLeft(1);
  }
  wheelsDown(0);
}

void turnRight(){
  
}

// void's with a timer you can set

void wheelsDown(float timelapse) {
  Serial.println("weels down signal");
  ServoLeft.write(160);
  ServoRight.write(-160);
  delay(timelapse);
}

void wheelsStop(float timelapse) {
  Serial.println("weels stop signal");
  ServoLeft.write(90);
  ServoRight.write(90);
  delay(timelapse);
}

void wheelsUp(float timelapse) {
  Serial.println("weels up signal");
  ServoLeft.write(-160);
  ServoRight.write(160);
  delay(timelapse);
}

void wheelsTurnLeft(float timelapse) {
  Serial.println("Weels Turn Left");
  ServoLeft.write(160);
  ServoRight.write(160);
  delay(timelapse);
}

void wheelsTurnRight(float timelapse) {
  Serial.println("Weels Turn Right");
  ServoLeft.write(-160);
  ServoRight.write(-160);
  delay(timelapse);
}

bool infraRoodFront() {
  if (digitalRead(IRPin) == HIGH) {
    return true;
  } else {
    return false;
  }
}

bool infraRoodRight() {
  if (digitalRead(IRPin) == HIGH) {
    return true;
  } else {
    return false;
  }
}

/*void checkFrontColour() {
  bool green;
  bool red;
  bool blue;
  digitalWrite(greenPin, LOW);
  delay(10);
  green = digitalRead(LDRPin);
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
  delay(10);
  red = digitalRead(LDRPin);
  digitalWrite(redPin, HIGH);
  digitalWrite(bluePin, LOW);
  delay(10);
  blue = digitalRead(LDRPin);
  digitalWrite(bluePin, HIGH);

  if (!green && !blue && red) {
    return "red";
  } else if (!green && !blue && !red) {
    return "black";
  } else if (green && blue && red) {
    return "ground";
  } else {
    return "Unknown";
  }
}
*/
void findRightLine(){
  
}

