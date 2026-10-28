#ifndef MAPLE_HELLO_NODE_H
#define MAPLE_HELLO_NODE_H

#include <godot_cpp/classes/node.hpp>

namespace godot {

class HelloNode : public Node {
	GDCLASS(HelloNode, Node)

protected:
	static void _bind_methods();

public:
	HelloNode() = default;
	~HelloNode() override = default;

	void _ready() override;
};

} // namespace godot

#endif
