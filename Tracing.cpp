#include "Tracing.h"
#include <iostream>

Pixel TraceRay(Scene& s, const Coord& d, const Coord& o) {
    float tmp = std::numeric_limits<float>::max();
    Pixel& tmp_c = s.BG;
    for(auto& sph : s.sp) {
        float dist = 0;
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
