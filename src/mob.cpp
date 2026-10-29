#include "mob.h"
#include "debug_map.h"
#include "player.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include <algorithm>
#include <cmath>

void Mob::_bind_methods() {
}

void Mob::_ready() {
    DebugMap *map_node = Object::cast_to<DebugMap>(get_parent()->find_child("DebugMap"));
    map = &map_node->get_footholds();
    current = &(*map)[0];
    x = 150.0;
}

void Mob::take_damage(double dir) {
    if (dead) return;
    hp -= 1;
    x += dir * KNOCKBACK;
    if (hp <= 0) {
        dead = true;
        respawn_timer = RESPAWN_DELAY;
        godot::UtilityFunctions::print("mob died");
    }
}

void Mob::_physics_process(double delta) {
    if (!map || !current) return;

    if (dead) {
        respawn_timer -= delta;
        if (respawn_timer <= 0.0) {
            dead = false;
            hp = 3;
            x = spawn_x;
        }
        return;
    }

    x += dir * PATROL_SPEED * delta;

    double lo = std::min(current->start.x, current->end.x);
    double hi = std::max(current->start.x, current->end.x);

    if (x > hi) {
        if (current->next) current = current->next;
        else {
            x = hi;
            dir = -dir;
        }
    }
    else if (x < lo) {
        if (current->prev) current = current->prev;
        else {
            x = lo;
            dir = -dir;
        }
    }

    Player *player = Object::cast_to<Player>(get_parent()->find_child("Player"));
    if (player) {
        godot::Vector2 dp = player->get_position() - get_position();
        bool now = std::abs(dp.x) < 14.0 && dp.y > -10.0 && dp.y < 24.0;
        if (now && !touching) {
            player->take_damage(dp.x > 0 ? 1.0 : -1.0);
            godot::UtilityFunctions::print("mob touched the player!");
        }
        touching = now;
    }

    set_position(godot::Vector2(x, current->y_at(x)));
}

void Mob::_draw() {
    draw_rect(godot::Rect2(-8, -10, 16, 10), godot::Color(0.4, 0.8, 0.3));
}
