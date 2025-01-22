# The Tomos Game Engine

The Tomos Game Engine is a 2D/3D game engine written in C++ and OpenGL. It is designed to be simple and easy to use,
while still being powerful and flexible. The engine is still in development.

---

Right now the engine is in a very early stage of development, and is not yet ready for use. Hence, it's not even being
built as a library yet. The engine is being developed on Linux, but it should be possible to build it on Windows and
MacOS as well.

## Planned Features

- 2D and 3D rendering
- Physics engine
- Audio engine
- Input handling
- Scene management
- Scripting support
- Networking support ??
- Editor ??
- Cross-platform support (Windows, Linux, MacOS)
- more...

## Building

```bash
git clone git@github.com:Life-Path-Game/TomosEngine.git
cd TomosEngine
```

```bash
mkdir build
cd build
```

##### Debug

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

##### Release

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
