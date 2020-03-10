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
        dist = 0;
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
    Coord p = o + d * dist;
    return tmp_sp.color * Lighting(p, tmp_sp.getNormal(p));
}

double Scene::Lighting(const Coord& p, const Coord& n) {
    double i = 0.;
    for (auto& l : li) {
        if (l.Type == Light::AMBIENT) {
            i += l.Intency;
        } else {
            Coord L = l.c;
            if (l.Type == Light::POINT) {
                L = L - p;
            }
            if (n * L > 0) {
                i += l.Intency * (n * L) / (n.len() * L.len());
            }
        }
    }
    return i;
}
