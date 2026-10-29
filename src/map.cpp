#include "map.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

void Map::_bind_methods() {
}

void Map::_ready() {
    load("C:/Users/shout/Desktop/Projects/cpp_godot/tools/extractor/out/100020000.json");
}

void Map::_draw() {
    for (const Foothold &fh : footholds) {
        draw_line(fh.start, fh.end, godot::Color(1, 1, 1), 2.0);
    }
    for (const Ladder &l : ladders) {
        draw_line(godot::Vector2(l.x, l.y1), godot::Vector2(l.x, l.y2),
                  godot::Color(0.4, 0.8, 1.0), 2.0);
    }
}

void Map::load(const godot::String &path) {
    auto f = godot::FileAccess::open(path, godot::FileAccess::READ);
    if (f.is_null()) {
        ERR_FAIL_MSG("map json missing");
    }
    godot::String text = f->get_as_text();

    godot::Ref<godot::JSON> json;
    json.instantiate();
    if (json->parse(text) != godot::OK) {
        ERR_FAIL_MSG("bad json");
    }
    godot::Dictionary root = json->get_data();

    godot::Array fhs = root["footholds"];
    for (int i = 0; i < fhs.size(); i++) {
        godot::Dictionary d = fhs[i];
        int id = d["id"];
        fh_index[id] = footholds.size();
        footholds.push_back(Foothold{id,
            godot::Vector2((double)d["x1"], (double)d["y1"]),
            godot::Vector2((double)d["x2"], (double)d["y2"]),
            nullptr,
            nullptr
        });
    }
}

const std::vector<Foothold>& Map::get_footholds() const {
    return footholds;
}

const std::vector<Ladder>& Map::get_ladders() const {
    return ladders;
}
