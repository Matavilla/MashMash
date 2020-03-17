#include "Tracing.h"

Coord Scene::ToRealCoord(const double& x, const double& y) {
    return Coord((x - w / 2) / (double) (w / 2), (y - h / 2) / (double) (h / 2), 1);
}

inline Coord ReflectRay(const Coord& R, const Coord& N) {
    return (N * (2 * (N * R))) - R;
}

bool Scene::ClosestIntersection(ObjINT** tmp_sp, double& tmp, Coord O, Coord D, double t_min, double t_max) {
    tmp = inf;
    bool flag = false;
    for (auto &sph : sp) {
        auto i = sph->RayIntersect(O, D);
        if (i.first < tmp && t_min < i.first && t_max > i.first) {
            tmp = i.first;
            flag = true;
            *tmp_sp = sph.get();
        }
        if (i.second < tmp && t_min < i.second && t_max > i.second) {
            tmp = i.second;
            *tmp_sp = sph.get();
            flag = true;
        }
    }
    return flag;
}

Pixel Scene::TraceRay(Coord O, Coord D, double t_min, double t_max, int depth) {
    ObjINT* tmp_sp;

    double tmp;
     
    if (ClosestIntersection(&tmp_sp, tmp, O, D, t_min, t_max)) {
        Coord P = O + D * tmp;
        Coord N = tmp_sp->GetNormal(P);
        Pixel local_color = tmp_sp->color * Lighting(P, N, -D, tmp_sp->specular);

        if (depth <= 0 || tmp_sp->reflective <= 0)
            return local_color;

        Coord R = ReflectRay(-D, N);
        Pixel reflected_color = TraceRay(P, R, 0.001, inf, depth - 1);
    
        return local_color * (1. - tmp_sp->reflective) + reflected_color * tmp_sp->reflective;
    } 

    return BG;
}

double Scene::Lighting(Coord P, Coord N, Coord V, double s) {
    double i = 0.;
    for (auto& l : li) {
        if (l.Type == Light::AMBIENT) {
            i += l.Intency;
        } else {
            double tmp = inf;
            Coord L = l.c;
            if (l.Type == Light::POINT) {
                L = l.c - P;
                tmp = 1.;
            }

            ObjINT* tmp_sp;
            double dist;

            if (ClosestIntersection(&tmp_sp, dist, P, L, 0.001, tmp)) {
                continue;
            }

            if (N * L > 0) {
                i += l.Intency * (N * L) / (N.len() * L.len());
            }
            
            if (s != -1) {
                Coord R = ReflectRay(L, N);
                if (R * V > 0) {
                    i += l.Intency * std::pow((R * V) / (R.len() * V.len()) , s);
                }
            }
        }
    }
    return i;
}
