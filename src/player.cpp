#include "player.h"
#include "map.h"
#include "mob.h"
#include <godot_cpp/classes/input.hpp>

#include <algorithm>
#include <cmath>

void Player::_bind_methods() {
}

void Player::_ready() {
    Map *map_node = Object::cast_to<Map>(get_parent()->find_child("Map"));
    map = &map_node->get_footholds();
    current = &(*map)[0];
    x = current->start.x;

    ladders = &map_node->get_ladders();
}

void Player::take_damage(double dir) {
    if (iframes > 0.0) return;
    hp -= 1;
    iframes = IFRAME_TIME;
    state = State::Airborne;
    y = current->y_at(x);
    vx = dir * 200.0;
    vy = -150.0;
    if (hp <= 0) {
        x = 50.0;
        current = &(*map)[0];
        y = current->y_at(x);
        vx = 0.0;
        vy = 0.0;
        hp = 3;
        state = State::Grounded;
    }
}

void Player::_physics_process(double delta) {
    if (!map || !current) return;

    iframes -= delta;

    godot::Input *input = godot::Input::get_singleton();
    double dir = (input->is_action_pressed("ui_right") ? 1.0 : 0.0)
        -  (input->is_action_pressed("ui_left") ? 1.0 : 0.0);

    if (dir != 0.0) facing = dir;

    switch (state) {
        case State::Grounded: {
            if (input->is_action_just_pressed("attack")) {
                Mob *mob = cast_to<Mob>(get_parent()->find_child("Mob"));
                if (mob && !mob->is_dead()) {
                    double dx = mob->get_position().x - get_position().x;
                    if (dx * facing > 0.0 && std::abs(dx) < ATTACK_REACH) {
                        mob->take_damage(facing);
                    }
                }
            }
            else if (input->is_action_pressed("ui_up")) {
                double feet_y = current->y_at(x);
                for (const Ladder &l : *ladders) {
                    if (std::abs(x - l.x) > GRAB_RANGE) continue;
                    if (feet_y < l.y1 || feet_y > l.y2) continue;
                    rope = &l;
                    x = rope->x;
                    y = feet_y;
                    state = State::OnRope;
                    break;
                }
            }
            else if (input->is_action_just_pressed("ui_accept")) {
                state = State::Airborne;
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
                        state = State::Airborne;
                        x = hi;
                        y = current->y_at(hi);
                        vx = dir * SPEED;
                        vy = 0.0;
                    }
                }
                else if (x < lo) {
                    if (current->prev) current = current->prev;
                    else {
                        state = State::Airborne;
                        x = lo;
                        y = current->y_at(lo);
                        vx = dir * SPEED;
                        vy = 0.0;
                    }
                }
            }
            break;
        }
        case State::Airborne: {
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
                        state = State::Grounded;
                        break;
                    }
                }
            }
            break;
        }
        case State::OnRope: {
            double cdir = (input->is_action_pressed("ui_down") ? 1.0 : 0.0)
                - (input->is_action_pressed("ui_up") ? 1.0 : 0.0);
            y += cdir * CLIMB * delta;

            if (input->is_action_just_pressed("ui_accept")) {
                state = State::Airborne;
                vy = -200.0;
                vx = dir * SPEED;
            }
            else if (y <= rope->y1) {
                current = rope->top_foothold;
                y = rope->y1;
                state = State::Grounded;
            }
            else if (y >= rope->y2) {
                y = rope->y2;
                state = State::Airborne;
                vy = 0.0;
            }
            break;
        }
    }

    set_position(godot::Vector2(x, state == State::Grounded ? current->y_at(x) : y));
}

void Player::_draw() {
    if (iframes > 0.0 && int(iframes * 10) % 2 == 0) return;
    draw_rect(godot::Rect2(-6, -24, 12, 24), godot::Color(1.0, 0.4, 0.2));
}
