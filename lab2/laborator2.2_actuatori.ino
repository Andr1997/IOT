#include <LiquidCrystal.h>

#define BAUD_RATE       9600

#define PWM_PIN         10
#define IN1             7
#define IN2             6

#define SPEED_PERCENT_MIN               0
#define SPEED_PERCENT_MAX               100
#define SPEED__NEGATIVE_PERCENT_MAX     -100

#define PWM_VALUE_MIN                   0
#define PWM_VALUE_MAX                   255


void setup(){
    Serial.begin(BAUD_RATE);
    pinMode(PWM_PIN, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
}

void loop(){
    motorControl();
}

//layer 1
void pinControl(uint8_t firstInput, uint8_t secondInput){
    digitalWrite(IN1, firstInput);
    digitalWrite(IN2, secondInput);
}

void setPWM(uint8_t pwmValue)
{
    analogWrite(PWM_PIN, pwmValue);
}

//layer 2
int8_t setSpeed(int8_t speedPercent)
{
    uint8_t pwmValue;
    speedPercent = abs(speedPercent);

    pwmValue = map(speedPercent, SPEED_PERCENT_MIN,SPEED_PERCENT_MAX, PWM_VALUE_MIN, PWM_VALUE_MAX);

    setPWM(pwmValue);

    return speedPercent;
}

int8_t setDirection(int8_t speedPercent)
{
    if (speedPercent > 0)
    {
        pinControl(HIGH, LOW);
        return 1;
    }
    if (speedPercent < 0)
    {
        pinControl(LOW, HIGH);
        return -1;
    }
    if (speedPercent == 0)
    {
        //pinControl(analogWrite,LOW);
        pinControl(LOW, LOW);
        return 0;
    }
    return 2;
}

//layer 3
void motorControl(void)
{
    int16_t speedPercent;
    int8_t speed;
    int8_t direction;

    speedPercent = readInputData();
    speedPercent = constrain(
                                speedPercent,
                                SPEED__NEGATIVE_PERCENT_MAX,
                                SPEED_PERCENT_MAX
                            );
    direction = setDirection((int8_t)speedPercent);
    speed = setSpeed((uint8_t)speedPercent);

    writeOutputData(direction, speed);
}

//layer 4
int16_t readInputData()
{
    static int16_t speedPercent = 0;

    if (Serial.available() > 0)
    {
        speedPercent = Serial.parseInt();


        //analogWrite(PWM_PIN, speedPercent);
    }

    return speedPercent;
}

void writeOutputData(const int8_t direction, const uint8_t speedPercent)
{
    static uint32_t toRefresh = 0;

    if (millis() > toRefresh)
    {
        switch(direction)
        {
            case 1:
                Serial.println("Inainte!");
                Serial.print("Putere: ");
                Serial.print(speedPercent);
                Serial.println(" %");
            break;
            case -1:
                Serial.println("Inapoi!");
                Serial.print("Putere: ");
                Serial.print(speedPercent);
                Serial.println(" %");
            break;
            case 0:
                Serial.println("Stop!");
                Serial.print("Putere: ");
                Serial.print(speedPercent);
                Serial.println(" %");
            break;
            default:
            Serial.println("Eroare!");
        }
        
        Serial.println();
        toRefresh = millis() + 500;
    }
}