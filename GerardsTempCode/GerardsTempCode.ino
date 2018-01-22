
// library voor servo's
#include <Servo.h>

#define S0 8   // 8 16
#define S1 9   // 9 5
#define S2 10  // 10 4
#define S3 11  // 11 0
#define sensorOut 12 // 12 2 

#define infra1 2 // 2 14
#define infra2 3 // 3 12
#define infra3 4 // 4 13
#define infra4 5 // 5 15

#define trigPin 22 // 22 A0
#define echoPin 24 // 24 10

int frequency = 0;

// Create objects for the weels
Servo ServoLeft;
Servo ServoRight;

// Servo pin's
#define servoPinLeft 3   // 28 3
#define servoPinRight 1  // 30 1

bool colourBlack;
bool colourRed;
int IRsensors[4] = { 2, 3, 4, 5 };
bool IRvalues[4];
bool objectClose;
int red;
int blue;
int green;
long cm;
bool redFirst = true;
int turnTime = 150;
int waitTime = 10;
int timer = 0;

bool alreadyTurned = false;

int detecteerAfstandInCentimeter = 20;

/*************************************************************************/

void setup() {
  // Serial stuff
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  pinMode(IRsensors[0], INPUT);
  pinMode(IRsensors[1], INPUT);
  pinMode(IRsensors[2], INPUT);
  pinMode(IRsensors[3], INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Bind servo objects to pins
  ServoRight.attach(servoPinRight);
  ServoLeft.attach(servoPinLeft);
}

/*************************************************************************/


void loop() {
  // ga naar beneden 100 mili seconden
  getSensorData();
  printSensorData();
  determineNextMove();
}

/*************************************************************************/

void determineNextMove() {
  if (colourRed) {
    if (redFirst) {
     // if (!colourBlack and !objectClose and !IRvalues[0] and !IRvalues[1] and !IRvalues[2] and !IRvalues[3]) {
        weelsForward(turnTime);
        getSensorData();
        if(!colourRed){
          redFirst = false;
        }
      //}
    }
    else if (!redFirst) {
      getSensorData();
      if(colourRed){
        weelsStop();
        while (true){};
      }
    }
  }
  if (!colourRed and !colourBlack and !objectClose and !IRvalues[0] and !IRvalues[1] and !IRvalues[2] and !IRvalues[3]) {
    if(!alreadyTurned){
      weelsTurnRightStraight();
      alreadyTurned = true;
    }
    else{
      weelsTurnRight(turnTime);
    }
  }
  if (!colourRed and !objectClose and (colourBlack or IRvalues[0])) {
    weelsTurnLeft(turnTime);
  }
  if (!colourRed and !colourBlack and !objectClose and !IRvalues[0] and IRvalues[1] and (IRvalues[2] or IRvalues[3])) {
    weelsForward(turnTime);
  }
  if(!colourRed and !colourBlack and !objectClose and !IRvalues[0] and IRvalues[1]){
    weelsForward(turnTime);
  }
  if (objectClose) {
    if (timer <= waitTime) {
      weelsStop();
      delay(1000);
      timer++;
    }
    else {
      weelsTurnLeftStraight();
      alreadyTurned = true;
      timer = 0;
    }
  }
  //reset timer.
  if (!objectClose) {
    timer = 0;
  }
  if (!colourRed and !colourBlack and !objectClose and !IRvalues[0] and !IRvalues[1] and (IRvalues[2] or IRvalues[3])) {
    weelsTurnRight(turnTime);
  }
}

void getSensorData() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Reading the output frequency
  red = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  green = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  blue = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  delay(100);
  if (red > 200 && blue > 200 && green > 200) {
    colourBlack = true;
    colourRed = false;
  }
  else if (red < 60 && blue >= 90 && green >= 90) {
    colourBlack = false;
    colourRed = true;
  }
  else {
    colourBlack = false;
    colourRed = false;
  }

  for (int i = 0; i < 4; i++) {
    if (digitalRead(IRsensors[i]) != HIGH) {
      IRvalues[i] = false;
    }
    else {
      IRvalues[i] = true;
    }
  }

  long duratie;

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
    objectClose = true; //returns true if object is closer then distance in the parameter
  } else {
    objectClose = false; //return false if object is further away then distance in the parameter
  }
}

void printSensorData() {
  Serial.print("Red: ");
  Serial.println(red);
  Serial.print("Blue: ");
  Serial.println(blue);
  Serial.print("Green: ");
  Serial.println(green);
  Serial.print("colourBlack: ");
  Serial.println(colourBlack);
  Serial.print("colourRed:   ");
  Serial.println(colourRed);
  Serial.print("Distance: ");
  Serial.println(cm);
  Serial.print("IR1: ");
  Serial.println(IRvalues[0]);
  Serial.print("IR2: ");
  Serial.println(IRvalues[1]);
  Serial.print("IR3: ");
  Serial.println(IRvalues[2]);
  Serial.print("IR4: ");
  Serial.println(IRvalues[3]);

}

// void's with a timer you can set

void weelsBackwards(float timelapse) {
  Serial.println("weels up signal");
  ServoLeft.writeMicroseconds(1470);
  ServoRight.writeMicroseconds(1600);
  delay(timelapse);
  weelsStop();
}

void weelsStop() {
  Serial.println("weels stop signal");
  ServoLeft.write(1500);
  ServoRight.write(1500);
}

void weelsForward(float timelapse) {
  Serial.println("weels up signal");
  ServoLeft.writeMicroseconds(1530);
  ServoRight.writeMicroseconds(1400);
  delay(timelapse);
  weelsStop();
}

void weelsTurnLeft(float timelapse) {
  Serial.println("Weels Turn Left");
  ServoLeft.writeMicroseconds(1470);
  ServoRight.writeMicroseconds(1400);
  delay(timelapse*2);
  weelsStop();
}

void weelsTurnRight(float timelapse) {
  Serial.println("Weels Turn Right");
  ServoRight.writeMicroseconds(1600);
  ServoLeft.writeMicroseconds(1600);
  delay(timelapse*2);
  weelsStop();
}

void weelsTurnRightStraight() {
  Serial.println("Weels Turn Right Straight");
  ServoRight.writeMicroseconds(1600);
  ServoLeft.writeMicroseconds(1560);
  delay(1200);
  weelsStop();
}

void weelsTurnLeftStraight() {
  Serial.println("Weels Turn Left Straight");
  ServoLeft.writeMicroseconds(1470);
  ServoRight.writeMicroseconds(1300);
  delay(1200);
  weelsStop();
}
