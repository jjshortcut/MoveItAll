#include <Servo.h>

Servo myservo; 

// Pins
const int analogInPin = A0;
const int servoPin = 9;

int pos = 1000;    // variable to store the servo position
int current = 0;
char received;

void setup() {
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  //Serial.begin(9600);
  //Serial.println("MoveItAll Servo movement test");
}

void loop() {
  /*received = Serial.read();
  switch (received) 
  {
    case '+':
      pos+=100;
      break;
    case '-':
      pos-=100;
      break;
    case 'r':
      pos = 0;
      break;
    case 'm':
      pos = 180;
      break;
    case 's':
      sweeptest();
      break;
    default: 
    break;
  }*/

  myservo.writeMicroseconds(pos);
  //myservo.write(pos); 

  //sweeptest();
  
  //current = analogRead(analogInPin);
  //Serial.println(String(pos) + mean());
  Serial.println("Pos=" + String(pos) + " Cur=" + String(mean()));
  delay(15);
}

int mean(void)
{
  uint8_t avg_num = 20;
  int mean = 0;
  for(uint8_t i=0; i<avg_num; i++)
  {
   mean += analogRead(analogInPin); 
  }
  mean = mean/avg_num;
  //Serial.println(mean);

  return mean;
}

void sweeptest(void)
{
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    mean();
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    mean();
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

