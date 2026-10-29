#include "debug_map.h"
#include "foothold.h"

#include <godot_cpp/core/class_db.hpp>
#include <vector>

void DebugMap::_bind_methods() {
}

void DebugMap::_ready() {
    footholds.push_back(Foothold{0, godot::Vector2(50, 400), godot::Vector2(300, 400)});
    footholds.push_back(Foothold{1, godot::Vector2(300, 400), godot::Vector2(500, 320)});
    footholds.push_back(Foothold{2, godot::Vector2(500, 320), godot::Vector2(650, 320)});
    footholds.push_back(Foothold{3, godot::Vector2(200, 220), godot::Vector2(400, 220)});
    footholds[0].next = &footholds[1];
    footholds[1].prev = &footholds[0];
    footholds[1].next = &footholds[2];
    footholds[2].prev = &footholds[1];
}

void DebugMap::_draw() {
    for (const Foothold &fh : footholds) {
        draw_line(fh.start, fh.end, godot::Color(1, 1, 1), 2.0);
    }
}

const std::vector<Foothold>& DebugMap::get_footholds() const {
    return footholds;
}
