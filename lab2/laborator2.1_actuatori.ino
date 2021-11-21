#include <LiquidCrystal.h>

#define BAUD_RATE       9600

#define RELAY_PIN       6

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

static FILE uartout = {0};

static int putChar(char c, FILE *stream){
    lcd.write(c);
    return 0;
}

static int getChar(FILE *stream){
    while (!Serial.available());

    return Serial.read();
}

void setup(){
    Serial.begin(BAUD_RATE);
    lcd.begin(16, 2);
    pinMode(RELAY_PIN, OUTPUT);

    fdev_setup_stream(&uartout,  putChar, getChar, _FDEV_SETUP_RW);
    stdout = &uartout;
    stdin = &uartout;
}

void loop(){
    int buff;


    if (Serial.available() > 0)
    {
        lcd.clear();

        buff = Serial.read();

        if (buff ==  '1')
        {
            digitalWrite(RELAY_PIN, HIGH);
            printf("Bec conectat");
        }
        if(buff == '0')
        {
            digitalWrite(RELAY_PIN, LOW);
            printf("Bec deconectat");
        }
        
    }
    
}