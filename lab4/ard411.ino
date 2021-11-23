#include <LiquidCrystal.h>
#include <stdio.h>
#include <Wire.h>

#define BAUD_RATE       9600

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

static FILE uartout = {0};

static int putChar(char c, FILE *stream){
    if (isPrintable(c))
    {
       lcd.write(c);
       Serial.write(c);
    }

    return 0;
}

static int getChar(FILE *stream){
    while (!Serial.available());

    return Serial.read();
}

void setup(){
    Serial.begin(BAUD_RATE);
    Wire.begin();
    lcd.begin(16, 2);

    fdev_setup_stream(&uartout,  putChar, getChar, _FDEV_SETUP_RW);
    stdout = &uartout;
    stdin = &uartout;
}

void loop(){
    const uint8_t buff_lenght = 5;

    char buff[buff_lenght] = {};

    Wire.requestFrom(0x88, buff_lenght);
    uint8_t i =0;
    while (Wire.available())
    {
        buff[i] = Wire.read();
        i++;
    }

    lcd.clear();
  	printf("Res ");
  	lcd.print(buff);
  	printf(" ohm\n");
    delay(500);
}