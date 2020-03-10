#include "Tracing.h"

Coord Scene::ToRealCoord(const double& x, const double& y) {
    return Coord((x - w / 2) / (double) w, (y - h / 2) / (double) h, 1);
}

Pixel Scene::TraceRay(const Coord& d, const Coord& o) {
    double tmp = std::numeric_limits<double>::max();
    Pixel tmp_c = BG;
    for(auto& sph : sp) {
        double dist = 0;
        if(!sph.RayIntersect(o, d, dist)) {
            continue;
        }
        if (dist < tmp) {
            tmp = dist;
            tmp_c = sph.color;
        }
    }
    return tmp_c;
}
