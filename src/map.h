#ifndef MAPLE_MAP_H
#define MAPLE_MAP_H

#include "foothold.h"
#include "godot_cpp/variant/rect2.hpp"
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

	const std::vector<Foothold>& get_footholds() const {
	    return footholds;
	}
	const std::vector<Ladder>& get_ladders() const {
	    return ladders;
	}

	godot::Rect2 get_bounds() const {
	    return bounds;
	}

private:
    std::vector<Foothold> footholds;
    std::vector<Ladder> ladders;
    std::unordered_map<int, int> fh_index;

    godot::Rect2 bounds;
};

#endif
