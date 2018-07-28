/*
 * Arduino EG
 *
 * 2018.07.10
 *
 */

#define UART_TRACE  (0)
#define PIN_CHECK   (0)

#define TITLE_STR1  ("Arduino EG")
#define TITLE_STR2  ("20180726")

const int ThresholdPin = 0;   // A0
const int AttackLevelPin = 1; // A1

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
volatile bool isStateChanged = true;

// Attack time threashold: (2/3) * 1024
int attackThreshold = 683;

void gateIn()
{
  bool isGateOn = digitalRead(GateInPin);
  
#if (UART_TRACE)
  Serial.println(isGateOn);
#endif

  if (isGateOn) {
    state = ST_ATTACK;
    isStateChanged = true;
    digitalWrite(LedPin, HIGH);
  }
  else {
    state = ST_RELEASE;
    isStateChanged = true;
    digitalWrite(LedPin, LOW);
  }
}

void setup()
{
  pinMode(AttackPin, OUTPUT);
  pinMode(InvAttackPin, OUTPUT);
  pinMode(GateOutPin, OUTPUT);
  pinMode(LedPin, OUTPUT);

  pinMode(GateInPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(GateInPin), gateIn, CHANGE);

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
#if (PIN_CHECK)
    digitalWrite(CheckPin, HIGH);
#endif
    
  attackThreshold = analogRead(AttackLevelPin);
  int th = analogRead(ThresholdPin);

  if (state == ST_ATTACK && th > attackThreshold) {
    state = ST_DECAY;
    isStateChanged = true;
  }
  
#if (UART_TRACE)
  Serial.print(attackThreshold);
  Serial.print("\t");
  Serial.print(th);
  Serial.print("\t");
  Serial.println(state);
#endif

  if (isStateChanged) {
    isStateChanged = false;
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
      digitalWrite(InvAttackPin, HIGH);
      digitalWrite(GateOutPin, LOW);
      break;
    }
  }
#if (PIN_CHECK)
    digitalWrite(CheckPin, LOW);
#endif

}

