
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
#define servoPinLeft 3
#define servoPinRight 1

bool colorBlack;
bool colorRed;
int IRsensors[4] = { 2, 3, 4, 5 };
bool IRvalues[4];
bool objectClose;
int red;
int blue;
int green;
long cm;

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

  pinMode(infra1, INPUT);
  pinMode(infra2, INPUT);
  pinMode(infra3, INPUT);
  pinMode(infra4, INPUT);

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
}

/*************************************************************************/

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
  if (red > 200 && blue > 20 && green > 20) {
    colorBlack = true;
    colorRed = false;
  }
  else if (red < 50 && blue > 100 && green > 100) {
    colorBlack = false;
    colorRed = true;
  }
  else {
    colorBlack = false;
    colorRed = false;
  }

  for (int i = 0; i < 4; i++) {
    if (digitalRead(IRsensors[i]) != HIGH) {
      IRvalues[i] = true;
    }
    else {
      IRvalues[i] = false;
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

void printSensorData(){
  Serial.print("Red: ");
  Serial.println(red);
  Serial.print("Blue: ");
  Serial.println(blue);
  Serial.print("Green: ");
  Serial.println(green);
  Serial.print("colorBlack: ");
  Serial.println(colorBlack);
  Serial.print("colorRed: ");
  Serial.println(colorRed);
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
