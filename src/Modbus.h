#ifndef Modbus_h
#define Modbus_h

#include <SoftwareSerial.h>

class Modbus
{
private:
    SoftwareSerial mod;
    unsigned char RE;
    unsigned char DE;
    byte BufferValue[9];
    byte ModReadBuffer[] = {0x01, 0x04, 0x00, 0x06, 0x00, 0x02, 0x91, 0xCA}; // Current

    int counter = 0;

public:
    Modbus(unsigned char RE, unsigned char DE, unsigned char RX, unsigned char TX, int baud);
    unsigned short ModBus_CRC16(unsigned char *buffer, unsigned short len);
    byte ModbusData();
    void run();
    ~Modbus();
};

Modbus::Modbus(unsigned char RE, unsigned char DE, unsigned char RX, unsigned char TX, int baud)
{
    mod(RX, TX);
    mod.begin(baud);
    this->RE = RE;
    this->DE = DE;

    pinMode(RE, OUTPUT);
    pinMode(DE, OUTPUT);
}

Modbus::~Modbus()
{
}

unsigned short Modbus::ModBus_CRC16(unsigned char *buffer, unsigned short len)
{
    unsigned int cur_crc;
    cur_crc = 0xFFFF;
    do
    {
        unsigned int i = 8;
        cur_crc = cur_crc ^ *buffer++;
        do
        {
            if (0x0001 & cur_crc)
            {
                cur_crc >>= 1;
                cur_crc ^= 0xA001;
            }
            else
            {
                cur_crc >>= 1;
            }
        } while (--i);
    } while (--len);
    return cur_crc;
}

byte Modbus::ModbusData()
{
    byte i;

    // We set driver enable and receiver enable to high in order to send our request
    digitalWrite(this->DE, HIGH);
    digitalWrite(this->RE, HIGH);
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

void Modbus::run()
{
    while (mod.write(ModReadBuffer, sizeof(ModReadBuffer)) == 8)
    {
        ModbusData();
        delay(1000);
    }
}

#endif