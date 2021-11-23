#include <LiquidCrystal.h>
#include <stdio.h>
#include <Wire.h>


#define BAUD_RATE 		9600
#define SENSOR_PIN		A0

#define MAX_VOLTAGE 	(double)5
#define ACD_RESOLUTION	1024
#define MAX_RESISTANCE 	(double)1000

#define WINDOW			5

static FILE uartout = {0};

static int uart_putchar(char c, FILE *stream)
{
    Serial.write(c);
    return 0 ;
}

void setup()
{
	Serial.begin(BAUD_RATE);
    Wire.begin(0x08);
    Wire.onRequest(requestEvent);

	fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &uartout;
}

void loop()
{
 
  delay(10);
}

void requestEvent()
{
    double temperature;
    temperature = getTemperature();

    const uint8_t buff_length = 6;

    char buff[buff_length] = {};

    dtostrf(temperature, 5, 2, buff);

    for (int i = 0; i < buff_length; i++)
    {
        Wire.write(buff[i]);
    }
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