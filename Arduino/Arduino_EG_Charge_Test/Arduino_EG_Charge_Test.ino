/*
 * Arduino EG Charge Test
 *
 * 2018.07.05
 *
 */

#define TITLE_STR1  ("Arduino EG Charge Test")
#define TITLE_STR2  ("20180705")

const int ThresholdPin = 0;

const int GateInPin = 2;

const int GateOutPin = 4;
const int DischargePin = 5;

const int CheckPin = 7;

volatile bool isGateOn = false;

void GateIn()
{
  isGateOn = !digitalRead(GateInPin);
  //Serial.println(isGateOn);

  if (isGateOn) {
    // GateOutPin
    digitalWrite(GateOutPin, HIGH);

    // DischaregePin
    pinMode(DischargePin, INPUT); // Hi-Z
  }
  else {
    // GateOutPin
    digitalWrite(GateOutPin, LOW);

    // DischaregePin
    pinMode(DischargePin, OUTPUT);
    digitalWrite(DischargePin, LOW);  // Sink
  }
}

void setup()
{
  pinMode(GateOutPin, OUTPUT);
  pinMode(DischargePin, INPUT); // Hi-Z

  pinMode(GateInPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GateInPin), GateIn, CHANGE);
  
  pinMode(CheckPin, OUTPUT);

  Serial.begin(115200);
  Serial.println(TITLE_STR1);
  Serial.println(TITLE_STR2);
  delay(1000);
}

void loop()
{
  int th = analogRead(ThresholdPin);
  Serial.println(th);

  if (isGateOn && th < 683) {
    digitalWrite(CheckPin, HIGH);
  }
  else {
    digitalWrite(CheckPin, LOW);
  }
}

