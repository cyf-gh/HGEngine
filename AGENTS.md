# HGEngine Agent Guidelines

This document provides guidelines for AI agents working on the HGEngine codebase.

## Project Overview

HGEngine is a Unity-like 2D game engine built in C++11 using SDL2. The project uses CMake for build management and MSVC as the primary compiler.

## Build Commands

### CMake + MinGW (experimental)
```bash
cmake -G "MinGW Makefiles" -B "./build" -S "./src"
cd build && make
```

### MSVC (recommended)
Open `msvc/HoneyGame.sln` in Visual Studio and build.

### Running the Application
```bash
start build/hg
```

### Running Tests
Tests use a custom framework in `src/Core/Test.hpp`. No automated test runner is configured. Tests are executed manually using:
- `HG_TEST_START("TestName")` - Start a test
- `HG_TEST_ASSERT_TRUE(expression, info)` - Assert true
- `HG_TEST_ASSERT_FALSE(expression, info)` - Assert false

## Code Style Guidelines

### File Organization
- Headers: `src/Engine/*.h` for public API, `src/EngineImpl/*.h` for internal
- Implementations: `src/EngineImpl/*.cpp`
- Core: `src/Core/*.h` and `src/Core/*.cc`
- Use both `#pragma once` and header guards: `#ifndef HONEYGAME_<FILENAME>_H`

### Naming Conventions
- **Namespaces**: `HG` (core), `HGEngine::V1SDL` (engine impl)
- **Classes**: PascalCase (e.g., `GameObject`, `HGComponent`)
- **Methods/Functions**: PascalCase (e.g., `GetName`, `SetParent`)
- **Member Variables**: `m_` prefix (e.g., `m_pGameObject`, `mStrName`)
- **Pointers**: `p` prefix (e.g., `pScene`, `pComp`)
- **Static Variables**: `um` prefix for maps (e.g., `umTheseOnes`)

### Type Definitions
Use custom integer types from `src/Core/Type.h`:
- `ch8`, `n8`, `n16`, `n32`, `n64` (signed)
- `un8`, `un16`, `un32`, `un64` (unsigned)
- `f32`, `f64` (floating point)
- `HGResult` for return codes

### Input Modifiers
- `IN` - const input
- `OUT` - output parameter (can be null, function allocates)
- `IN_OUT` - must be valid input/output

### Indentation
- Use tabs for indentation
- Opening braces on same line as control statements

### Error Handling
- Use `HG_ASSERT(expression)` for debug assertions
- Use `HG_CHECK_ERR(func)` with `HG_RESULT_RES` for error checking
- Return `HGResult` from functions (0### Macros
- ` = success)

HG_INLINE` - platform-safe inline keyword
- `HG_ERASE_IN_VEC(ptr, vec)` - remove from vector
- `HG_EVENT_BIND(obj, event)` - bind event handler
- `HG_COMPONENT_RENDERABLE` - mark component as renderable
- `HG_COMPONENT_MUTILABLE` - allow multiple instances per GameObject

### Includes
- Use angle brackets for system/third-party: `#include <string>`
- Use quotes for local: `#include "HGObject.h"`
- Order: system, third-party, local (separated by blank lines)

### Best Practices
1. Use `explicit` for single-argument constructors
2. Prefer `nullptr` over `NULL`
3. Use `static_cast<T>`, `dynamic_cast<T>` for C++ casts
4. Use `typeid()` for runtime type checking
5. Prefix implementation files with module name (e.g., `Collision.cpp` has `HGEngine::V1SDL::BoxCollision`)
6. Components should inherit from `HGComponent`
7. Behaviors should inherit from `HGBehaviour`
8. GameObjects manage their Transform automatically

## Key Files
- `src/Engine/HG.h` - Main entry point definitions
- `src/Engine/HGObject.h` - Base object with ID system
- `src/Engine/HGComponent.h` - Component base class
- `src/Engine/HGBehaviour.h` - Behavior/event system
- `src/EngineImpl/GameObject.h` - Game object implementation
- `src/Core/Type.h` - Type definitions
- `src/Core/Error.h` - Error handling macros

## Third-Party Libraries
- SDL2 (graphics/input)
- SDL2_image, SDL2_ttf
- RapidJSON (serialization)
- nameof (type name reflection)
- UDRefl (reflection)
- imgui (debug UI)
