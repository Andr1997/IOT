#include <stdio.h>

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
	fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &uartout;
}

void loop()
{
  uint16_t rawDataAcd;
  double voltage;
  double resistance;
  uint16_t rawDataAcdBuff[WINDOW];
  uint16_t medianValue;

  printf("Selected values: \n");

  for(uint16_t i = 0; i < WINDOW; i++){              //Populate array with values
      rawDataAcdBuff[i] = analogRead(SENSOR_PIN);
      printf("arr ");
      Serial.print(i);
      printf(" ");
      Serial.print(rawDataAcdBuff[i]);
      printf("\n");
      delay(1000);                                  //Delay for rotate Pontentiometer
  }

  printf("Calculate data\n");
  
  medianValue = medianFilter(rawDataAcdBuff);
  rawDataAcd = avgFilter(rawDataAcdBuff);
  voltage = adcToVoltage(rawDataAcd);
  resistance = voltageToResistance(voltage);

  printf("Median Value: ");                         //Afisarea valorilor calculate
  Serial.print(medianValue);
  printf("\n");

  printf("Average Value ADC: ");
  Serial.print(rawDataAcd);
  printf("\n");

  printf("Voltage: ");
  Serial.print(voltage);
  printf("\n");
  
  printf("Resistance: ");
  Serial.print(resistance);
  printf("\n");

  printf("\n\n");
  delay(5000);
}



double adcToVoltage(const uint16_t adcRaw)
{
	return (adcRaw * MAX_VOLTAGE) / ACD_RESOLUTION;
}

double voltageToResistance(const double voltage)
{
	return (voltage * MAX_RESISTANCE) / MAX_VOLTAGE;
}

uint16_t medianFilter(uint16_t *buff)       //median filter
{
	sort(buff, WINDOW);
	return buff[WINDOW / 2];
}

void sort(uint16_t *arr, uint16_t n) // Selection sort || min sort
{
    int i, j, min_idx;
    int aux;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
        if (arr[j] < arr[min_idx])
            min_idx = j;
 
        // Swap the found minimum element with the first element with auxiliar variable
        aux = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = aux;
    }
}


uint16_t avgFilter(uint16_t *tab)           //averagae filter
{
    uint16_t summ=0;

    for (uint16_t i = 0; i< WINDOW; i++)
        summ+=tab[i];

    return summ/WINDOW;
}