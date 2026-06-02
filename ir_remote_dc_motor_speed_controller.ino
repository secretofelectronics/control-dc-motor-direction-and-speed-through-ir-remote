#include <IRremote.hpp>

#define IR_PIN 11

#define ENA 5
#define IN1 7
#define IN2 8

#define IR_UP      0xE619FF00
#define IR_DOWN    0xF906FF00
#define IR_POWER   0xF609FF00
#define IR_REVERSE 0xEA15FF00

bool motorState = false;
bool direction = true;   // true=Forward, false=Reverse

int speedLevel = 1;      // 1=Slow, 2=Medium, 3=Fast

void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  updateMotor();
}

void loop()
{
  if (IrReceiver.decode())
  {
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;

    switch (code)
    {
      case IR_POWER:
        motorState = !motorState;
        break;

      case IR_UP:
        if (speedLevel < 3)
          speedLevel++;
        break;

      case IR_DOWN:
        if (speedLevel > 1)
          speedLevel--;
        break;

      case IR_REVERSE:
        direction = !direction;
        break;
    }

    updateMotor();

    IrReceiver.resume();
  }
}

void updateMotor()
{
  if (direction)
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

  switch (speedLevel)
  {
    case 1:
      pwmValue = 85;   // Slow
      break;

    case 2:
      pwmValue = 170;  // Medium
      break;

    case 3:
      pwmValue = 255;  // Fast
      break;
  }

  if (!motorState)
    pwmValue = 0;

  analogWrite(ENA, pwmValue);
}
