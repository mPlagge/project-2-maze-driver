
// library voor servo's
#include <Servo.h>

// Create objects for the weels
Servo ServoLeft;
Servo ServoRight;

// Servo pin's
#define servoPinLeft D1
#define servoPinRight D3

/*************************************************************************/

void setup() {
    // Serial stuff
    //Serial.begin(9600);

    // Bind servo objects to pins
    ServoRight.attach(servoPinRight);
    ServoLeft.attach(servoPinLeft);
}

/*************************************************************************/


void loop() {
  // ga naar beneden 100 mili seconden
  weelsDowns(1000);
  weelsStop(1000);
}

/*************************************************************************/

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

// Gerard test even of git werkt lol squadfam, Pollution

