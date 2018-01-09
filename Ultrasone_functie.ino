/* HC-SR04 Sensor
   https://www.dealextreme.com/p/hc-sr04-ultrasonic-sensor-distance-measuring-module-133696

   This sketch reads a HC-SR04 ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse
   to return.  The length of the returning pulse is proportional to
   the distance of the object from the sensor.

   The circuit:
    VCC connection of the sensor attached to +5V
    GND connection of the sensor attached to ground
    TRIG connection of the sensor attached to digital pin 2
    ECHO connection of the sensor attached to digital pin 4
   Original code for Ping))) example was created by David A. Mellis
   Adapted for HC-SR04 by Tautvidas Sipavicius
   This example code is in the public domain.
*/

#define trigPin 8
#define echoPin 10

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop(){
  bool test;
  test = ultrasoon(10);
  Serial.println(test);
}

bool ultrasoon(int detecteerAfstandInCentimeter) { //
  // establish variables for duration of the ping,
  // and the distance result in centimeters:
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


