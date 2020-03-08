#pragma once

#include <utility>
#include <limits>
#include <cmath>
#include <iostream>

#include "Bitmap.h"


const double inf = std::numeric_limits<double>::infinity();
const int w = 512;
const int h = 512;

class Coord{
public:
    float x, y, z;
    
    Coord(float _x = 0, float _y = 0, float _z = 0): x(_x), y(_y), z(_z) {
    }

    friend Coord operator -(const Coord& f, const Coord& d) {
        return Coord(f.x - d.x, f.y - d.y, f.z - d.z);
    }
    friend float operator *(const Coord& f, const Coord& d) {
        return f.x * d.x + f.y * d.y + f.z * d.z;
    }
    
    Coord& operator /(const float r) {
        x /= r;
        y /= r;
        z /= r;
        return *this;
    }
        
    Coord& normalize() {
        float n = sqrt(x * x + y * y + z * z);
        return (*this) / n;
    }

};

struct Sphere {
    Coord c;
    float r;
    Pixel color;

    Sphere(Coord _c, float r1, Pixel _col) : c(_c), r(r1), color(_col) {
    }
    
    Coord getNormal(const Coord&p) const {
        return (p - c) / r;
    }

    bool RayIntersect(const Coord& o, const Coord& d, float& t0) {
        Coord oc = o - c;
        float b = 2 * (oc * d);
        float c = (oc * oc) - r * r;
        float disc = b * b - 4 * c;
        if (disc < 0) {
            return false;
        }
        disc = sqrt(disc);
        float t1 = -b - disc;
        float t2 = -b + disc;
        t0 = (t1 < t2) ? t1 : t2;
        return true;
    }
};

struct Scene {
    std::vector<Sphere> sp;
    Pixel BG;

    Scene() {
        BG = Pixel(0, 0, 0);
        //sp.push_back(Sphere(Coord(128, 128, 3), 1, Pixel(255, 0, 0)));
        sp.push_back(Sphere(Coord(w / 2., h / 2., 50), 50, Pixel(0, 255, 0)));
        //sp.push_back(Sphere(Coord(312, 312, 4), 1, Pixel(0, 0, 255)));
    }
};

extern Pixel TraceRay(Scene& s, const Coord& d, const Coord& o);
