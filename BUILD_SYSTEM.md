# Build System Documentation

## Overview

This project uses a hierarchical makefile system with:
- **Master Makefile** (root) - Orchestrates all three build targets (cpp, emscripten, xcode)
- **CPP Makefile** (cpp/) - C++ native build with optimization flags
- **Emscripten Makefile** (emscripten/) - WebGL build for browser
- **Xcode Makefile** (xcode/) - Xcode/macOS native build

## Project Structure

```
.
├── Makefile              # Master makefile
├── assets/               # Game assets (copied to build directories)
├── src/                  # Source files
├── cmake/                # CMake modules
├── cpp/
│   ├── Makefile
│   ├── build/
│   │   ├── release/      # Release build artifacts + assets
│   │   └── debug/        # Debug build artifacts + assets
│   └── CMakeFiles/       # CMake generated
├── emscripten/
│   ├── Makefile
│   ├── build/
│   │   ├── release/      # Release build artifacts + assets
│   │   └── debug/        # Debug build artifacts + assets
│   └── CMakeFiles/       # CMake generated
└── xcode/
    ├── Makefile
    └── assets/           # Assets copied directly to xcode folder
```

## Master Makefile Commands

### Building

```bash
make build              # Build all targets (cpp, emscripten, xcode)
make build-cpp          # Build C++ native (Release, optimized)
make build-emscripten   # Build Emscripten (Release, optimized)
make build-xcode        # Build Xcode (Release, optimized)
```

### Running (Release builds with full optimization)

```bash
make run                # Run all builds sequentially
make run-cpp            # Run C++ native version
make run-emscripten     # Run Emscripten in browser (uses emrun)
make run-xcode          # Run Xcode version
```

### Debug

```bash
make debug              # Build all in Debug mode
make debug-cpp          # Build C++ Debug
make debug-emscripten   # Build Emscripten Debug
make debug-xcode        # Build Xcode Debug
```

### Debug Execution

```bash
make run-debug          # Run all Debug builds
make run-debug-cpp      # Run C++ Debug
make run-debug-emscripten  # Run Emscripten Debug in browser
make run-debug-xcode    # Run Xcode Debug
```

### Cleaning

```bash
make clean              # Clean all build artifacts
make clean-cpp          # Clean C++ build
make clean-emscripten   # Clean Emscripten build
make clean-xcode        # Clean Xcode build
```

### Help

```bash
make help               # Show help (default)
```

## C++ Native Build (cpp/)

### Structure
- **build/release/** - Release build with full optimization
- **build/debug/** - Debug build with symbols

### Optimization Flags (Release)
- `-O3` - Maximum optimization
- `-march=native` - Native CPU architecture optimizations
- `-flto` - Link-time optimization
- `-funroll-loops` - Loop unrolling for performance
- `-fvectorize` - SIMD vectorization (auto)

### Usage
```bash
cd cpp
make build              # Build Release
make debug              # Build Debug
make run                # Build & run Release
make run-debug          # Build & run Debug
make clean              # Clean builds
make help               # Show C++ makefile help
```

## Emscripten Build (emscripten/)

### Structure
- **build/release/** - Release build with HTML output
- **build/debug/** - Debug build with HTML output

### Output
- HTML file: `build/release/PGE_Johnnys_Has_To_Change.html`
- Debug HTML: `build/debug/PGE_Johnnys_Has_To_Change.html`

### Requirements
- Emscripten SDK installed
- `emcmake` command available
- `emrun` command available

### Optimization Flags (Release)
- `-O3` - Maximum optimization
- `-flto` - Link-time optimization
- `-DWEB=ON` - Web build flag

### Usage
```bash
cd emscripten
make build              # Build Release (WebGL)
make debug              # Build Debug (WebGL)
make run                # Build & run Release in browser
make run-debug          # Build & run Debug in browser
make clean              # Clean builds
make help               # Show Emscripten makefile help
```

## Xcode Build (xcode/)

### Structure
- Creates `.app` bundle
- Assets copied directly to xcode folder

### Optimization Flags (Release)
- `-O3` - Maximum optimization
- `-march=native` - Native CPU architecture optimizations
- `-flto` - Link-time optimization
- `-funroll-loops` - Loop unrolling
- `-fvectorize` - SIMD vectorization

### Usage
```bash
cd xcode
make build              # Build Release
make debug              # Build Debug
make run                # Build & run Release
make run-debug          # Build & run Debug
make clean              # Clean builds
make help               # Show Xcode makefile help
```

## Asset Management

Assets are automatically copied during build preparation:
- **cpp/build/release/** and **cpp/build/debug/** - Assets copied here
- **emscripten/build/release/** and **emscripten/build/debug/** - Assets copied here
- **xcode/** - Assets copied directly to xcode folder

The build system ensures assets are present before compilation.

## Build Modes

### Release Mode (Default for `make run`)
- Maximum optimization (`-O3`)
- Link-time optimization (`-flto`)
- Loop unrolling (`-funroll-loops`)
- Auto SIMD vectorization
- Performance prioritized over file size
- No debug symbols

### Debug Mode
- Debug symbols enabled (`-g`)
- No optimization (`-O0`)
- Easier debugging with breakpoints
- Larger executable

## Performance Tuning

The Release builds are configured for maximum performance:

**C++ and Xcode:**
- `-march=native` - Uses all available CPU features (SIMD, AVX, etc.)
- `-funroll-loops` - Unrolls loops for better instruction caching
- `-flto` - Link-time optimization for cross-file optimization
- `-fvectorize` - Auto-vectorization with SIMD instructions

**Emscripten:**
- `-O3` - Aggressive JavaScript optimization
- `-flto` - Link-time optimization
- WebGL backend for graphics

## Troubleshooting

### Build fails with "emcmake not found"
```bash
# Install Emscripten SDK
# Follow: https://emscripten.org/docs/getting_started/downloads.html
source ~/emsdk/emsdk_env.sh
```

### Clean build
```bash
make clean
make build-cpp  # or build-emscripten, build-xcode
```

### Incremental rebuild
```bash
make build-cpp  # Rebuilds only changed files
```

### Debug a build
```bash
cd cpp/build/release
cmake --build . --verbose
```

## Platform Notes

- **macOS**: All targets supported (cpp, emscripten, xcode)
- **Linux**: cpp and emscripten supported
- **Windows**: Emscripten and cpp with appropriate toolchain

## CMake Integration

All builds use CMake for actual compilation:
1. Makefile calls `cmake` to generate build system
2. Makefile calls `cmake --build` to compile
3. Build files organized in `build/release` and `build/debug`

## Next Steps

1. Build all targets:
   ```bash
   make build
   ```

2. Run Release builds:
   ```bash
   make run
   ```

3. Debug specific target:
   ```bash
   make run-debug-cpp
   ```

4. Clean everything:
   ```bash
   make clean
   ```
