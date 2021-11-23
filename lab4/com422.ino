#include <LiquidCrystal.h>
#include <stdio.h>

#define BAUD_RATE            9600

#define START                0x00
#define NUMBER               0x01
#define TX                   0x02
#define RX                   0x03
#define TYPE                 0x04
#define DATA                 0x05
#define CRC                  0x06
#define END                  0x07

#define START_INDICATOR      0xA5A5
#define ID_TX                0x08
#define ID_RX                0x09
#define REQUEST              0x00
#define ANSWER               0x01
#define END_INDICATOR        0xFFFF

uint16_t buff[7];

void setup()
{
    Serial.begin(BAUD_RATE);
    initBuff();

    buff[NUMBER] = 0;
}

void loop()
{
    sendData();
    receiveRequest();
    delay(100);
}

void sendData(void)
{
    for (int i = 0; i < 7; i++)
    {
        Serial.print(buff[i]);
        Serial.print(" ");
    }
    
    delay(500);
    Serial.print("\n");
}

int8_t receiveRequest(void)
{
    int8_t approved = 0;

    if (Serial.available() > 0)
    {
        for (uint8_t i = START; i < END; i++)
        {
            buff[i] = Serial.parseInt();

            approved += controlReceiveData(i);
        }

        buff[NUMBER] += buff[NUMBER];
    }

    return approved;
}

int8_t controlReceiveData(uint8_t index)
{
    switch (index)
    {
    case START:
        return checkSwitch(index, START_INDICATOR);
    case TX:
        return checkSwitch(index, ID_TX);
    case RX:
        return checkSwitch(index, ID_RX);
    case TYPE:
        return checkSwitch(index, REQUEST);
    case CRC:
        return checkSwitch(index, getCRC());
    case END:
        return checkSwitch(index, END_INDICATOR);
    default:
        return 0;
        break;
    }
}

int8_t checkSwitch(uint8_t index, uint16_t step)
{
    if (buff[index] == step)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

void initBuff()
{
    buff[START] = START_INDICATOR;
    buff[NUMBER] = 0;
    buff[TX] = ID_TX;
    buff[RX] = ID_RX;
    buff[TYPE] = ANSWER;
    buff[DATA] = 66;
    buff[CRC] = getCRC();
    buff[END] = END_INDICATOR;
}

uint16_t getCRC(void)
{
    uint16_t crc_summ = 0;

    for (int i = NUMBER; i < CRC; i++)
    {
       crc_summ+=buff[i];
    }

    return crc_summ;
}
