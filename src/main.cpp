// https://microdigisoft.com/esp32-with-modbus-rtu-rs485-protocol-using-arduino-ide/
#include <SoftwareSerial.h>

// RS485 setup with ESp32
#define RE 32 // Connect RE terminal with 32 of ESP
#define DE 33 // Connect DE terminal with 33 of ESP

// const byte ModReadBuffer[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x02, 0x71, 0xCB}; // Voltage
byte ModReadBuffer[] = {0x01, 0x04, 0x00, 0x06, 0x00, 0x02, 0x91, 0xCA}; // Current
// const byte ModReadBuffer[] = {0x01, 0x04, 0x00, 0x0C, 0x00, 0x02, 0xB1, 0xC8}; // Power
// const byte ModReadBuffer[] = {0x01, 0x04, 0x00, 0x46, 0x00, 0x02, 0x90, 0x1E}; // Frequency
// const byte ModReadBuffer[] = {0x01, 0x04, 0x00, 0x48, 0x00, 0x02, 0xF1, 0xDD}; // kwh

byte BufferValue[9];
int counter = 0;

SoftwareSerial mod(26, 27); // RX=26 , TX =27

unsigned short ModBus_CRC16(unsigned char *buffer, unsigned short len) {
    unsigned int cur_crc;
    cur_crc = 0xFFFF;
    do {
        unsigned int i = 8;
        cur_crc = cur_crc ^ *buffer++;
        do {
            if (0x0001 & cur_crc) {
                cur_crc >>= 1;
                cur_crc ^= 0xA001;
            } else {
                cur_crc >>= 1;
            }
        } while (--i);
    } while (--len);
    return cur_crc;
}

byte ModbusData()
{
  byte i;

  // We set driver enable and receiver enable to high in order to send our request
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  if (mod.write(ModReadBuffer, sizeof(ModReadBuffer)) == 8)
  {
    // We set driver enable and receiver enable to low in order to receive a response from our req
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);

    for (int k = 0; k < sizeof(BufferValue); k++)
    {
      BufferValue[i] = 0;
    }

    Serial.print("Reading nr: ");
    Serial.println(++counter);

    for (i = 0; i < sizeof(BufferValue); i++)
    {
      // Serial.println(mod.read(), HEX);
      BufferValue[i] = mod.read();
    }

/*
    for (i = 0; i < sizeof(BufferValue); i++)
    {
      Serial.println(BufferValue[i], HEX);
    }
*/
    Serial.println("");

    byte incoming[4] = {BufferValue[6], BufferValue[5], BufferValue[4], BufferValue[3]};
    // byte incoming[4] = {BufferValue[3], BufferValue[4], BufferValue[5], BufferValue[6]};
    float volt1 = *((float *)incoming);
    Serial.print(volt1);
    Serial.println("V just nu!!!!!!!!!!!!!!!");
  }
  return BufferValue[8];
}

void setup()
{
  Serial.begin(115200);
  mod.begin(9600); // modbus configuration

  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
}

void loop()
{
  while (mod.write(ModReadBuffer, sizeof(ModReadBuffer)) == 8)
  {
    ModbusData();
    delay(1000);
  }
}
