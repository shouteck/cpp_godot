#include "player.h"
#include "debug_map.h"

#include <godot_cpp/core/class_db.hpp>

void Player::_bind_methods() {
}

void Player::_ready() {

    DebugMap *map_node = Object::cast_to<DebugMap>(get_parent()->find_child("DebugMap"));
    map = &map_node->get_footholds();
    current = &(*map)[0];
    x = current->start.x;

}

void Player::_physics_process(double delta) {
    if (!map || !current) return;
    set_position(godot::Vector2(x, current->y_at(x)));
}

void Player::_draw() {
    draw_rect(godot::Rect2(-6, -24, 12, 24), godot::Color(1.0, 0.4, 0.2));
}
