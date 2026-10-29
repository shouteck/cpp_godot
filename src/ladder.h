#ifndef MAPLE_LADDER_H
#define MAPLE_LADDER_H

struct Foothold;

struct Ladder {
    int id;
    double x;
    double y1;
    double y2;
    const Foothold *top_foothold;
};

#endif
