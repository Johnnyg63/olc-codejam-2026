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
	@echo "$(BLUE)=== C++ Build Targets ===$(NC)"
	@echo ""
	@echo "$(GREEN)Build targets:$(NC)"
	@echo "  make build        - Build Release version (optimized with O3, SIMD, unrolling)"
	@echo "  make debug        - Build Debug version"
	@echo ""
	@echo "$(GREEN)Run targets:$(NC)"
	@echo "  make run          - Build and run Release version (optimized)"
	@echo "  make run-debug    - Build and run Debug version"
	@echo ""
	@echo "$(GREEN)Clean targets:$(NC)"
	@echo "  make clean        - Remove all build artifacts"
	@echo ""

all: build

build: prepare-release
	@echo "$(YELLOW)Building C++ Release (O3 + SIMD + Unrolling)...$(NC)"
	@cd $(RELEASE_DIR) && cmake --build . --config Release
	@echo "$(GREEN)✓ C++ Release build complete$(NC)"

clean:
	@echo "$(YELLOW)Cleaning C++ builds...$(NC)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)✓ C++ cleanup complete$(NC)"

debug: prepare-debug
	@echo "$(YELLOW)Building C++ Debug...$(NC)"
	@cd $(DEBUG_DIR) && cmake --build . --config Debug
	@echo "$(GREEN)✓ C++ Debug build complete$(NC)"

run: build
	@echo "$(YELLOW)Running C++ Release build (optimized)...$(NC)"
	@cd $(ROOT_DIR) && ./cpp/$(RELEASE_DIR)/$(PROJECT_NAME)

run-debug: debug
	@echo "$(YELLOW)Running C++ Debug build...$(NC)"
	@cd $(ROOT_DIR) && ./cpp/$(DEBUG_DIR)/$(PROJECT_NAME)

prepare-release:
	@mkdir -p $(RELEASE_DIR)
	@cp -r $(ASSETS_SRC)/* $(RELEASE_DIR)/ 2>/dev/null || true
	@cd $(RELEASE_DIR) && \
		cmake \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_CXX_FLAGS="-O3 -march=native -flto -funroll-loops -fvectorize" \
			../../..

prepare-debug:
	@mkdir -p $(DEBUG_DIR)
	@cp -r $(ASSETS_SRC)/* $(DEBUG_DIR)/ 2>/dev/null || true
	@cd $(DEBUG_DIR) && \
		cmake \
			-DCMAKE_BUILD_TYPE=Debug \
			-DCMAKE_CXX_FLAGS="-g -O0" \
			../../..
