#pragma once

#include <utility>
#include <limits>
#include <cmath>
#include <iostream>

#include "Bitmap.h"


const double inf = std::numeric_limits<double>::infinity();

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

        double k1 = d * d;
        double k2 = 2 * (oc * d);
        double k3 = (oc * oc) - r * r;
        double disc = k2 * k2 - 4 * k1 * k3;
        if (disc < 0) {
            return false;
        }
        double t1 = (-k2 + sqrt(disc)) / (2 * k1);
        double t2 = (-k2 - sqrt(disc)) / (2 * k1);
        if (t1 > 1) {
            t0 = t1;
        }
        if (t2 > 1 && t2 < t0) {
            t0 = t2;
        }
        if (t0 == 0) {
            t0 = inf;
        }
        return true;
    }
};

struct Light {
    enum TYPE_LIGHT {
        AMBIENT, 
        POINT,
        DIRECTIONAL
    };

    int Type;
    int Intency;
    Coord c;

    Light(const int& Typ, const int& Intenc, const Coord& _c = Coord()) : Type(Typ), Intency(Intenc), c(_c) {
    }
};

struct Scene {
    std::vector<Sphere> sp;
    std::vector<Light> li;


    Pixel BG;
    int w = 600;
    int h = 600;

    Scene() {
        BG = Pixel(10, 10, 10);

        sp.push_back(Sphere(Coord(1, 0, 4), 1, Pixel(255, 0, 0)));
        sp.push_back(Sphere(Coord(0, -1, 3), 1, Pixel(0, 255, 0)));
        sp.push_back(Sphere(Coord(-2, 0, 4), 1, Pixel(0, 0, 255)));
        sp.push_back(Sphere(Coord(0, -5001, 0), 5000, Pixel(0, 255, 255)));

        
        li.push_back(Light(Coord(1, 0, 4), 0.2));
        li.push_back(Light(Coord(0, -1, 3), 0.6, Coord(0, 255, 0)));
        li.push_back(Light(Coord(-2, 0, 4), 0.2, Coord(0, 0, 255)));
    }
    Coord ToRealCoord(const double& x, const double& y);

    Pixel TraceRay(const Coord& d, const Coord& o);
};

