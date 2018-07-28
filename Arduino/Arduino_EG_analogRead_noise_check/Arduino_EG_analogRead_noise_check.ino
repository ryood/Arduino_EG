/*
 * Arduino EG analogRead noise check
 *
 * 2018.07.26
 *
 */

#define PIN_CHECK   (1)

const int ThresholdPin = 0;   // A0
const int AttackLevelPin = 1; // A1

#if (PIN_CHECK)
const int CheckPin = 10;
#endif

void setup()
{
#if (PIN_CHECK)
  pinMode(CheckPin, OUTPUT);
#endif
}

void loop()
{
#if (PIN_CHECK)
    digitalWrite(CheckPin, HIGH);
#endif
    
  int at = analogRead(AttackLevelPin);
  int th = analogRead(ThresholdPin);

#if (PIN_CHECK)
    digitalWrite(CheckPin, LOW);
#endif

}
