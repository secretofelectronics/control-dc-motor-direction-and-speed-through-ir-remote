#include <IRremote.hpp>

#define IR_PIN 11

#define ENA 5
#define IN1 7
#define IN2 8

#define IR_UP      0xF10EFF00
#define IR_DOWN    0xE51AFF00
#define IR_POWER   0xF609FF00
#define IR_REVERSE 0x

int speedLevel = 0;
bool motorState = false;
bool direction = true;

void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  Serial.begin(9600);

  updateMotor();
}

void loop()
{
  if (IrReceiver.decode())
  {
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;

    Serial.print("Code: ");
    Serial.println(code, HEX);

    switch(code)
    {
      case IR_POWER:
        motorState = !motorState;
        break;

      case IR_UP:
        if(speedLevel < 4)
          speedLevel++;
        break;

      case IR_DOWN:
        if(speedLevel > 0)
          speedLevel--;
        break;

      case IR_REVERSE:
        direction = !direction;
        break;
    }

    updateMotor();

    IrReceiver.resume();
  }
  delay(1000);
}

void updateMotor()
{
  if(direction)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }

  int pwmValue = 0;

  switch(speedLevel)
  {
    case 0: pwmValue = 0; break;
    case 1: pwmValue = 64; break;
    case 2: pwmValue = 128; break;
    case 3: pwmValue = 192; break;
    case 4: pwmValue = 255; break;
  }

  if(!motorState)
    pwmValue = 0;

  analogWrite(ENA, pwmValue);

  Serial.print("Speed Level: ");
  Serial.print(speedLevel);

  Serial.print(" PWM: ");
  Serial.println(pwmValue);
}