#pragma once

class Random
{
public:
    static unsigned int random_uint(unsigned int min = 0x00000000, unsigned int max = 0xFFFFFFFF);
    static float random_float(float min = 0.0f, float max = 1.0f);
    static int random_int(int min = 0x00000000, int max = 0xFFFFFFFF);
    static unsigned short random_ushort(unsigned short min = 0x0000, unsigned short max = 0xFFFF);
    static unsigned char random_byte(unsigned char min = 0x00, unsigned char max = 0xFF);
    static short random_short(short min = 0x0000, short max = 0xFFFF);
    static char random_sbyte(char min = 0x00, char max = 0xFF);
};