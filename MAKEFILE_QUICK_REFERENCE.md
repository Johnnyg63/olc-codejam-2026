# Quick Reference Guide

## Master Makefile (Root Directory)

### Quick Start
```bash
# Build all platforms
make build

# Run all platforms (Release, optimized)
make run

# Clean everything
make clean
```

### Single Platform Commands
```bash
# C++ Native
make build-cpp          # Build C++
make run-cpp            # Run C++ (optimized)
make debug-cpp          # Debug build
make run-debug-cpp      # Run debug
make clean-cpp          # Clean

# Emscripten/WebGL
make build-emscripten   # Build WebGL
make run-emscripten     # Run in browser
make debug-emscripten   # Debug build
make run-debug-emscripten
make clean-emscripten   # Clean

# Xcode/macOS
make build-xcode        # Build
make run-xcode          # Run (optimized)
make debug-xcode        # Debug build
make run-debug-xcode    # Run debug
make clean-xcode        # Clean
```

## Platform-Specific Makefiles

### C++ (cpp/Makefile)
```bash
cd cpp
make              # Build release (O3, native arch, LTO, unrolling)
make debug        # Debug build
make run          # Build & run release
make run-debug    # Build & run debug
make clean        # Clean build/
make help         # Show options
```

### Emscripten (emscripten/Makefile)
```bash
cd emscripten
make              # Build release (WebGL, optimized)
make debug        # Debug build
make run          # Build & run in browser with emrun
make run-debug    # Debug build & run
make clean        # Clean build/
make help         # Show options
```

### Xcode (xcode/Makefile)
```bash
cd xcode
make              # Build release (O3, native arch, LTO, unrolling)
make debug        # Debug build
make run          # Build & run release
make run-debug    # Build & run debug
make clean        # Clean xcode build
make help         # Show options
```

## Directory Structure After Build

```
project/
├── cpp/
│   └── build/
│       ├── release/          # Optimized executable + assets
│       │   ├── PGE_Johnnys_Has_To_Change
│       │   └── assets/
│       └── debug/            # Debug executable + assets
│           ├── PGE_Johnnys_Has_To_Change
│           └── assets/
├── emscripten/
│   └── build/
│       ├── release/          # Optimized HTML + assets
│       │   ├── PGE_Johnnys_Has_To_Change.html
│       │   ├── PGE_Johnnys_Has_To_Change.js
│       │   ├── PGE_Johnnys_Has_To_Change.wasm
│       │   └── assets/
│       └── debug/            # Debug HTML + assets
│           ├── PGE_Johnnys_Has_To_Change.html
│           ├── PGE_Johnnys_Has_To_Change.js
│           ├── PGE_Johnnys_Has_To_Change.wasm
│           └── assets/
└── xcode/
    ├── assets/               # Assets copied here
    └── PGE_Johnnys_Has_To_Change.app  # macOS app bundle
```

## Optimization Flags

### Release Mode (make run)
- **C++ & Xcode:**
  - `-O3` Maximum optimization
  - `-march=native` CPU-specific optimizations
  - `-flto` Link-time optimization
  - `-funroll-loops` Loop unrolling
  - `-fvectorize` Auto SIMD

- **Emscripten:**
  - `-O3` Aggressive JS optimization
  - `-flto` Link-time optimization
  - WebGL backend

### Debug Mode (make run-debug)
- `-g` Debug symbols
- `-O0` No optimization
- Best for step-through debugging

## Common Tasks

| Task | Command |
|------|---------|
| Build everything | `make build` |
| Run everything optimized | `make run` |
| Run C++ only | `make run-cpp` |
| Run in browser | `make run-emscripten` |
| Debug C++ | `make run-debug-cpp` |
| Clean all | `make clean` |
| Clean C++ only | `make clean-cpp` |
| Full rebuild | `make clean && make build` |

## Requirements

### All Targets
- CMake 3.15+
- C++ compiler (clang, g++, or MSVC)

### C++ & Xcode
- macOS or Linux
- Standard C++ toolchain

### Emscripten
- Emscripten SDK installed
- `emcmake` and `emrun` in PATH
- Setup: `source ~/emsdk/emsdk_env.sh`

## Build Performance

**Release builds prioritize performance:**
- `make run-cpp` uses `-O3 -march=native -flto -funroll-loops`
- Expected 2-3x faster than debug builds
- Larger binary size (but optimized)

**Debug builds prioritize debugging:**
- `make run-debug-cpp` uses `-g -O0`
- Smaller binary, faster compile
- Step-through debugging available

## Help

```bash
# Master Makefile help
make help

# Platform-specific help
cd cpp && make help
cd emscripten && make help
cd xcode && make help
```

## Troubleshooting

| Issue | Solution |
|-------|----------|
| emcmake not found | `source ~/emsdk/emsdk_env.sh` |
| CMake errors | Run `make clean` then `make build` |
| Assets missing | Makefiles auto-copy from `assets/` |
| Permission denied | Check executable permissions in build/ |

## Tips

1. **Parallel builds**: Makefiles don't use `-j`, add to `~/.make.env` or `CMAKE_BUILD_PARALLEL_LEVEL`
2. **Verbose output**: Add `VERBOSE=1` → `make VERBOSE=1 build-cpp`
3. **Specific target**: Use platform Makefile directly → `cd cpp && make run`
4. **Incremental**: Subsequent `make run` only rebuilds changed files
5. **Full rebuild**: `make clean && make build` (removes all artifacts)
