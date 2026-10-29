#ifndef MAPLE_PLAYER_H
#define MAPLE_PLAYER_H

#include "foothold.h"
#include "ladder.h"

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
	void take_damage(double dir);

private:
	const std::vector<Foothold> *map = nullptr;
	const Foothold *current = nullptr;
	double x = 0.0;

	double vx = 0.0;
	double vy = 0.0;
	double y = 0.0;

	static constexpr double SPEED = 200.0;
	static constexpr double GRAVITY = 1500.0;

	static constexpr double JUMP = 800.0;

	enum class State {
	    Grounded,
		Airborne,
		OnRope
	};
	State state = State::Grounded;
	const std::vector<Ladder> *ladders = nullptr;
	const Ladder *rope = nullptr;

	static constexpr double CLIMB = 150.0;
	static constexpr double GRAB_RANGE = 15.0;

	double facing = 1.0;
	int hp = 3;
	double iframes = 0.0;

	static constexpr double IFRAME_TIME = 1.0;
	static constexpr double ATTACK_REACH = 40.0;
};

#endif
