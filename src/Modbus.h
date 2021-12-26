#ifndef _Modbus_H // Put these two lines at the top of your file.
#define _Modbus_H // (Use a suitable name, usually based on the file name.)

unsigned short ModBus_CRC16(unsigned char *buffer, unsigned short len)
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

#endif