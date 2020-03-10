#pragma once

#include <vector>

struct Pixel { 
    unsigned char r, g, b;

    Pixel(int a = 0, int a2 = 0, int a3 = 0) {
        r = (a > 255) ? 255 : (a < 0) ? 0 : a;
        g = (a2 > 255) ? 255 : (a2 < 0) ? 0 : a2;
        b = (a3 > 255) ? 255 : (a3 < 0) ? 0 : a3;
    }
    
    Pixel operator +(const Pixel& t) const {
        return Pixel(r + t.r, g + t.g, b + t.b);
    }
    
    Pixel operator -(const Pixel& t) const {
        return Pixel(r - t.r, g - t.g, b - t.b);
    }
    
    Pixel operator *(const double& t) const {
        return Pixel(r * t, g * t, b * t);
    }

    friend std::ostream& operator<< (std::ostream &out, const Pixel& p) {
        out << '(' << (int) p.r << ' ' << (int) p.g << ' ' << (int) p.b << ')';
    }

};

extern void SaveBMP(const char* fname, Pixel* pixels, int w, int h);
