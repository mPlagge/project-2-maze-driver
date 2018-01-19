//Set debug mode/Serial.print on or off
bool debugMode = false;
int timeToTurn90Degrees = 6000;
bool lineDetected = false;
// library voor servo's
#include <Servo.h>

// Create objects for the weels
Servo ServoLeft;
Servo ServoRight;

// Servo pin's
#define IRPinRight D0
#define IRPinFront D1
#define servoPinLeft D2
#define servoPinRight D3
#define redPin D4
#define greenPin D5
#define bluePin D6
#define LDRPin D7
#define trigPin D8
#define echoPin D9

/*************************************************************************/

void setup() {
  // Serial stuff
  if (debugMode) {
    Serial.begin(9600);
  }

  //Set all the pin modes
  pinMode(IRPinRight, INPUT);
  pinMode(IRPinRight, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(LDRPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Bind servo objects to pins
  ServoRight.attach(servoPinRight);
  ServoLeft.attach(servoPinLeft);

  //Turn the LED's off
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, HIGH);
  digitalWrite(bluePin, HIGH);

  wheelsTurnRight(0);
  int startTime = millis();
  while (millis() < (startTime + timeToTurn90Degrees)){
    if(!infraRoodFront()){
      lineDetected = true;
      break;
    }
  }
  if (lineDetected){
    turnLeft();
  } else {
    wheelsDown(0);
    while(infraRoodFront()){
      //rechtdoorrijden tot er een lijn is gevonden
    }
    turnLeft();
  }
}
/*************************************************************************/


void loop() {

  if (!infraRoodFront() && !infraRoodRight()) {
    turnLeft();
  }
  if (infraRoodFront()) {
    turnLeft();
  }
  if (checkFrontColour() == 'r') {
    commenceDancing();
  }

  if (ultrasoon(10)) {
    avoidObstacle();
  }

  wheelsDown(100);

}

/*************************************************************************/
void avoidObstacle() {
  wheelsStop(0);
  int startTimer = millis();
  while (millis() < (startTimer + 5000)) {
    if (!ultrasoon(10)) {
      if (!ultrasoon(10)) {
        return;
      }
    }
  }
  wheelsTurnLeft(6000);//draai 180 graden
  wheelsDown(0);
  while (millis() < (startTimer + 5000)) {
    if (ultrasoon(10)) {
      avoidObstacle();
    }
  }
  wheelsTurnLeft(6000);
}


void turnLeft() {
  while (infraRoodFront()) {
    wheelsTurnLeft(0);
  }
  wheelsDown(0);
}

void turnRight() {
  while (!infraRoodFront()) {
    wheelsTurnRight(0);
  }
  while (infraRoodFront()) {
    wheelsTurnLeft(0);
  }
  wheelsDown(0);
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
  if (digitalRead(IRPinFront) == HIGH) {
    return true; //grond
  } else {
    return false; //lijn
  }
}

bool infraRoodRight() {
  if (digitalRead(IRPinRight) == HIGH) {
    return true; //grond
  } else {
    return false; //lijn
  }
}

void commenceDancing() {
  wheelsTurnRight(1);
  while (true) {
    //ENDLESS LOOP
  }
}

char checkFrontColour() {
  digitalWrite(greenPin, LOW);
  delay(10);
  bool green = digitalRead(LDRPin);
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
  delay(10);
  bool red = digitalRead(LDRPin);
  digitalWrite(redPin, HIGH);
  digitalWrite(bluePin, LOW);
  delay(10);
  bool blue = digitalRead(LDRPin);
  digitalWrite(bluePin, HIGH);

  if (!green && !blue && red) {
    return 'r'; //Rood
  } else if (!green && !blue && !red) {
    return 'z'; //Zwart
  } else if (green && blue && red) {
    return 'w'; //Wit
  } else {
    return 'u'; //Unknown
  }
}

bool ultrasoon(int detecteerAfstandInCentimeter) {
  long duratie;
  long cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duratie = pulseIn(echoPin, HIGH);

  // convert the time into a distance by dividing the duration by the speed of sound in cm/μs (29cm/μs).
  //Then devide by two because the soudwave has to travel the same distance twic (foward and back)
  cm = duratie / 29 / 2;

  if (cm < detecteerAfstandInCentimeter) {
    return true; //returns true if object is closer then distance in the parameter
  } else {
    return false; //return false if object is further away then distance in the parameter
  }
}

