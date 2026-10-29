#ifndef MAPLE_MOB_H
#define MAPLE_MOB_H

#include "foothold.h"

#include <godot_cpp/classes/node2d.hpp>
#include <vector>

class Mob : public godot::Node2D {
	GDCLASS(Mob, Node2D)

protected:
	static void _bind_methods();

public:
	void _ready() override;
	void _draw() override;
	void _physics_process(double delta) override;
	void take_damage(double dir);
	bool is_dead() const {
	    return dead;
	}
	double get_x() const {
	    return x;
	}

private:
	const std::vector<Foothold> *map = nullptr;
	const Foothold *current = nullptr;
	double x = 0.0;

	double dir = 1.0;
	bool touching = false;

	static constexpr double PATROL_SPEED = 60.0;

	int hp = 3;
	bool dead = false;
	double respawn_timer = 0.0;
	double spawn_x = 150.0;
	static constexpr double RESPAWN_DELAY = 3.0;
	static constexpr double KNOCKBACK = 40.0;
};

#endif
