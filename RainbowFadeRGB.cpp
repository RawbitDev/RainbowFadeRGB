#include <iostream>
#include <tuple>
#include <math.h>

#define DEBUG true
#define ToUInt(x) static_cast<unsigned int>(x)

template <typename T>
using triple = std::tuple<T, T, T>;

triple<float> RgbToHsv(triple<unsigned char> rgb) {
    unsigned int r, g, b;
    float h = 0.0f, s, v;
    std::tie(r, g, b) = rgb;
    unsigned int max = std::max(r, std::max(g, b));
    unsigned int min = std::min(r, std::min(g, b));
    float maxf = max, minf = min, rf = r, gf = g, bf = b;
    if (min != max) {
        if (max == r) {
            h = (gf - bf) / (maxf - minf);
        } else if (max == g) {
            h = 2.0f + (bf - rf) / (maxf - minf);
        } else {
            h = 4.0f + (rf - gf) / (maxf - minf);
        }
        h *= 60;
        if (h < 0) {
            h += 360;
        }
    }
    s = 1.0f - (minf / maxf);
    v = maxf / 255.0f;
    if (DEBUG) {
        std::cout << "> DEBUG # [ "
                  << "R(" << ToUInt(std::get<0>(rgb)) << ") "
                  << "G(" << ToUInt(std::get<1>(rgb)) << ") "
                  << "B(" << ToUInt(std::get<2>(rgb)) << ") ]\t-> [ "
                  << "H(" << h << ") "
                  << "S(" << s << ") "
                  << "V(" << v << ") ]"
                  << std::endl;
    }
    return std::make_tuple(h, s, v);
}

triple<unsigned char> HsvToRgb(triple<float> hsv) {
    unsigned int r, g, b;
    float h, s, v, rf, gf, bf;
    std::tie(h, s, v) = hsv;
    float c = s * v;
    float x = c * (1.0f - static_cast<float>(fabs(fmod(static_cast<double>(h) / 60.0, 2.0) - 1.0)));
    float m = v - c;
    if (h >= 0 && h < 60) {
        rf = c;
        gf = x;
        bf = 0.0f;
    } else if (h >= 60 && h < 120) {
        rf = x;
        gf = c;
        bf = 0.0f;
    } else if (h >= 120 && h < 180) {
        rf = 0.0f;
        gf = c;
        bf = x;
    } else if (h >= 180 && h < 240) {
        rf = 0;
        gf = x;
        bf = c;
    } else if (h >= 240 && h < 300) {
        rf = x;
        gf = 0.0f;
        bf = c;
    } else {
        rf = c;
        gf = 0.0f;
        bf = x;
    }
    r = ToUInt((rf + m) * 255.0f);
    g = ToUInt((gf + m) * 255.0f);
    b = ToUInt((bf + m) * 255.0f);
    if (DEBUG) {
        std::cout << "> DEBUG # [ "
                  << "H(" << std::get <0>(hsv)
                  << ") " << "S(" << std::get <1>(hsv) << ") "
                  << "V(" << std::get <2>(hsv) << ") ]\t-> [ "
                  << "R(" << r << ") "
                  << "G(" << g << ") "
                  << "B(" << b << ") ]"
                  << std::endl;
    }
    return std::make_tuple(r, g, b);
}

triple<unsigned char> nextRGB(triple<unsigned char> rgb) {
    triple<float> hsv = RgbToHsv(rgb);
    std::get<0>(hsv) = ToUInt((roundf(std::get<0>(hsv) + 1))) % 360;
    return HsvToRgb(hsv);
}

[[noreturn]] void interpolateColor(unsigned char r, unsigned char g, unsigned char b) {
    triple<unsigned char> rgb = std::make_tuple(r, g, b);
    while (true) {
        std::cout << "R(" << ToUInt(std::get<0>(rgb)) << ") "
                  << "G(" << ToUInt(std::get<1>(rgb)) << ") "
                  << "B(" << ToUInt(std::get<2>(rgb)) << ")"
                  << std::endl;
        rgb = nextRGB(rgb);
    }
}

int main() {
    interpolateColor(255, 0, 0);
}
