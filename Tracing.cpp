#include "Tracing.h"

Coord Scene::ToRealCoord(const double& x, const double& y) {
    return Coord((x - w / 2) / (double) (w / 2), (y - h / 2) / (double) (h / 2), 1);
}

Pixel Scene::TraceRay(const Coord& d, const Coord& o) {
    double tmp = std::numeric_limits<double>::max();
    Sphere tmp_sp;
    bool flag = false;
    double dist;
    for (auto& sph : sp) {
        double dist = 0;
        if (!sph.RayIntersect(o, d, dist)) {
            continue;
        }
        if (dist < tmp) {
            tmp = dist;
            tmp_sp = sph;
            flag = true;
        }
    }
    if (!flag) {
        return BG;
    }
    Coord p = o + d * tmp;
    return tmp_sp.color * Lighting(p, tmp_sp.getNormal(p), d * -1, tmp_sp.specular);
}

double Scene::Lighting(const Coord& p, const Coord& n, const Coord& v, const double s) {
    double i = 0.;
    for (auto& l : li) {
        if (l.Type == Light::AMBIENT) {
            i += l.Intency;
        } else {
            Coord L = l.c;
            if (l.Type == Light::POINT) {
                L = l.c - p;
            } 
            if (n * L > 0) {
                i += l.Intency * (n * L) / (n.len() * L.len());
            }

            Coord r = n * 2 * (n * L) - L;
            if (r * v > 0) {
                i += l.Intency * std::pow((r * v) / (r.len() * v.len()) , s);
            }
        }
    }
    return i;
}
