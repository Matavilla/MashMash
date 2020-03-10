#include "Tracing.h"

Pixel TraceRay(Scene& s, const Coord& d, const Coord& o) {
    double tmp = std::numeric_limits<double>::max();
    Pixel tmp_c = s.BG;
    for(auto& sph : s.sp) {
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
