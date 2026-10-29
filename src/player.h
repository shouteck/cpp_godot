#ifndef MAPLE_PLAYER_H
#define MAPLE_PLAYER_H

#include "foothold.h"

#include <godot_cpp/classes/node2d.hpp>
#include <vector>

class Player : public godot::Node2D {
	GDCLASS(Player, Node2D)

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

	bool grounded = true;
	double vx = 0.0;
	double vy = 0.0;
	double y = 0.0;

	static constexpr double SPEED = 200.0;
	static constexpr double GRAVITY = 1500.0;

	static constexpr double JUMP = 800.0;
};

#endif
