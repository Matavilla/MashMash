#include <vector>

struct Pixel { 
    unsigned char r, g, b;

    Pixel(int a = 0, int a2 = 0, int a3 = 0) {
        if (a < 0) {
            a = 0;
        }
        if (a2 < 0) {
            a2 = 0;
        }
        if (a3 < 0) {
            a3 = 0;
        }
        if (a > 255) {
            a = 255;
        }
        if (a2 > 255) {
            a2 = 255;
        }
        if (a3 > 255) {
            a3 = 255;
        }
        r = a;
        g = a2;
        b = a3;
    }
    
    Pixel& operator +=(Pixel& t) {
        if (r + t.r < 255) {
            r += t.r;
        } else {
            r = 255;
        }
        if (g + t.g < 255) {
            g += t.g;
        } else {
            g = 255;
        }
        if (b + t.b < 255) {
            b += t.b;
        } else {
            b = 255;
        }
        return *this;
    }
    
    Pixel& operator -=(Pixel& t) {
        if (r - t.r > 0) {
            r -= t.r;
        } else {
            r = 0;
        }
        if (g - t.g > 0) {
            g -= t.g;
        } else {
            g = 0;
        }
        if (b - t.b > 0) {
            b -= t.b;
        } else {
            b = 0;
        }
        return *this;
    }
    
    Pixel& operator *=(double& t) {
        if (r * t < 255) {
            r *= t;
        } else {
            r = 255;
        }
        if (g * t < 255) {
            g *= t;
        } else {
            g = 255;
        }
        if (b * t < 255) {
            b *= t;
        } else {
            b = 255;
        }
        return *this;
    }

    Pixel operator +(Pixel& t) {
        return Pixel(r + t.r, g + t.g, b + t.b);
    }
    
    Pixel operator -(Pixel& t) {
        return Pixel(r - t.r, g - t.g, b - t.b);
    }
    
    Pixel operator *(double& t) {
        return Pixel(r * t, g * t, b * t);
    }
    

};

extern void SaveBMP(const char* fname, std::vector<Pixel>& pixels, int w, int h);
