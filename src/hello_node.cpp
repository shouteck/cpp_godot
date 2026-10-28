#include "hello_node.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void HelloNode::_bind_methods() {
}

void HelloNode::_ready() {
	UtilityFunctions::print("HelloNode: hello from C++ (GDExtension loaded)");
}

} // namespace godot
