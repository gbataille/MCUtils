#include <Arduino.h>
#include <SPI.h>

const int CS_PIN = 9;
const int CLK_PIN = 13;
const int DATA_PIN = 11;

// void setup() {
//   Serial.begin(115200);
//
//   pinMode(SS_PIN, OUTPUT);
//
//   SPI.begin();
//   // digitalWrite(SS_PIN, LOW);
//   // delay(100);
//   // SPI.transfer(0b10000000011);
//   // delay(100);
//   // digitalWrite(SS_PIN, HIGH);
//   // delay(100);
//
//   // TEST mode
//   // digitalWrite(SS_PIN, LOW);
//   // SPI.transfer(0b100111000000, 13);
//   // digitalWrite(SS_PIN, HIGH);
// }
//
// void loop() {
//   SPI.beginTransaction(SPISettings(32000, MSBFIRST, SPI_MODE0));
//
//   digitalWrite(SS_PIN, LOW);
//   SPI.transfer(0b00010100);
//   SPI.transfer(0b00010010);
//   // SPI.transfer(0b100100001000);
//   // SPI.transfer(0b100111000000, 13);
//   digitalWrite(SS_PIN, HIGH);
//   SPI.endTransaction();
// }

void sendBits(bool *bits, int length) {
  digitalWrite(CS_PIN, LOW);
  for (int i = 0; i < length; i++) {
    bool b = *(bits + i);
    digitalWrite(DATA_PIN, b);
    delayMicroseconds(90);
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(CLK_PIN, LOW);
  }
  digitalWrite(CS_PIN, HIGH);
  delay(1);
}

bool data[137];
void setup() {
  Serial.begin(115200);

  pinMode(CS_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  digitalWrite(CLK_PIN, LOW);
  digitalWrite(CS_PIN, HIGH);

  delay(100);

  bool instr[30] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                    0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0};

  data[0] = 1;
  data[1] = 0;
  data[2] = 1;
  for (int i = 3; i < sizeof(data); i++) {
    data[i] = 0;
  }

  Serial.println("Sending config");
  sendBits(&instr[0], 30);
  Serial.println("Sending data");
  sendBits(&data[0], sizeof(data));
  Serial.println("Done");
}

uint8_t value = 0;

void loop() {
  for (int i = 9; i < sizeof(data); i++) {
    if (i - 9 == value) {
      data[sizeof(data) - i + 9 - 1] = 1;
    } else {
      data[sizeof(data) - i + 9 - 1] = 0;
    }
  }
  sendBits(data, sizeof(data));
  Serial.println("########");
  Serial.print("Value: " + String(value + 1) + " - Data: ");
  for (int i = 0; i < sizeof(data); i++) {
    Serial.print(data[i]);
  }
  Serial.println("\n");

  value++;
  if (value == 128) {
    value = 0;
  }
  while (!Serial.available()) {
  }
  while (Serial.available()) {
    Serial.read();
  }
  delay(200);
}
