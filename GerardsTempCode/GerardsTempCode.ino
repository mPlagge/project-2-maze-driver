
// library voor servo's
#include <Servo.h>

#define S0 16
#define S1 5
#define S2 4
#define S3 0
#define sensorOut 2

#define infra1 14
#define infra2 12
#define infra3 13
#define infra4 15

#define trigPin A0
#define echoPin 10

int frequency = 0;

// Create objects for the weels
Servo ServoLeft;
Servo ServoRight;

// Servo pin's
#define servoPinLeft 3
#define servoPinRight 1

bool colorBlack;
bool colorRed;
int IRsensors[4] = { 14, 12, 13, 15 };
bool IRvalues[4];
bool objectClose;

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

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Bind servo objects to pins
  ServoRight.attach(servoPinRight);
  ServoLeft.attach(servoPinLeft);
}

/*************************************************************************/


void loop() {
  // ga naar beneden 100 mili seconden
  weelsDowns(100);
}

/*************************************************************************/

void getSensorInformation() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Reading the output frequency
  int red = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  int green = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  int blue = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  delay(100);
  if (red < 20 && blue < 20 && green < 20) {
    colorBlack = true;
    colorRed = false;
  }
  else if (red > 80 && blue < 20 && green < 20) {
    colorBlack = false;
    colorRed = true;
  }
  else {
    colorBlack = false;
    colorRed = false;
  }

  for (int i = 0; i < 4; i++) {
    if (digitalRead(IRsensors[i]) == HIGH) {
      IRvalues[i] = true;
    }
    else {
      IRvalues[i] = false;
    }
  }

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
    objectClose = true; //returns true if object is closer then distance in the parameter
  } else {
    objectClose = false; //return false if object is further away then distance in the parameter
  }
}

// void's with a timer you can set

void weelsDowns(float timelapse) {
  Serial.println("weels up signal");
  ServoLeft.write(160);
  ServoRight.write(-160);
  delay(timelapse);
}

void weelsStop(float timelapse) {
  Serial.println("weels stop signal");
  ServoLeft.write(90);
  ServoRight.write(90);
  delay(timelapse);
}

void weelsUp(float timelapse) {
  Serial.println("weels up signal");
  ServoLeft.write(-160);
  ServoRight.write(160);
  delay(timelapse);
}

void weelsTurnLeft(float timelapse) {
  Serial.println("Weels Turn Left");
  ServoLeft.write(90);
  ServoRight.write(360);
  delay(timelapse);
}
