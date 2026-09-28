.PHONY: all clean build debug run run-debug help

# Project settings
PROJECT_NAME := PGE_Johnnys_Has_To_Change
ASSETS_SRC := ../assets
ROOT_DIR := $(shell pwd)/..

# Build directories
BUILD_DIR := build
RELEASE_DIR := $(BUILD_DIR)/release
DEBUG_DIR := $(BUILD_DIR)/debug

# Colors for output
GREEN := \033[0;32m
YELLOW := \033[0;33m
BLUE := \033[0;34m
RED := \033[0;31m
NC := \033[0m # No Color

help:
	@echo "$(BLUE)=== Emscripten Build Targets ===$(NC)"
	@echo ""
	@echo "$(GREEN)Build targets:$(NC)"
	@echo "  make build        - Build Emscripten Release (optimized, WebGL)"
	@echo "  make debug        - Build Emscripten Debug (WebGL)"
	@echo ""
	@echo "$(GREEN)Run targets:$(NC)"
	@echo "  make run          - Build and run Emscripten Release in browser with emrun"
	@echo "  make run-debug    - Build and run Emscripten Debug in browser with emrun"
	@echo ""
	@echo "$(GREEN)Clean targets:$(NC)"
	@echo "  make clean        - Remove all build artifacts"
	@echo ""
	@echo "$(YELLOW)Note: Requires Emscripten SDK installed (emcmake, emrun commands)$(NC)"
	@echo ""

all: build

build: prepare-release
	@echo "$(YELLOW)Building Emscripten Release (optimized for WebGL)...$(NC)"
	@cd $(RELEASE_DIR) && cmake --build . --config Release
	@echo "$(GREEN)✓ Emscripten Release build complete$(NC)"
	@echo "$(YELLOW)HTML output: build/release/$(PROJECT_NAME).html$(NC)"

clean:
	@echo "$(YELLOW)Cleaning Emscripten builds...$(NC)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)✓ Emscripten cleanup complete$(NC)"

debug: prepare-debug
	@echo "$(YELLOW)Building Emscripten Debug...$(NC)"
	@cd $(DEBUG_DIR) && cmake --build . --config Debug
	@echo "$(GREEN)✓ Emscripten Debug build complete$(NC)"
	@echo "$(YELLOW)HTML output: build/debug/$(PROJECT_NAME).html$(NC)"

run: build
	@echo "$(YELLOW)Running Emscripten Release in browser with emrun...$(NC)"
	@emrun "$(shell pwd)/$(RELEASE_DIR)/$(PROJECT_NAME).html"

run-debug: debug
	@echo "$(YELLOW)Running Emscripten Debug in browser with emrun...$(NC)"
	@emrun "$(shell pwd)/$(DEBUG_DIR)/$(PROJECT_NAME).html"

prepare-release:
	@mkdir -p $(RELEASE_DIR)
	@cp -r $(ASSETS_SRC)/* $(RELEASE_DIR)/ 2>/dev/null || true
	@cd $(RELEASE_DIR) && \
		emcmake cmake \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_CXX_FLAGS="-O3 -flto" \
			-DWEB=ON \
			../../..

prepare-debug:
	@mkdir -p $(DEBUG_DIR)
	@cp -r $(ASSETS_SRC)/* $(DEBUG_DIR)/ 2>/dev/null || true
	@cd $(DEBUG_DIR) && \
		emcmake cmake \
			-DCMAKE_BUILD_TYPE=Debug \
			-DWEB=ON \
			../../..
