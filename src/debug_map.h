#ifndef MAPLE_DEBUG_MAP_H
#define MAPLE_DEBUG_MAP_H

#include "foothold.h"

#include <godot_cpp/classes/node2d.hpp>
#include <vector>

class DebugMap : public godot::Node2D {
    GDCLASS(DebugMap, Node2D)
    protected:
        static void _bind_methods();
    public:
        void _ready() override;
        void _draw() override;
    private:
        std::vector<Foothold> footholds;
};

#endif
