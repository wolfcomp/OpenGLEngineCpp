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

    bool operator==(const UUID &other) const
    {
        return data1 == other.data1 && data2 == other.data2 && data3 == other.data3 && data4 == other.data4 && data5[0] == other.data5[0] && data5[1] == other.data5[1] && data5[2] == other.data5[2] && data5[3] == other.data5[3] && data5[4] == other.data5[4] && data5[5] == other.data5[5];
    }

    bool operator!=(const UUID &other) const
    {
        return !(*this == other);
    }

    bool equals(const UUID &other) const
    {
        return *this == other;
    }
};