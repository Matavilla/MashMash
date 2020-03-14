#pragma once

#include <utility>
#include <limits>
#include <cmath>
#include <iostream>
#include <ctime>

#include "Bitmap.h"


const double inf = std::numeric_limits<double>::infinity();

struct Coord{
    double x, y, z;
    
    Coord(double _x = 0, double _y = 0, double _z = 0): x(_x), y(_y), z(_z) {
    }

    Coord operator -() const {
        return Coord(-x, -y, -z);
    }
    Coord operator +(const double& d) const {
        return Coord(x + d, y + d, z + d);
    }
    Coord operator +(const Coord& d) const {
        return Coord(x + d.x, y + d.y, z + d.z);
    }
    Coord operator -(const Coord& d) const {
        return Coord(x - d.x, y - d.y, z - d.z);
    }
    Coord operator *(const double& d) const {
        return Coord(x * d, y * d, z * d);
    }
    double operator *(const Coord& d) const {
        return x * d.x + y * d.y + z * d.z;
    }
    Coord operator /(const double r) const {
        return Coord(x / r, y / r, z / r);
    }
    
    double len() const {
        return sqrt(x * x + y * y + z * z);
    }

    Coord normalize() const {
        return (*this) / len();
    }

};

struct Sphere {
    Coord c;
    double r;
    Pixel color;
    int specular;
    double reflective;

    Sphere(Coord _c = Coord(), double r1 = 0., Pixel _col = Pixel(), int _s = 0, double _ref = 0) : c(_c),
                                                                                                    r(r1),
                                                                                                    color(_col),
                                                                                                    specular(_s),
                                                                                                    reflective(_ref) {}
    
    Coord GetNormal(const Coord& p) const {
        return (p - c) / r;
    }

    std::pair<double, double> RayIntersect(Coord O, Coord D) {
        Coord oc = O - c;

        double k1 = D * D;
        double k2 = 2 * (oc * D);
        double k3 = (oc * oc) - r * r;

        double disc = k2 * k2 - 4 * k1 * k3;
        if (disc < 0) {
            return std::make_pair(inf, inf);
        }

        double t1 = (-k2 + sqrt(disc)) / (2 * k1);
        double t2 = (-k2 - sqrt(disc)) / (2 * k1);
        
        return std::make_pair(t1, t2);
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
    int w = 800;
    int h = 800;

    Scene() {
        BG  = Pixel(0, 0, 0);

        sp.push_back(Sphere(Coord(2, 0, 4), 1, Pixel(133, 21, 199), 500, 0.3));
        sp.push_back(Sphere(Coord(0, -1, 3), 1, Pixel(238, 104, 123), 50, 0));
        sp.push_back(Sphere(Coord(-2, 0, 4), 1, Pixel(47, 255, 173), 100, 0.9));
        sp.push_back(Sphere(Coord(0, -5001, 0), 5000, Pixel(192, 192, 192), 1000, 0.05));
        sp.push_back(Sphere(Coord(130, 140, 180), 12, Pixel(225, 228, 255), 2000, 0.04));

        li.push_back(Light(Light::AMBIENT, 0.1));
        li.push_back(Light(Light::POINT, 0.8, Coord(2, 1, 0)));
        li.push_back(Light(Light::DIRECTIONAL, 0.1, Coord(1, 4, 4)));
    }
    Coord ToRealCoord(const double& x, const double& y);

    Pixel TraceRay(Coord O, Coord D, double t_min, double t_max, int depth = 3);

    double Lighting(Coord p, Coord n, Coord v, double s);

    bool ClosestIntersection(Sphere& tmp_sp, double& tmp, Coord O, Coord D, double t_min = 1, double t_max = inf);
};

