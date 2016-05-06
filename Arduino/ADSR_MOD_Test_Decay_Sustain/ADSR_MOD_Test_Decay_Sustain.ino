#include <SPI.h>
#include <MsTimer2.h>

#define MOD_RATE  1   // ms

#define PIN_LDAC   9      // MCP4922のラッチ動作出力ピン
#define PIN_DURATION  A0
#define PIN_DECAY     A1
#define PIN_SUSTAIN   A2   

int16_t beatLen = 500;

int16_t level = 4095;
int16_t duration = 400;
int16_t decay = 100;
int16_t sustain = 2000;

int16_t decay_delta;
int16_t mod_value;

int16_t tick;

// DACに出力
// parameter: v: 出力値(0 .. 4095)
void outDAC(int16_t v)
{
  digitalWrite(PIN_LDAC, HIGH) ;
  digitalWrite(SS, LOW) ;
  SPI.transfer((v >> 8)| 0x30) ;
  SPI.transfer(v & 0xff) ;
  digitalWrite(SS, HIGH) ;
  digitalWrite(PIN_LDAC, LOW) ;
}

void outADSR()
{
  tick++;
  
  if (tick > beatLen) {
    tick = 0;    
    // モジュレーション波形を初期化する
    mod_value = level;
    decay_delta = (level - sustain) / decay;
  }
  
  // 出力値補正
  if (mod_value < 0) {
      mod_value = 0;
  }
  outDAC(mod_value);
  
  if (tick < decay) {
    mod_value -= decay_delta;
  }
  if (tick == duration) {
    mod_value = 0;
  }
}

void setup() {
  Serial.begin(9600);
  
  // DAC出力の初期化
  pinMode(PIN_LDAC, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);            // ビットオーダー
  SPI.setClockDivider(SPI_CLOCK_DIV8);  // クロック(CLK)をシステムクロックの1/8で使用(16MHz/8)
  SPI.setDataMode(SPI_MODE0) ;          // クロック極性０(LOW)　クロック位相０
  
  // Timerの初期化
  MsTimer2::set(MOD_RATE, outADSR);
  MsTimer2::start();
}

void loop() {
  
  duration = map(analogRead(PIN_DURATION), 0, 1023, 0, beatLen);
  decay    = map(analogRead(PIN_DECAY), 0, 1023, 0, beatLen);
  sustain  = analogRead(PIN_SUSTAIN) << 2;
  
  Serial.print(duration);
  Serial.print("\t");
  Serial.print(decay);
  Serial.print("\t");
  Serial.print(sustain);
  Serial.print("\n");
}
