#pragma once

#include <utility>
#include <limits>
#include <cmath>
#include <iostream>

#include "Bitmap.h"


const double inf = std::numeric_limits<double>::infinity();
const int w = 600;
const int h = 600;

struct Coord{
    double x, y, z;
    
    Coord(double _x = 0, double _y = 0, double _z = 0): x(_x), y(_y), z(_z) {
    }

    Coord operator +(const Coord& d) const{
        return Coord(x + d.x, y + d.y, z + d.z);
    }
    Coord operator -(const Coord& d) const{
        return Coord(x - d.x, y - d.y, z - d.z);
    }
    Coord operator *(const double& d) const{
        return Coord(x * d, y * d, z * d);
    }
    double operator *(const Coord& d) const{
        return x * d.x + y * d.y + z * d.z;
    }
    Coord operator /(const double r) const{
        return Coord(x / r, y / r, z / r);
    }
    Coord normalize() const{
        double n = sqrt(x * x + y * y + z * z);
        return (*this) / n;
    }

};

struct Sphere {
    Coord c;
    double r;
    Pixel color;

    Sphere(Coord _c, double r1, Pixel _col) : c(_c), r(r1), color(_col) {
    }
    
    Coord getNormal(const Coord&p) const {
        return (p - c) / r;
    }

    bool RayIntersect(const Coord& o, const Coord& d, double& t0) {
        Coord oc = o - c;
        double b = 2 * (oc * d);
        double c1 = (oc * oc) - r * r;
        double disc = b * b - 4 * c1;
        if (disc < 1e-4) {
            return false;
        }
        disc = sqrt(disc);
        double t1 = -b - disc;
        double t2 = -b + disc;
        t0 = (t1 < t2) ? t1 : t2;
        return true;
    }
};

struct Scene {
    std::vector<Sphere> sp;
    Pixel BG;

    Scene() {
        BG = Pixel(10, 10, 10);
        //sp.push_back(Sphere(Coord(128, 128, 3), 1, Pixel(255, 0, 0)));
        sp.push_back(Sphere(Coord(w / 2., h / 2., -50), 50, Pixel(0, 255, 0)));
        //sp.push_back(Sphere(Coord(312, 312, 4), 1, Pixel(0, 0, 255)));
    }
};

extern Pixel TraceRay(Scene& s, const Coord& d, const Coord& o);
