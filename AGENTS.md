# Maple — MapleStory-like 2D game

Godot 4.7.2 project with a C++ GDExtension ("maple"). Single-player first;
code is structured so multiplayer could be added later.

## Layout

- `game/` — Godot project (scenes, assets, `maple.gdextension`, `bin/` output)
- `src/` — C++ extension source (auto-globbed by SConstruct, incl. subdirs)
- `godot-cpp/` — cloned binding repo, tag `10.0.0-stable` (NOT committed)
- `editor/` — Godot 4.7.2 editor exe (NOT committed)
- `tools/extractor/` — (planned) WZ → PNG+JSON asset pipeline

## One-time setup

```sh
git clone --depth 1 --branch 10.0.0-stable https://github.com/godotengine/godot-cpp
# editor: https://github.com/godotengine/godot/releases/download/4.7.2-stable/Godot_v4.7.2-stable_win64.exe.zip
pip install scons
```

Requires: Python 3, SCons, MSVC (VS 2022+/VS 18 "Desktop development with C++").

## Build

```sh
scons platform=windows target=template_debug api_version=4.7 -j6
```

`api_version` is REQUIRED by godot-cpp 10.x and must match the editor
minor version (4.7). Output: `game/bin/libmaple.windows.template_debug.x86_64.dll`.

## Run / verify

```sh
# First ever run needs an editor scan so extension_list.cfg gets written:
./editor/Godot_v4.7.2-stable_win64_console.exe --headless --editor --quit --path game
# Then project-mode runs work:
./editor/Godot_v4.7.2-stable_win64_console.exe --headless --path game --quit-after 60
```

Expected output: `HelloNode: hello from C++ (GDExtension loaded)`.
Note: `--editor --quit` currently segfaults on exit AFTER printing — appears
to be a headless-shutdown quirk, not a build problem. Keep an eye on it.

## Editor setup (Zed / clangd)

- `compile_commands.json` is generated (gitignored). Regenerate after adding
  files to `src/`:
  `scons platform=windows target=template_debug api_version=4.7 compiledb -j6`
- `.clangd` adds `--driver-mode=cl` so clangd parses the MSVC-style flags
  in the compile database.
- `.zed/tasks.json` has tasks: build extension, regenerate compile db,
  run project headless (spawn via `task: spawn` in Zed).
- Zed auto-downloads clangd on first C++ file open.

## Gotchas learned

- godot-cpp tags: `godot-X.Y-stable` through 4.5, then switched to own
  versioning (`10.0.0-stable` = current). We use 10.0.0-stable.
- `.gdextension` `windows.debug` key = `template_debug` build artifacts.
- New extension classes need `GDREGISTER_CLASS` in `register_types.cpp`
  AND an editor rescan before project mode sees them.

## Legal

MapleStory assets (WZ data, sprites, audio) are Nexon IP. Extracted assets
live under `game/assets/extracted/` and are gitignored — never commit or
distribute them.
