#include "foothold.h"

double Foothold::y_at(double x) const {
    if (start.x == end.x) return start.y;
    double t = (x - start.x) / (end.x - start.x);
    return start.y + t * (end.y - start.y);
}
