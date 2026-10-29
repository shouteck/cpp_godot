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

private:
	const std::vector<Foothold> *map = nullptr;
	const Foothold *current = nullptr;
	double x = 0.0;

	double dir = 1.0;
	bool touching = false;

	static constexpr double PATROL_SPEED = 60.0;
};

#endif
