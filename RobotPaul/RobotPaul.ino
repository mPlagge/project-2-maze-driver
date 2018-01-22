//Set debug mode/Serial.print on or off
bool debugMode = true;
int timeToTurn90Degrees = 6000;
bool lineDetected = false;
bool finished = false;
// library voor servo's
#include <Servo.h>

// Create objects for the weels
Servo ServoLeft;
Servo ServoRight;

// Servo pin's
#define IRPinLeft D6
#define IRPinFront D5
#define servoPinLeft D8
#define servoPinRight D9
#define redPin D4
#define greenPin D2
#define bluePin D3
#define LDRPin D7
#define trigPinUS D0
#define echoPinUS D1

/*************************************************************************/

void setup() {
  // Serial stuff
  if (debugMode) {
    Serial.begin(9600);
  }

  //Set all the pin modes
  pinMode(IRPinLeft, INPUT);
  pinMode(IRPinFront, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(LDRPin, INPUT);
  //pinMode(trigPin, OUTPUT);
  //pinMode(echoPin, INPUT);
  // Bind servo objects to pins
  ServoRight.attach(servoPinRight);
  ServoLeft.attach(servoPinLeft);

  //Turn the LED's off
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, HIGH);
  digitalWrite(bluePin, HIGH);

  /* if (!InfraRoodLeft()) {
     wheelsTurnRight(0);
     int startTime = millis();
     while (millis() < (startTime + timeToTurn90Degrees)) {
       if (!infraRoodFront()) {
         lineDetected = true;
         break;
       }
     }
     if (lineDetected) {
       turnLeft();
     } else {
       wheelsDown(0);
       while (infraRoodFront()) {
         //rechtdoorrijden tot er een lijn is gevonden
       }
       turnLeft();
     }
    }*/
}
/*************************************************************************/


void loop() {
  if (!finished) {
    if (infraRoodFront()) {                  //Als voor een zwarte lijn ziet
      turnRight();                           //ga dan naar rechts
    }
    if (!infraRoodLeft() && !infraRoodFront()) { //als links en links wit zien
      turnLeft();                             //ga dan naar links
    }
    if (ultrasoon(10)) {
      avoidObstacle();
    } else {
      wheelsDown(10);
    }
    if (checkFrontColour() == 'r') {
      Serial.println("ROOD!");
      finished = true;
      wheelsStop(0);
    }
  }
}

/*************************************************************************/
void avoidObstacle() {
  wheelsStop(0);
  int startTimer = millis();
  while (millis() < (startTimer + 5000)) {
    delay(10);
  }

  wheelsTurnRight(3006);
  while (!infraRoodFront()) {
    wheelsDown(0);
  }
}


void turnRight() {
  while (infraRoodFront()) {//zolang fornt zwart is ga naar rechts
    wheelsTurnRight(0);
  }
  wheelsDown(0);
}

void turnLeft() {
  wheelsUp(750);
  while (!infraRoodFront()) {
    wheelsTurnLeft(600);
    wheelsUp(50);
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
  ServoLeft.write(50);
  ServoRight.write(50);
  delay(timelapse);
}

void wheelsTurnRight(float timelapse) {
  Serial.println("Weels Turn Right");
  ServoLeft.write(160);
  ServoRight.write(160);
  delay(timelapse);
}

bool infraRoodFront() {
  if (digitalRead(IRPinFront) == HIGH) {
    Serial.println("Front true");
    return true; //lijn is true
  } else {
    Serial.println("Front false");

    return false; //grond is false
  }

}

bool infraRoodLeft() {
  if (digitalRead(IRPinLeft) == HIGH) {
    return true; //lijn is true
  } else {
    return false; //grond is false
  }
  Serial.println("Left OOPS");
}

void commenceDancing() {
  wheelsTurnRight(1);
}

char checkFrontColour() {
  digitalWrite(greenPin, LOW);
  delay(50);
  bool green = digitalRead(LDRPin);
  Serial.print("green = ");
  Serial.println(green);
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
  delay(50);
  bool red = digitalRead(LDRPin);
  Serial.print("Red = ");
  Serial.println(red);
  digitalWrite(redPin, HIGH);
  digitalWrite(bluePin, LOW);
  delay(50);
  bool blue = digitalRead(LDRPin);
  Serial.print("blue = ");
  Serial.println(blue);
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
  int duration, distanceUS;
  pinMode(trigPinUS, OUTPUT);
  digitalWrite(trigPinUS, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinUS, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinUS, LOW);
  pinMode(echoPinUS, INPUT);
  duration = pulseIn(echoPinUS, HIGH);
  distanceUS = microsecondsToCentimeters(duration);

  Serial.println(distanceUS);
  Serial.print("cm");
  Serial.println();
  if (distanceUS < detecteerAfstandInCentimeter) {
    return true;
  } else {
    return false;
  }
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

