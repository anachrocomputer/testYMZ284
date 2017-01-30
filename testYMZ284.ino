/* testYMZ284 --- simple sketch to drive the YMZ284 sound chip      2017-01-28 */
/* Copyright (c) 2017 John Honniball                                           */

#define CS_PIN    (2)
#define CLK_PIN   (3)
#define WR_PIN    (4)
#define A0_PIN    (5)
#define D0_PIN    (6)

void setup(void)
{
  int i;

  Serial.begin(9600);

  // Generate 2MHz clock on Pin 3
  pinMode(CLK_PIN, OUTPUT);
  TCCR2A = 0x23;
  TCCR2B = 0x09;
  OCR2A = 7;
  OCR2B = 1;

  delay(50);
  
  // Chip Select pin
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  // Write pin
  pinMode(WR_PIN, OUTPUT);
  digitalWrite(WR_PIN, HIGH);

  // A0 pin
  pinMode(A0_PIN, OUTPUT);
  digitalWrite(A0_PIN, LOW);

  // D0-D7 pins
  for (i = D0_PIN; i < (D0_PIN + 8); i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  //for (i = 0; i <= 0x0d; i++) {
  //  if (i == 7)
  //    aywrite(i, 0x3f);
  //  else
  //    aywrite(i, 0);
  //}

  aywrite(15, 0);
  aywrite(0, 142);  // (2000000 / 32) / frequency
  aywrite(1, 0);
  //aywrite(2, 255);
  //aywrite(0, 64);
  aywrite(6, 15);   // Noise
  aywrite(7, 0x3e); // Channel enables
  aywrite(8, 16);   // Channel amplitudes
  aywrite(9, 0);  
  aywrite(10, 0);
  aywrite(11, 0);   // Envelope period
  aywrite(12, 4);
  aywrite(13, 10);
  //aywrite(10, 15);
}

void loop(void)
{
  int ana;

  ana = analogRead(0);

  aywrite(11, ana & 0xff);
  aywrite(12, ana >> 8);

  ana = analogRead(1);
  
  //aywrite(0, ana & 0xff);
  //aywrite(1, ana >> 8);

}

void aywrite(int reg, int val)
{
  ymzwrite(LOW, reg);
  ymzwrite(HIGH, val);
}

void ymzwrite(int a0, int val)
{
  int i;

  digitalWrite(A0_PIN, a0);
  
  for (i = 0; i < 8; i++) {
    if (val & (1 << i))
      digitalWrite(D0_PIN + i, HIGH);
    else
      digitalWrite(D0_PIN + i, LOW);
  }

  // CS LOW
  digitalWrite(CS_PIN, LOW);

  // WR LOW
  digitalWrite(WR_PIN, LOW);

  //delayMicroseconds(1);
  
  // WR HIGH
  digitalWrite(WR_PIN, HIGH);

  // CS HIGH
  digitalWrite(CS_PIN, HIGH);
}

