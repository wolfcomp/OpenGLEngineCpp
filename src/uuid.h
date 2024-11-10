#pragma once

#include <random>
#include "crypto_rand.h"

struct UUID
{
public:
    unsigned int data1;
    unsigned short data2;
    unsigned short data3;
    unsigned short data4;
    unsigned char data5[6];

    static UUID generate_v4()
    {
        UUID uuid;
        uuid.data1 = Random::random_uint();
        uuid.data2 = Random::random_ushort();
        uuid.data3 = Random::random_ushort();
        uuid.data4 = Random::random_ushort();
        for (int i = 0; i < 6; i++)
        {
            uuid.data5[i] = Random::random_byte();
        }
        uuid.data3 = (uuid.data3 & 0x0FFF) | 0x4000;
        uuid.data4 = (uuid.data4 & 0x3FFF) | 0x8000;
        return uuid;
    }
};