/*
 * Arduino EG
 *
 * 2018.07.10
 *
 */

#define UART_TRACE  (0)
#define PIN_CHECK   (0)

#define TITLE_STR1  ("Arduino EG")
#define TITLE_STR2  ("20180710")

const int ThresholdPin = 0;

const int GateInPin = 2;

const int AttackPin = 4;
const int InvAttackPin = 5;
const int GateOutPin = 6;

const int LedPin = 13;

#if (PIN_CHECK)
const int CheckPin = 10;
#endif

enum EG_STATE {
  ST_ATTACK,
  ST_DECAY,
  ST_RELEASE
};

volatile enum EG_STATE state = ST_RELEASE;

void GateIn()
{
  bool isGateOn = digitalRead(GateInPin);
  
#if (UART_TRACE)
  Serial.println(isGateOn);
#endif

  if (isGateOn) {
    state = ST_ATTACK;
    digitalWrite(LedPin, HIGH);
  }
  else {
    state = ST_RELEASE;
    digitalWrite(LedPin, LOW);
  }
}

void setup()
{
  pinMode(AttackPin, OUTPUT);
  pinMode(InvAttackPin, OUTPUT);
  pinMode(GateOutPin, OUTPUT);
  pinMode(LedPin, OUTPUT);

  pinMode(GateInPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GateInPin), GateIn, CHANGE);

#if (PIN_CHECK)
  pinMode(CheckPin, OUTPUT);
#endif

#if (UART_TRACE)
  Serial.begin(115200);
  Serial.println(TITLE_STR1);
  Serial.println(TITLE_STR2);
  delay(1000);
#endif
}

void loop()
{
  int th = analogRead(ThresholdPin);

  if (state == ST_ATTACK && th > 683) {
    state = ST_DECAY;
  }
  
#if (UART_TRACE)
  Serial.print(th);
  Serial.print("\t");
  Serial.println(state);
#endif

  switch (state) {
  case ST_ATTACK:
    digitalWrite(GateOutPin, HIGH);
    digitalWrite(InvAttackPin, LOW);
    digitalWrite(AttackPin, HIGH);
    break;
  case ST_DECAY:
    digitalWrite(AttackPin, LOW);
    digitalWrite(InvAttackPin, HIGH);
    break;
  case ST_RELEASE:
    digitalWrite(AttackPin, LOW);
    digitalWrite(GateOutPin, LOW);
    break;
  }
}

