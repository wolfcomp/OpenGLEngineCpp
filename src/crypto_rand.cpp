#include <random>
#include "crypto_rand.h"

std::random_device rd;
std::mt19937_64 gen(rd());

unsigned int Random::random_uint(unsigned int min, unsigned int max)
{
    std::uniform_int_distribution<unsigned int> dis(min, max);
    return dis(gen);
}

float Random::random_float(float min, float max)
{
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

int Random::random_int(int min, int max)
{
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

unsigned short Random::random_ushort(unsigned short min, unsigned short max)
{
    std::uniform_int_distribution<unsigned short> dis(min, max);
    return dis(gen);
}

unsigned char Random::random_byte(unsigned char min, unsigned char max)
{
    std::uniform_int_distribution<unsigned short> dis(min, max);
    return dis(gen);
}

short Random::random_short(short min, short max)
{
    std::uniform_int_distribution<short> dis(min, max);
    return dis(gen);
}

char Random::random_sbyte(char min, char max)
{
    std::uniform_int_distribution<short> dis(min, max);
    return dis(gen);
}