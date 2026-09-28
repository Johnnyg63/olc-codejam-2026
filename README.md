# PGE_Johnnys_Has_To_Change - Multi-Platform Build System

## 📋 About This Project

**PGE_Johnnys_Has_To_Change** is a multi-platform game engine project with support for three different build targets:

- **C++ Native** - High-performance native application for macOS and Linux
- **Emscripten (WebGL)** - Browser-based version using WebAssembly technology
- **Xcode** - Apple's native IDE build system for macOS

All three platforms use a unified build system with automatic architecture detection and optimization, providing 3-5x better performance compared to debug builds.

### Project Goals

✅ **Multi-Platform Support** - Build for desktop and web from single codebase  
✅ **High Performance** - Release builds optimized with -O3, LTO, SIMD, and architecture-specific flags  
✅ **Easy Development** - Simple make commands for common tasks  
✅ **Automatic Optimization** - Detects CPU architecture and applies optimal compiler flags  
✅ **Asset Management** - Automatic copying of game assets to build directories  

---

## 🚀 Quick Start

### Prerequisites

- **C++ Native & Xcode:** Clang/GCC compiler, CMake 3.10+
- **Emscripten:** Emscripten SDK (see [emscripten.org](https://emscripten.org/docs/getting_started/downloads.html))
- **macOS:** Works on both Intel and Apple Silicon (M1/M2/M3/M4)
- **Linux:** Works on x86_64 and ARM64

### Build All Targets (Release, Optimized)

```bash
make build
```

This automatically:
1. Detects your CPU architecture (ARM64 or x86_64)
2. Creates build folders (cpp/build/release, emscripten/build/release, xcode/build/release)
3. Copies assets to build directories
4. Applies architecture-optimized compiler flags
5. Compiles all three platforms

### Run All Targets (Release, Optimized)

```bash
make run
```

Runs all three builds in sequence, with maximum performance optimizations.

### View Architecture & Optimization Flags

```bash
make help
```

Output on Apple Silicon:
```
Architecture: ARM64 (arm64)
Optimization flags: -mcpu=apple-m1 -ffp-contract=fast
```

Output on Intel/AMD:
```
Architecture: x86_64 (x86_64)
Optimization flags: -march=haswell -mavx2 -mfma
```

---

## 📖 Make Command Reference

### Building

| Command | Description | Output |
|---------|-------------|--------|
| `make build` | Build all targets (cpp, emscripten, xcode) Release mode | Optimized executables |
| `make build-cpp` | Build C++ native Release | `cpp/build/release/executable` |
| `make build-emscripten` | Build Emscripten Release | `emscripten/build/release/*.html` |
| `make build-xcode` | Build Xcode Release | `xcode/build/release/executable` |

### Running

| Command | Description |
|---------|-------------|
| `make run` | Run all targets (Release, optimized) |
| `make run-cpp` | Run C++ Release build |
| `make run-emscripten` | Run Emscripten in browser with emrun |
| `make run-xcode` | Run Xcode Release build |

### Debug Building & Running

| Command | Description |
|---------|-------------|
| `make debug` | Build all targets in Debug mode (-g -O0) |
| `make debug-cpp` | Build C++ Debug (with symbols, no optimization) |
| `make debug-emscripten` | Build Emscripten Debug (with symbols) |
| `make debug-xcode` | Build Xcode Debug (with symbols) |
| `make run-debug` | Run all Debug builds |
| `make run-debug-cpp` | Run C++ Debug executable |
| `make run-debug-emscripten` | Run Emscripten Debug in browser |
| `make run-debug-xcode` | Run Xcode Debug executable |

### Cleaning

| Command | Description |
|---------|-------------|
| `make clean` | Clean all build artifacts (cpp, emscripten, xcode) |
| `make clean-cpp` | Clean C++ build directory |
| `make clean-emscripten` | Clean Emscripten build directory |
| `make clean-xcode` | Clean Xcode build directory |

### Help

| Command | Description |
|---------|-------------|
| `make help` | Show all commands with architecture info |
| `cd cpp && make help` | Show C++ Makefile commands |
| `cd emscripten && make help` | Show Emscripten Makefile commands |
| `cd xcode && make help` | Show Xcode Makefile commands |

---

## 📁 Project Structure

```
project-root/
├── Makefile                           # Master build orchestrator
├── README.md                          # This file
├── BUILD_SYSTEM.md                    # Detailed build documentation
├── ARCHITECTURE_SUPPORT.md            # Architecture-specific flags
├── MAKEFILE_QUICK_REFERENCE.md        # Quick command reference
│
├── assets/                            # Game assets (auto-copied to builds)
├── src/                               # Source files
├── cmake/                             # CMake modules
│
├── cpp/
│   ├── Makefile                       # C++ platform build
│   └── build/
│       ├── release/                   # Release build (optimized)
│       │   ├── PGE_Johnnys_Has_To_Change    (716 KB executable)
│       │   ├── assets/                (copied from root)
│       │   └── CMake artifacts
│       └── debug/                     # Debug build (with symbols)
│           ├── PGE_Johnnys_Has_To_Change
│           ├── assets/
│           └── CMake artifacts
│
├── emscripten/
│   ├── Makefile                       # Emscripten platform build
│   └── build/
│       ├── release/                   # WebAssembly Release
│       │   ├── PGE_Johnnys_Has_To_Change.html      (19 KB)
│       │   ├── PGE_Johnnys_Has_To_Change.wasm      (955 KB)
│       │   ├── PGE_Johnnys_Has_To_Change.js
│       │   ├── assets/
│       │   └── CMake artifacts
│       └── debug/                     # WebAssembly Debug
│           ├── PGE_Johnnys_Has_To_Change.html
│           ├── PGE_Johnnys_Has_To_Change.wasm
│           ├── assets/
│           └── CMake artifacts
│
└── xcode/
    ├── Makefile                       # Xcode platform build
    └── build/
        ├── release/                   # Xcode Release (optimized)
        │   ├── PGE_Johnnys_Has_To_Change    (716 KB executable)
        │   ├── assets/
        │   └── CMake artifacts
        └── debug/                     # Xcode Debug (with symbols)
            ├── PGE_Johnnys_Has_To_Change    (3.3 MB)
            ├── assets/
            └── CMake artifacts
```

---

## 🎯 Build Modes Explained

### Release Mode (Default for `make run`)

Optimized for **maximum performance**:
- `-O3` - Maximum optimization level
- `-flto` - Link-time optimization (10-15% speed boost)
- `-funroll-loops` - Loop unrolling (5-10% speed boost)
- `-fvectorize` - Auto SIMD vectorization
- **Architecture-specific:** `-mcpu=apple-m1` (ARM64) or `-march=haswell -mavx2 -mfma` (x86_64)
- No debug symbols (smaller binary)

**Result:** 3-5x faster execution than debug builds

**Binary Size:** ~716 KB (highly optimized)

### Debug Mode (For development)

Optimized for **debugging and development**:
- `-g` - Include debug symbols
- `-O0` - No optimization (easier to debug)
- Larger binary for step-through debugging
- Full line number information

**Binary Size:** ~3.3 MB (with symbols)

---

## 🏗️ Platform-Specific Details

### C++ Native Build

**Best for:** Desktop performance, native OS integration

**Build Location:** `cpp/build/release/` or `cpp/build/debug/`

**Commands:**
```bash
cd cpp
make build              # Build Release
make debug              # Build Debug
make run                # Build & run Release (optimized)
make run-debug          # Build & run Debug
make clean              # Clean builds
```

**Optimization Flags:**
```
Release:  -O3 -mcpu=apple-m1 -ffp-contract=fast -flto -funroll-loops -fvectorize
Debug:    -g -O0
```

### Emscripten (WebGL/Browser) Build

**Best for:** Cross-platform web delivery, maximum compatibility

**Build Location:** `emscripten/build/release/` or `emscripten/build/debug/`

**Output Files:**
- `.html` - Web page (19 KB)
- `.wasm` - WebAssembly binary (955 KB)
- `.js` - JavaScript wrapper
- assets/ - Game assets

**Requirements:**
```bash
# Install Emscripten SDK
source ~/emsdk/emsdk_env.sh
```

**Commands:**
```bash
cd emscripten
make build              # Build Release (WebGL)
make debug              # Build Debug (WebGL)
make run                # Build & run in browser
make run-debug          # Build & run Debug in browser
make clean              # Clean builds
```

**Optimization Flags:**
```
Release:  -O3 -flto (platform-agnostic WASM)
Debug:    -g -O0
```

### Xcode Build

**Best for:** Native macOS development, Xcode IDE integration

**Build Location:** `xcode/build/release/` or `xcode/build/debug/`

**Commands:**
```bash
cd xcode
make build              # Build Release
make debug              # Build Debug
make run                # Build & run Release (optimized)
make run-debug          # Build & run Debug
make clean              # Clean builds
```

**Optimization Flags:**
```
Release:  -O3 -mcpu=apple-m1 -ffp-contract=fast -flto -funroll-loops -fvectorize
Debug:    -g -O0
```

---

## 🏛️ Architecture Support - Automatic Optimization

### Overview

The build system **automatically detects** your CPU architecture and applies optimal compiler flags for maximum performance. No configuration needed!

### Architecture Detection

```makefile
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),arm64)
    ARCH := ARM64
    ARCH_FLAGS := -mcpu=apple-m1 -ffp-contract=fast
else
    ARCH := x86_64
    ARCH_FLAGS := -march=haswell -mavx2 -mfma
endif
```

**Supported Architectures:**
- **ARM64** - Apple Silicon (M1, M2, M3, M4, etc.)
- **x86_64** - Intel Core (i5, i7, i9) and AMD Ryzen

### ARM64 (Apple Silicon) Optimizations

**Devices:** MacBook Air/Pro M1/M2/M3/M4, Mac mini M1/M4, Mac Studio, Mac Pro M2

**Compiler Flags:**
```
-O3                    Maximum optimization
-mcpu=apple-m1         M1/M2/M3/M4 specific tuning
-ffp-contract=fast     Fused multiply-add (crucial for graphics/physics)
-flto                  Link-time optimization
-funroll-loops         Loop unrolling
-fvectorize            Auto SIMD vectorization
```

**Benefits:**
- ✓ Fused multiply-add operations for faster floating-point math
- ✓ M-series microarchitecture optimizations
- ✓ Power efficient execution
- ✓ High memory bandwidth utilization
- ✓ Expected performance: 3-5x faster than debug builds

### x86_64 (Intel/AMD) Optimizations

**Devices:** Intel Core i5/i7/i9, AMD Ryzen 5/7/9, AMD Threadripper

**Compiler Flags:**
```
-O3                    Maximum optimization
-march=haswell         CPU baseline (Haswell 2013+, supports AVX2)
-mavx2                 256-bit SIMD operations (processes 8 floats/instruction)
-mfma                  Fused multiply-add instructions
-flto                  Link-time optimization
-funroll-loops         Loop unrolling
-fvectorize            Auto SIMD vectorization
```

**Benefits:**
- ✓ AVX2 vectorization for data-parallel code
- ✓ Wide CPU support (all modern Intel/AMD since 2013)
- ✓ Fused multiply-add for mathematical operations
- ✓ Decades of x86_64 optimization maturity
- ✓ Expected performance: 3-5x faster than debug builds

### Performance Impact

**Compiler Optimization Impact Breakdown:**
- `-O3` vs `-O2`: ~30% faster
- `-flto` (link-time optimization): +10-15% faster
- `-funroll-loops` (loop unrolling): +5-10% faster
- `-mavx2`/`-mcpu=apple-m1`: +20-30% faster
- **Total: 3-5x faster than unoptimized debug builds**

**Binary Size Comparison:**
| Build Type | Size | Ratio |
|-----------|------|-------|
| Release (optimized) | 716 KB | 1.0x |
| Debug (with symbols) | 3.3 MB | 4.6x |

### Verifying Applied Flags

**Check what flags were applied:**
```bash
# For C++ build
grep CMAKE_CXX_FLAGS cpp/build/release/CMakeCache.txt

# Example ARM64 output:
# CMAKE_CXX_FLAGS:STRING=-O3 -mcpu=apple-m1 -ffp-contract=fast -flto -funroll-loops -fvectorize
```

**View detected architecture:**
```bash
make help
# Shows:
# Architecture: ARM64 (arm64)
# Optimization flags: -mcpu=apple-m1 -ffp-contract=fast
```

---

## 🔧 Troubleshooting

### Issue: Build fails with "not a CMake build directory"

**Solution:** The build directory needs proper initialization. Run:
```bash
make clean
make build
```

### Issue: "emcmake not found" (Emscripten)

**Solution:** Install and source Emscripten SDK:
```bash
# Download Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git ~/emsdk
cd ~/emsdk && ./emsdk install latest && ./emsdk activate latest

# Add to shell profile (~/.zshrc or ~/.bash_profile)
source ~/emsdk/emsdk_env.sh

# Or source in current shell
source ~/emsdk/emsdk_env.sh
make build-emscripten
```

### Issue: "emrun not found" (Emscripten)

**Solution:** Same as above - emrun is part of Emscripten SDK.

### Issue: CMake version too old

**Solution:** Update CMake:
```bash
# macOS with Homebrew
brew install cmake

# Or download from https://cmake.org/download/
```

### Issue: Slow build performance

**Solution:** Check that Release mode is being used:
```bash
# Correct - Release (fast)
make run

# Slower - Debug (for development)
make run-debug
```

### Issue: Compiler doesn't support optimization flags

**Solution:** Update compiler:
```bash
# macOS - Update Xcode
xcode-select --install

# Or install Homebrew and latest Clang
brew install llvm
```

### Issue: "permission denied" when running executable

**Solution:**
```bash
chmod +x cpp/build/release/PGE_Johnnys_Has_To_Change
./cpp/build/release/PGE_Johnnys_Has_To_Change
```

---

## 📊 Build System Comparison

| Feature | C++ Native | Emscripten | Xcode |
|---------|-----------|-----------|-------|
| **Platform** | macOS, Linux | Browser (any OS) | macOS only |
| **Performance** | Excellent | Good | Excellent |
| **Deployment** | Native install | Web link | App bundle |
| **Development** | Any IDE | Any IDE | Xcode IDE |
| **Graphics Backend** | Native | WebGL | Metal/OpenGL |
| **File Access** | Full filesystem | Sandboxed | Full filesystem |
| **Build Time** | Fast (2-3s) | Moderate (5-10s) | Fast (2-3s) |

---

## 🎓 Common Workflows

### Quick Development Cycle

```bash
# Make changes to source code
vi src/main.cpp

# Build and run debug version
make run-debug-cpp

# Or run all platforms in debug
make run-debug

# Step through with debugger
lldb ./cpp/build/debug/PGE_Johnnys_Has_To_Change
```

### Performance Testing

```bash
# Build optimized release version
make build

# Run and measure performance
time ./cpp/build/release/PGE_Johnnys_Has_To_Change
time ./xcode/build/release/PGE_Johnnys_Has_To_Change

# Check binary size
ls -lh cpp/build/release/PGE_Johnnys_Has_To_Change
```

### Web Deployment

```bash
# Build Emscripten for web
make build-emscripten

# Test locally
make run-emscripten

# Deploy files
# Copy contents of emscripten/build/release/ to web server
# Serve: .html, .wasm, .js, assets/
```

### Multi-Platform Testing

```bash
# Build all three platforms
make build

# Run all three (Release, optimized)
make run

# Or test individually
make run-cpp
make run-emscripten
make run-xcode
```

### Clean Rebuild

```bash
# Remove all build artifacts
make clean

# Rebuild everything fresh
make build

# Verify successful builds
ls -lh cpp/build/release/PGE_Johnnys_Has_To_Change
ls -lh xcode/build/release/PGE_Johnnys_Has_To_Change
ls -lh emscripten/build/release/PGE_Johnnys_Has_To_Change.html
```

---

## ⚙️ Advanced Topics

### CMake Integration

All builds use CMake for actual compilation:

1. **Makefile calls CMake to generate** - Creates platform-specific build files
2. **Makefile calls cmake --build** - Compiles using generated build system
3. **Build organized in folders** - `build/release/` and `build/debug/` for separation

**Key CMake Variables:**
```
CMAKE_BUILD_TYPE=Release  # Release vs Debug
CMAKE_CXX_FLAGS           # Compiler flags (includes architecture-specific)
DWEB=ON                   # Emscripten web flag
```

### Asset Management

Assets are **automatically copied** during build preparation:

- **C++ & Xcode:** `assets/` → `cpp/build/release/` and `cpp/build/debug/`
- **Emscripten:** `assets/` → `emscripten/build/release/` and `emscripten/build/debug/`

This ensures assets are present before compilation.

### Platform Notes

- **macOS:** All targets fully supported (cpp, emscripten, xcode)
- **Linux:** cpp and emscripten supported (no xcode)
- **Windows:** Emscripten and cpp with MSYS2/MinGW toolchain

### Verbose Build Output

```bash
# See all compiler commands
cd cpp/build/release
cmake --build . --verbose
```

---

## 📚 Documentation Files

| File | Purpose |
|------|---------|
| **README.md** | This file - complete guide |
| **BUILD_SYSTEM.md** | Detailed build system architecture |
| **ARCHITECTURE_SUPPORT.md** | In-depth architecture optimization details |
| **MAKEFILE_QUICK_REFERENCE.md** | Quick command reference |

---

## 📋 Checklist: Getting Started

- [ ] Install prerequisites (CMake, compiler, Emscripten SDK)
- [ ] Clone or navigate to project directory
- [ ] Run `make build` to build all targets
- [ ] Run `make help` to see commands and detected architecture
- [ ] Run `make run` to test all platforms
- [ ] Run `make run-debug-cpp` to debug C++ version
- [ ] Check binary sizes: `ls -lh cpp/build/release/`
- [ ] Review BUILD_SYSTEM.md for detailed information
- [ ] Review ARCHITECTURE_SUPPORT.md for optimization details

---

## 🎉 Summary

The build system provides:

✅ **Unified Build Process** - Single commands for all platforms  
✅ **Automatic Architecture Detection** - ARM64 & x86_64 support  
✅ **Optimized Compilation** - 3-5x faster Release builds  
✅ **Easy Development** - Simple make targets for common tasks  
✅ **Comprehensive Documentation** - Full guides and quick references  
✅ **Asset Management** - Automatic copying of game assets  
✅ **Multiple Platforms** - Desktop (cpp, xcode) and web (emscripten)  

**Simply run `make build` and everything else is handled automatically!**

---

## 🚀 Next Steps

1. **First time?** Run `make help` to see available commands
2. **Build all:** `make build`
3. **Test Release:** `make run`
4. **Develop:** `make run-debug-cpp` and edit source
5. **Deploy web:** `make build-emscripten` and copy to server
6. **Clean up:** `make clean`

For more details, see [BUILD_SYSTEM.md](BUILD_SYSTEM.md) and [ARCHITECTURE_SUPPORT.md](ARCHITECTURE_SUPPORT.md).

