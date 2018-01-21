#define BACKWARD 0
#define FORWARD 1

#define CURR_MAX 55

int dirA = 2;
int speedA = 3;

int button1 = 6;  // not working buttons
int button2 = 7;

char received;
int current = 0;
int current_max = 0;
uint8_t curr_trip = 0;

void setup()
{
  pinMode (dirA, OUTPUT);
  pinMode (speedA, OUTPUT);
  digitalWrite(dirA, LOW);
  digitalWrite(speedA, LOW);

  Serial.begin(9600);
  Serial.println("Start motor test");
  Serial.println("Open = '+'");
  Serial.println("Close = '-'");
  Serial.println("Info = 'i'");
}

void loop()
{
  //Serial.println("Current = " + String(analogRead(A0)));
  Serial.println(avg());

  //Serial.println("Angle = " + String(analogRead(A1)));
  
  received = Serial.read();
  switch (received) 
  {
    case '+':
    Serial.println("Forward");
      motor(FORWARD, 255, 100);
      break;
    case '-':
      Serial.println("Backward");
      motor(BACKWARD, 255, 100);
      break;
    case 's':
    Serial.println("Stop");
    motor(FORWARD, 0, 500);
    break;
    case 'i':
    current_max = map(analogRead(A1), 0, 1023, 0, 100);
    current = analogRead(A0);
    Serial.println(String(current_max) + "," + String(current));
    break;
    default: 
    break;
  }
  received = 0;

  delay(10);
}

void motor(uint8_t dir, uint8_t speed, int ms)
{
  digitalWrite (dirA, dir);
  for(int i=0; i<speed; i++)
  {
    analogWrite (speedA,i);
    delayMicroseconds(500);
  }
   
  for(int i=0; i<ms; i++)
  {
    //current_max = analogRead(A1);
    current_max = map(analogRead(A1), 0, 1023, 0, 100);
    current = analogRead(A0);
    Serial.print(String(current_max) + "," + String(current));
    if(curr_trip)
    {
      Serial.println(",TRIP!");
    }
    else
    {
      Serial.println
      (",OK");
    }
    //if(current>CURR_MAX)
    if(current>current_max)
    {
      curr_trip = 1;
      digitalWrite (speedA, LOW);
      //Serial.println("Current too high!");
    }
    delay(1);
  }
  digitalWrite (speedA, LOW);
  curr_trip = 0;
}

int avg(void)
{
  int avg = 0;
  uint8_t nr = 10;
  for(uint8_t i; i<nr; i++)
  {
    avg += analogRead(A0);
  }
  return avg/nr;
}

