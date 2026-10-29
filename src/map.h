#ifndef MAPLE_MAP_H
#define MAPLE_MAP_H

#include "foothold.h"
#include "ladder.h"

#include <godot_cpp/classes/node2d.hpp>

#include <unordered_map>
#include <vector>

class Map : public godot::Node2D {
	GDCLASS(Map, Node2D)

protected:
	static void _bind_methods();

public:
	void _ready() override;
	void _draw() override;
	void load(const godot::String &path);

	const std::vector<Foothold>& get_footholds() const;
	const std::vector<Ladder>& get_ladders() const;

private:
    std::vector<Foothold> footholds;
    std::vector<Ladder> ladders;
    std::unordered_map<int, int> fh_index;
};

#endif
