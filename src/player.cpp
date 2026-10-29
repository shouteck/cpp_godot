#include "player.h"
#include "debug_map.h"
#include "godot_cpp/classes/input.hpp"

#include <godot_cpp/core/class_db.hpp>

#include <algorithm>

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

    godot::Input *input = godot::Input::get_singleton();
    double dir = (input->is_action_pressed("ui_right") ? 1.0 : 0.0)
        -  (input->is_action_pressed("ui_left") ? 1.0 : 0.0);

    if (grounded) {
        if (input->is_action_just_pressed("ui_accept")) {
            grounded = false;
            y = current->y_at(x);
            vy = -JUMP;
            vx = dir * SPEED;
        }
        else {
            x += dir * SPEED * delta;

            double lo = std::min(current->start.x, current->end.x);
            double hi = std::max(current->start.x, current->end.x);

            if (x > hi) {
                if (current->next) current = current->next;
                else {
                    grounded = false;
                    x = hi;
                    y = current->y_at(hi);
                    vx = dir * SPEED;
                    vy = 0.0;
                }
            }
            else if (x < lo) {
                if (current->prev) current = current->prev;
                else {
                    grounded = false;
                    x = lo;
                    y = current->y_at(lo);
                    vx = dir * SPEED;
                    vy = 0.0;
                }
            }
        }
    }
    else {
        double old_y = y;

        vy += GRAVITY * delta;
        x += vx * delta;
        y += vy * delta;

        if (vy > 0.0) {
            for (const Foothold& fh : *map) {
                double flo = std::min(fh.start.x, fh.end.x);
                double fhi = std::max(fh.start.x, fh.end.x);
                if (x < flo || x > fhi) continue;

                double surface_y = fh.y_at(x);
                if (old_y <= surface_y && surface_y <= y) {
                    current = &fh;
                    y = surface_y;
                    vx = 0.0;
                    vy = 0.0;
                    grounded = true;
                    break;
                }
            }
        }
    }

    set_position(godot::Vector2(x, grounded ? current->y_at(x) : y));
}

void Player::_draw() {
    draw_rect(godot::Rect2(-6, -24, 12, 24), godot::Color(1.0, 0.4, 0.2));
}
