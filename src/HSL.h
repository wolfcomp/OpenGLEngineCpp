#pragma once
#include <cmath>

#include <glm/vec3.hpp>

struct hsv
{
    float h;
    float s;
    float v;

    operator float *()
    {
        return &h;
    }

    operator const float *() const
    {
        return &h;
    }

    explicit operator const float *()
    {
        return &h;
    }
};

class hsl
{
    template <typename T>
    static T clamp(T i, T min, T max)
    {
        if (i < min)
            return min;
        if (i > max)
            return max;
        return i;
    }

    static float get_modif(float time, float (*func)(float))
    {
        auto t = func(time);
        if (t < 0)
            t = -t;
        return 0.25f * t + 0.25f;
    }

    template <typename T>
    static T mod(T i, int mod)
    {
        return i - mod * static_cast<int>(i / mod);
    }

public:
    hsl() : h(0), s(0), l(0), rgb{0, 0, 0}
    {
    }

    hsl(int h, float s, float l) : h(h), s(s), l(l), rgb{0, 0, 0}
    {
    }

    float h;
    float s;
    float l;
    float rgb[3];

    void blend(int h, float s, float l, float amount = 0.5f)
    {
        this->h = this->h + (h - this->h) * amount;
        this->s = this->s + (s - this->s) * amount;
        this->l = this->l + (l - this->l) * amount;
    }

    void blend(hsl &other, float amount = 0.5f)
    {
        h = h + (other.h - h) * amount;
        s = s + (other.s - s) * amount;
        l = l + (other.l - l) * amount;
    }

    hsv get_hsv()
    {
        float h = this->h;
        float v = this->l + this->s * std::min(this->l, 1 - this->l);
        float s = v > 0 ? 2 * (1 - this->l / v) : 0;
        return {h, s, v};
    }

    void set_from_hsv(hsv hsv)
    {
        h = hsv.h;
        l = hsv.v * (1 - hsv.s / 2);
        if (l < glm::epsilon<float>() || abs(l - 1) < glm::epsilon<float>())
        {
            s = 0;
            return;
        }
        s = (hsv.v - l) / std::min(l, 1 - l);
    }

    float *get_rgb()
    {
        if (s <= glm::epsilon<float>())
        {
            rgb[0] = l;
            rgb[1] = l;
            rgb[2] = l;
            return rgb;
        }
        int h = static_cast<int>(this->h * 360);
        float c = (1.0f - abs(2.0f * l - 1.0f)) * s;
        float x = c * (1.0f - abs(mod(h / 60.0f, 2) - 1.0f));
        float m = l - c / 2;
        float r, g, b;
        if (h < 60)
        {
            r = c;
            g = x;
            b = 0;
        }
        else if (h < 120)
        {
            r = x;
            g = c;
            b = 0;
        }
        else if (h < 180)
        {
            r = 0;
            g = c;
            b = x;
        }
        else if (h < 240)
        {
            r = 0;
            g = x;
            b = c;
        }
        else if (h < 300)
        {
            r = x;
            g = 0;
            b = c;
        }
        else
        {
            r = c;
            g = 0;
            b = x;
        }
        r += m;
        g += m;
        b += m;
        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
        return rgb;
    }

    glm::vec3 get_rgb_vec3()
    {
        get_rgb();
        return {rgb[0], rgb[1], rgb[2]};
    }
};