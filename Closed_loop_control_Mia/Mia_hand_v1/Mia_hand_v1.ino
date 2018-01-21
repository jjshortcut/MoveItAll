/*
 * MoveItAll closed loop control for the smart brace
 * 
 * JJS @ ProtoSpace 2017
 */

#define DEBUG // uncomment to use serial debugging
#define BAUDRATE 9600

#define CURRENT_SENSE_CHECK_MS 50 // check the current every x ms

#define BACKWARD 0
#define FORWARD 1

/* Pins */
#define PWM_PIN 12          // Channel A PWM
#define DIR_PIN 3             // Channel A direction
#define BRAKE_PIN 9           // Channel A brake
#define CURRENT_SENSE_PIN A0  // Channel A current sense adc
#define POS_PIN           A2  // Potentiometer connected to the shaft for position feedback

#define CURR_MAX 55
#define ANGLE_MIN_LIMIT 0   // degree
#define ANGLE_MAX_LIMIT 90  // degree
#define P_GAIN 1

/* Global variables */
char received_byte;
int current = 0;
int currentLimit = 0;
volatile uint8_t movementEnabled = true;
volatile uint8_t intCounter = 0;
uint8_t setpoint = 0;

SIGNAL(TIMER0_COMPA_vect) // Interrupt is called once a millisecond
{
  if(intCounter == CURRENT_SENSE_CHECK_MS)
  {
    current = analogRead(CURRENT_SENSE_PIN);  // check current
    if(current > currentLimit)
    {
      movementEnabled = false; //TODO TOGGLE BIT -> NO MOVEMENT!  
    }
    else {movementEnabled = true;} 

    control_motor();  // control loop
    
    intCounter = 0;
  }
  else {intCounter++;}
} 

void setup()
{
  pinMode (PWM_PIN, OUTPUT);
  digitalWrite(PWM_PIN, LOW);
  pinMode (DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW);

  pinMode (CURRENT_SENSE_PIN, INPUT);
  pinMode (POS_PIN, INPUT);
  
  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);

  Serial.begin(BAUDRATE);
  
  #ifdef DEBUG
    Serial.println("Start motor test");
    Serial.println("Open = '+'");
    Serial.println("Close = '-'");
    Serial.println("Info = 'i'");
  #endif
  
}

void loop()
{
  //Serial.println("Current = " + String(analogRead(A0)));
  //Serial.println(avg());

  received_byte = Serial.read();
  switch (received_byte) 
  {
    case '+':
    break;
    break;
    default: 
    break;
  }
  received_byte = 0;

  delay(10);
}
/*
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
      digitalWrite (PWM_PIN, LOW);
      //Serial.println("Current too high!");
    }
    delay(1);
  }
  digitalWrite (speedA, LOW);
  curr_trip = 0;
}*/

/* Function to set the position of the hand */
uint8_t set_setpoint(uint8_t angle)
{
  if((angle >= ANGLE_MIN_LIMIT) && (angle <= ANGLE_MAX_LIMIT))
  {
     setpoint = angle;
  }
  return setpoint;
}

uint8_t get_setpoint(void)
{
  return setpoint;
}

/* Function to get the angle of the hand in deg */
uint8_t get_angle(void)
{
  Serial.println("TODO, calculate angle number");
  uint8_t angle = 0;
  angle = map(analogRead(POS_PIN), 0, 1023, 0, 255);
  
  return (angle);
}

/* Function to calculate the pwm output to the motor (P control loop) */
uint8_t calculate_output(uint8_t setpoint, uint8_t current_angle)
{
  uint8_t pwm_output = (P_GAIN * (setpoint - current_angle)); // Calculate P action, pwm output to motor
  Serial.println("TODO: limit output to 0-255");
  pwm_output = (pwm_output > 255) ? 255 : pwm_output;  // limit input
  return pwm_output;
}

uint8_t control_motor(void)
{
  static uint8_t pwm_new;
  static uint8_t pwm_old;
  
  if(movementEnabled) // OK
  {
    pwm_new = calculate_output(get_setpoint(), get_angle());
    digitalWrite(BRAKE_PIN, LOW);       // Release brake condition
    
    if(pwm_new != pwm_old)              // Only write a new value if necessary
    {
      analogWrite(PWM_PIN, pwm_new);    // set pwm output  
      pwm_old = pwm_new;
    }
  }
  else                // current overflow
  {
    digitalWrite(BRAKE_PIN, HIGH); // Brake condition, HIGH = Brake
    analogWrite(PWM_PIN, 0);      
    #ifdef DEBUG
    Serial.println("Motor stopped, current overflow!");
    #endif
    return true;                   // failed, current limit
  }
  
  return false;
}

