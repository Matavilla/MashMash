#pragma once

#include <utility>
#include <limits>
#include <cmath>
#include <iostream>
#include <memory>
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

    Coord vect(const Coord& b) const {
        return Coord(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
    }

};

struct ObjINT {
    Pixel color;
    int specular;
    double reflective;

    virtual std::pair<double, double> RayIntersect(Coord O, Coord D) = 0;

    virtual Coord GetNormal(const Coord& p) const = 0;

    virtual ~ObjINT() {};
};

struct Sphere : ObjINT {
    Coord c;
    double r;

    Sphere(Coord _c = Coord(), double r1 = 0., Pixel _col = Pixel(), int _s = 0, double _ref = 0) {
        c = _c;
        r = r1;
        color = _col;
        specular = _s;
        reflective = _ref;
    }

    Coord GetNormal(const Coord& p) const {
        return (p - c) / r;
    }

    virtual std::pair<double, double> RayIntersect(Coord O, Coord D) override {
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

struct Triangle : ObjINT {
    Coord a, b, c;

    Triangle(const Coord& _a, const Coord& _b, const Coord& _c, const Pixel& _col, int _s, double _ref) {
        a = _a;
        b = _b;
        c = _c;
        color = _col;
        specular = _s;
        reflective = _ref;
    }

    std::pair<double, double> RayIntersect(Coord O, Coord D) override {
        Coord e1 = b - a;
        Coord e2 = c - a;
        Coord pvec = D.vect(e2);
        
        double disc = e1 * pvec;
        if (disc < 0) {
            return std::make_pair(inf, inf);
        }

        Coord tvec = O - a;
        double u = tvec * pvec;
        if (u < 0 || u > disc) {
            return std::make_pair(inf, inf);
        }

        Coord qvec = tvec.vect(e1);
        double v = D * qvec;
        if (v < 0 || u + v > disc) {
            return std::make_pair(inf, inf);
        }

        return std::make_pair((e2 * qvec) / disc, inf);
    }

    Coord GetNormal(const Coord& c) const {
        return c.vect(b - a);
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
    std::vector<std::unique_ptr<ObjINT>> sp;
    std::vector<Light> li;

    Pixel BG;
    int w = 1024;
    int h = 1024;

    Scene() {
        BG  = Pixel(114, 128, 250);

        sp.emplace_back(new Sphere(Coord(-3, 1, 4), 1, Pixel(133, 21, 199), 500, 0.3));
        sp.emplace_back(new Sphere(Coord(-1, 1, 5.5), 1, Pixel(238, 104, 123), 50, 0));
        sp.emplace_back(new Sphere(Coord(3, 1, 5), 1, Pixel(47, 255, 173), 100, 0.9));
        sp.emplace_back(new Sphere(Coord(0, -5001, 0), 5000, Pixel(181, 228, 255), 1000, 0.05));
        sp.emplace_back(new Sphere(Coord(130, 140, 180), 12, Pixel(225, 228, 255), 2000, 0.04));


        sp.emplace_back(new Triangle(Coord(2, -1, 4), Coord(0, 1, 3), Coord(-2, -1, 4), Pixel(107, 183, 189), 100, 0.02));
        sp.emplace_back(new Triangle(Coord(0, -1, 2), Coord(2, -1, 4), Coord(-2, -1, 4), Pixel(185, 218, 255), 100, 0.02));
        sp.emplace_back(new Triangle(Coord(0, -1, 2), Coord(-2, -1, 4), Coord(0, 1, 3), Pixel(238, 130, 238), 100, 0.02));
        sp.emplace_back(new Triangle(Coord(2, -1, 4), Coord(0, -1, 2), Coord(0, 1, 3), Pixel(128, 128, 0), 100, 0.02));

        li.push_back(Light(Light::AMBIENT, 0.2));
        li.push_back(Light(Light::POINT, 0.6, Coord(2, 1, 0)));
        li.push_back(Light(Light::DIRECTIONAL, 0.2, Coord(1, 4, 4)));
    }
    Coord ToRealCoord(const double& x, const double& y);

    Pixel TraceRay(Coord O, Coord D, double t_min, double t_max, int depth = 3);

    double Lighting(Coord p, Coord n, Coord v, double s);

    bool ClosestIntersection(ObjINT** tmp_sp, double& tmp, Coord O, Coord D, double t_min = 1, double t_max = inf);
};

