.PHONY: all clean build debug run run-debug help \
        build-all build-cpp build-emscripten build-xcode \
        clean-all clean-cpp clean-emscripten clean-xcode \
        run-all run-cpp run-emscripten run-xcode \
        debug-all debug-cpp debug-emscripten debug-xcode \
        run-debug-all run-debug-cpp run-debug-emscripten run-debug-xcode

# Project settings
PROJECT_NAME := PGE_Johnnys_Has_To_Change
ASSETS_SRC := assets
ROOT_DIR := $(shell pwd)

# Build directories
CPP_DIR := cpp
EMSCRIPTEN_DIR := emscripten
XCODE_DIR := xcode

# Colors for output
GREEN := \033[0;32m
YELLOW := \033[0;33m
BLUE := \033[0;34m
RED := \033[0;31m
NC := \033[0m # No Color

help:
	@echo "$(BLUE)=== Makefile for $(PROJECT_NAME) ===$(NC)"
	@echo ""
	@echo "$(GREEN)Build targets:$(NC)"
	@echo "  make build             - Build all targets (cpp, emscripten, xcode)"
	@echo "  make build-cpp         - Build C++ native version (Release)"
	@echo "  make build-emscripten  - Build Emscripten WebGL version (Release)"
	@echo "  make build-xcode       - Build Xcode native version (Release)"
	@echo ""
	@echo "$(GREEN)Run targets (Release builds):$(NC)"
	@echo "  make run               - Run all targets"
	@echo "  make run-cpp           - Run C++ native version"
	@echo "  make run-emscripten    - Run Emscripten in browser with emrun"
	@echo "  make run-xcode         - Run Xcode native version"
	@echo ""
	@echo "$(GREEN)Debug targets:$(NC)"
	@echo "  make debug             - Build all in Debug mode"
	@echo "  make debug-cpp         - Build C++ in Debug mode"
	@echo "  make debug-emscripten  - Build Emscripten in Debug mode (if supported)"
	@echo "  make debug-xcode       - Build Xcode in Debug mode"
	@echo ""
	@echo "$(GREEN)Run Debug targets:$(NC)"
	@echo "  make run-debug         - Run all in Debug mode"
	@echo "  make run-debug-cpp     - Run C++ Debug build"
	@echo "  make run-debug-emscripten - Run Emscripten Debug build"
	@echo "  make run-debug-xcode   - Run Xcode Debug build"
	@echo ""
	@echo "$(GREEN)Clean targets:$(NC)"
	@echo "  make clean             - Clean all build artifacts"
	@echo "  make clean-cpp         - Clean C++ build"
	@echo "  make clean-emscripten  - Clean Emscripten build"
	@echo "  make clean-xcode       - Clean Xcode build"

# ==================== ALL TARGETS ====================
all: build

build: build-cpp build-emscripten build-xcode
	@echo "$(GREEN)✓ All builds completed successfully$(NC)"

clean: clean-cpp clean-emscripten clean-xcode
	@echo "$(GREEN)✓ All cleanup completed$(NC)"

debug: debug-cpp debug-emscripten debug-xcode
	@echo "$(GREEN)✓ All debug builds completed$(NC)"

run: run-cpp run-emscripten run-xcode
	@echo "$(GREEN)✓ All runs completed$(NC)"

run-debug: run-debug-cpp run-debug-emscripten run-debug-xcode
	@echo "$(GREEN)✓ All debug runs completed$(NC)"

# ==================== C++ NATIVE TARGETS ====================
build-cpp: prepare-cpp-release
	@echo "$(YELLOW)Building C++ native (Release)...$(NC)"
	@cd $(CPP_DIR) && cmake --build build/release --config Release
	@echo "$(GREEN)✓ C++ Release build complete$(NC)"

clean-cpp:
	@echo "$(YELLOW)Cleaning C++ builds...$(NC)"
	@rm -rf $(CPP_DIR)/build
	@echo "$(GREEN)✓ C++ cleanup complete$(NC)"

debug-cpp: prepare-cpp-debug
	@echo "$(YELLOW)Building C++ native (Debug)...$(NC)"
	@cd $(CPP_DIR) && cmake --build build/debug --config Debug
	@echo "$(GREEN)✓ C++ Debug build complete$(NC)"

run-cpp: build-cpp
	@echo "$(YELLOW)Running C++ native (Release)...$(NC)"
	@cd $(ROOT_DIR) && ./$(CPP_DIR)/build/release/$(PROJECT_NAME)

run-debug-cpp: debug-cpp
	@echo "$(YELLOW)Running C++ native (Debug)...$(NC)"
	@cd $(ROOT_DIR) && ./$(CPP_DIR)/build/debug/$(PROJECT_NAME)

prepare-cpp-release:
	@mkdir -p $(CPP_DIR)/build/release
	@cp -r $(ASSETS_SRC)/* $(CPP_DIR)/build/release/ 2>/dev/null || true
	@cd $(CPP_DIR)/build/release && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native -flto -funroll-loops" ../../..

prepare-cpp-debug:
	@mkdir -p $(CPP_DIR)/build/debug
	@cp -r $(ASSETS_SRC)/* $(CPP_DIR)/build/debug/ 2>/dev/null || true
	@cd $(CPP_DIR)/build/debug && cmake -DCMAKE_BUILD_TYPE=Debug ../../..

# ==================== EMSCRIPTEN TARGETS ====================
build-emscripten: prepare-emscripten-release
	@echo "$(YELLOW)Building Emscripten (Release)...$(NC)"
	@cd $(EMSCRIPTEN_DIR)/build/release && cmake --build . --config Release
	@echo "$(GREEN)✓ Emscripten Release build complete$(NC)"

clean-emscripten:
	@echo "$(YELLOW)Cleaning Emscripten builds...$(NC)"
	@rm -rf $(EMSCRIPTEN_DIR)/build
	@echo "$(GREEN)✓ Emscripten cleanup complete$(NC)"

debug-emscripten: prepare-emscripten-debug
	@echo "$(YELLOW)Building Emscripten (Debug)...$(NC)"
	@cd $(EMSCRIPTEN_DIR)/build/debug && cmake --build . --config Debug
	@echo "$(GREEN)✓ Emscripten Debug build complete$(NC)"

run-emscripten: build-emscripten
	@echo "$(YELLOW)Running Emscripten (Release) with emrun...$(NC)"
	@emrun "$(ROOT_DIR)/$(EMSCRIPTEN_DIR)/build/release/$(PROJECT_NAME).html"

run-debug-emscripten: debug-emscripten
	@echo "$(YELLOW)Running Emscripten (Debug) with emrun...$(NC)"
	@emrun "$(ROOT_DIR)/$(EMSCRIPTEN_DIR)/build/debug/$(PROJECT_NAME).html"

prepare-emscripten-release:
	@mkdir -p $(EMSCRIPTEN_DIR)/build/release
	@cp -r $(ASSETS_SRC)/* $(EMSCRIPTEN_DIR)/build/release/ 2>/dev/null || true
	@cd $(EMSCRIPTEN_DIR)/build/release && \
		emcmake cmake \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_CXX_FLAGS="-O3 -flto" \
			-DWEB=ON \
			../../..

prepare-emscripten-debug:
	@mkdir -p $(EMSCRIPTEN_DIR)/build/debug
	@cp -r $(ASSETS_SRC)/* $(EMSCRIPTEN_DIR)/build/debug/ 2>/dev/null || true
	@cd $(EMSCRIPTEN_DIR)/build/debug && \
		emcmake cmake \
			-DCMAKE_BUILD_TYPE=Debug \
			-DWEB=ON \
			../../..

# ==================== XCODE TARGETS ====================
build-xcode: prepare-xcode
	@echo "$(YELLOW)Building Xcode (Release)...$(NC)"
	@cd $(XCODE_DIR) && cmake --build . --config Release
	@echo "$(GREEN)✓ Xcode Release build complete$(NC)"

clean-xcode:
	@echo "$(YELLOW)Cleaning Xcode build...$(NC)"
	@cd $(XCODE_DIR) && rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake Makefile
	@cd $(XCODE_DIR) && rm -rf $(PROJECT_NAME).app $(PROJECT_NAME).xcodeproj
	@echo "$(GREEN)✓ Xcode cleanup complete$(NC)"

debug-xcode: prepare-xcode-debug
	@echo "$(YELLOW)Building Xcode (Debug)...$(NC)"
	@cd $(XCODE_DIR) && cmake --build . --config Debug
	@echo "$(GREEN)✓ Xcode Debug build complete$(NC)"

run-xcode: build-xcode
	@echo "$(YELLOW)Running Xcode (Release)...$(NC)"
	@open $(XCODE_DIR)/$(PROJECT_NAME).app

run-debug-xcode: debug-xcode
	@echo "$(YELLOW)Running Xcode (Debug)...$(NC)"
	@open $(XCODE_DIR)/$(PROJECT_NAME).app

prepare-xcode:
	@mkdir -p $(XCODE_DIR)
	@cp -r $(ASSETS_SRC)/* $(XCODE_DIR)/ 2>/dev/null || true
	@cd $(XCODE_DIR) && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native -flto -funroll-loops" ..

prepare-xcode-debug:
	@mkdir -p $(XCODE_DIR)
	@cp -r $(ASSETS_SRC)/* $(XCODE_DIR)/ 2>/dev/null || true
	@cd $(XCODE_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug ..
