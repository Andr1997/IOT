#include <LiquidCrystal.h>
#include <stdio.h>

#define BAUD_RATE 		9600
#define SENSOR_PIN		A0
#define RELAY_PIN		6

#define MAX_VOLTAGE 	    (double)5
#define ACD_RESOLUTION	    1024
#define MAX_TEMPERATURE 	(double)1000

#define WINDOW			    5

#define INITIAL_TEMPERATURE 35
#define HISTEREZYS          10

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

uint8_t setPointValue;

static FILE uartout = {0};

static int uart_putchar(char c, FILE *stream)
{
    Serial.write(c);
    lcd.write(c);
    return 0 ;
}

void setup()
{
	Serial.begin(BAUD_RATE);
    lcd.begin(16, 2);
    initRelay(RELAY_PIN);

    setPointValue = INITIAL_TEMPERATURE;

	fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &uartout;
}

void loop()
{ 
  double temperature;
  int8_t relayState;

  lcd.clear();

  setPoint();
  temperature = getTemperature();

  relayState = controlSystem(temperature);
  
  printf("Temp ");
  //lcd.print("Temperatura: ");
  lcd.print(temperature);
  lcd.setCursor(13, 0);

  printByRelayState(relayState);

  lcd.setCursor(0, 1);
  printf("Set point ");
  //lcd.print("Set-Point: ");
  lcd.print(setPointValue);

  delay(5000);
}

void printByRelayState(int8_t relayState)
{
  if (!relayState)
  {
      printf("Off");
  }
  else
  {
      printf("On");
  }
}

void setPoint(void)
{
    if (Serial.available() > 0)
    {
        setPointValue = Serial.parseInt();
    }
}

int8_t controlSystem(double temperature)
{
    if(temperature  < (setPointValue - HISTEREZYS))
    {
        relayControl(LOW);
        return 0;
    }
    if (temperature  > (setPointValue + HISTEREZYS))
    {
        relayControl(HIGH);
        return 1;
    }

    return -1;
}

void initRelay(uint8_t pinNumber)
{
    pinMode(pinNumber, OUTPUT);
}

void relayControl(uint8_t state)
{
    state = constrain(state, LOW,  HIGH);
    
    digitalWrite(RELAY_PIN, state);
}

double getTemperature(void)
{
  int sensorInput;
  double temp;

  sensorInput = analogRead(SENSOR_PIN);    //read the analog sensor and store it
  temp = (double)sensorInput / 1024;       //find percentage of input reading
  temp = temp * 5;                 //multiply by 5V to get voltage
  temp = temp - 0.5;               //Subtract the offset 
  temp = temp * 100;  

  return temp;
}