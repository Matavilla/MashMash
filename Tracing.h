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
    
    double len() const {
        return sqrt(x * x + y * y + z * z);
    }

    Coord normalize() const{
        return (*this) / len();
    }

};

struct Sphere {
    Coord c;
    double r;
    Pixel color;

    Sphere(Coord _c = Coord(), double r1 = 0., Pixel _col = Pixel()) : c(_c), r(r1), color(_col) {
    }
    
    Coord getNormal(const Coord& p) const {
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
    double Intency;
    Coord c;

    Light(const int& Typ, const double& Intenc, const Coord& _c = Coord()) : Type(Typ), Intency(Intenc), c(_c) {
    }
};

struct Scene {
    std::vector<Sphere> sp;
    std::vector<Light> li;


    Pixel BG;
    int w = 600;
    int h = 600;

    Scene() {
        BG = Pixel(255, 255, 255);

        sp.push_back(Sphere(Coord(2, 0, 4), 1, Pixel(128, 0, 0)));
        sp.push_back(Sphere(Coord(0, -1, 3), 1, Pixel(140, 230, 240)));
        sp.push_back(Sphere(Coord(-2, 0, 4), 1, Pixel(0, 69, 255)));
        sp.push_back(Sphere(Coord(0, -5001, 0), 5000, Pixel(212, 255, 127)));

        
        li.push_back(Light(Light::AMBIENT, 0.6));
        li.push_back(Light(Light::POINT, 0.3, Coord(2, 3, 2)));
        li.push_back(Light(Light::DIRECTIONAL, 0.2, Coord(2, 1, 0)));
    }
    Coord ToRealCoord(const double& x, const double& y);

    Pixel TraceRay(const Coord& d, const Coord& o);

    double Lighting(const Coord& p, const Coord& n);
};

