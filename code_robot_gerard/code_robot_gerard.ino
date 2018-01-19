/* truth table
  red     black   ultra   IR1    IR2   IR3   Ir4
  1e time false   false   false  false false false    go forward
  2e time false   false   -      -     -     -        stop
  false   false   false   false  false false false    90 right then forward
  false   1 or 2  false   1 or 2 -     -     -        turn left
  false   false   false   false  true  true  true     go forward
  false   false   true    false  true  true  true     wait if nesesary turn 90 left
  false   false   false   false  false true  true     turn right


*/

// library voor servo's
#include <Servo.h>

// Create objects for the weels
Servo ServoLeft;
Servo ServoRight;

// Servo pin's
#define servoPinLeft 12
#define servoPinRight 14
#define waitTime 10 //time the robot waits when ultrasone is true. in s
#define turnTime 10 //time the robot turns whitout cheking sensors. in ms

bool red;
bool black;
bool ultrasone;
bool IR1;
bool IR2;
bool IR3;
bool IR4;
bool redFirst = true;
int timer = 0;

void setup() {
  // put your setup code here, to run once:
  // Serial stuff
  Serial.begin(9600);

  // Bind servo objects to pins
  ServoRight.attach(servoPinRight);
  ServoLeft.attach(servoPinLeft);
}

void loop() {
  // put your main code here, to run repeatedly:
  //check all sensors
  IR1 = infraRed1();
  IR2 = infraRed2();
  IR3 = infraRed3();
  IR4 = infraRed4();
  black = coulorBlack();
  red = coulorBlack();
  ultrasone = ultrasoneSens();

  //run sensor data through the if statments.
  ifCheck();
}

/*************************************************************************/

void ifCheck() {

  if (red) {
    if (redFirst) {
      if (!black and !ultrasone and !RI1 and !RI2 and !RI3 and !RI4) {
        weelsUp(turnTime);
        redFirst = false;
      }
    }
    if (!redFirst) {
      weelsStop(turnTime);
      while (1 == 1) {};
    }
  }
  if (!red and !black and !ultrasone and !RI1 and !RI2 and !RI3 and !RI4) {
    TurnRightStraight();
  }
  if (!red and !ultrasone and (black or RI1)) {
    weelsTurnLeft(turnTime);
  }
  if (!red and !black and !ultrasone and !RI1 and RI2 and RI3 and RI4) {
    weelsUp(turnTime);
  }
  if (!red and !black and ultrasone and !RI1 and RI2 and RI3 and RI4) {
    if (timer <= waitTime) {
      delay(1000);
      timer++;
    }
    else {
      TurnLeftStraight();
      timer = 0;
    }
  }
  //reset timer.
  if (!ultrasone) {
    timer = 0;
  }
  if (!red and !black and !ultrasone and !RI1 and !RI2 and RI3 and RI4) {
    weelsTurnRight(turnTime);
  }

}

// void's with a timer you can set

void weelsDowns(float timelapse) {
  Serial.println("weels up signal");
  ServoLeft.write(160);
  ServoRight.write(20);
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
  ServoLeft.write(20);
  ServoRight.write(160);
  delay(timelapse);
}

void weelsTurnLeft(float timelapse) {
  Serial.println("Weels Turn Left");
  ServoLeft.write(90);
  ServoRight.write(160);
  delay(timelapse);
}

void weelsTurnRight(float timelapse) {
  Serial.println("Weels Turn right");
  ServoLeft.write(160);
  ServoRight.write(90);
  delay(timelapse);
}

void TurnLeftStraight() {

}

void TurnRightStraight() {

}

bool infraRed1() {
  if (digitalRead(IRPin) == HIGH) {
    return true;
  } else {
    return false;
  }
}

bool infraRed2() {
  if (digitalRead(IRPin) == HIGH) {
    return true;
  } else {
    return false;
  }
}

bool infraRed3() {
  if (digitalRead(IRPin) == HIGH) {
    return true;
  } else {
    return false;
  }
}

bool infraRed4() {
  if (digitalRead(IRPin) == HIGH) {
    return true;
  } else {
    return false;
  }
}

bool coulorBlack() {
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
    return true;
  }
  else {
    return false;
  }
}
bool coulorRed() {
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
  if (red > 80 && blue < 20 && green < 20) {
    return true;
  }
  else {
    return false;
  }
}
bool ultrasoneSens() {
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
