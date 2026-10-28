#ifndef MAPLE_FOOTHOLD_H
#define MAPLE_FOOTHOLD_H

#include "godot_cpp/variant/vector2.hpp"

struct Foothold {
    int id;
    godot::Vector2 start;
    godot::Vector2 end;
    Foothold *prev = nullptr;
    Foothold *next = nullptr;

    double y_at(double x) const;
};

#endif
