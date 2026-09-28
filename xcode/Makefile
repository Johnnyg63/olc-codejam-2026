.PHONY: all clean build debug run run-debug help

# Project settings
PROJECT_NAME := PGE_Johnnys_Has_To_Change
ASSETS_SRC := ../assets
ROOT_DIR := $(shell pwd)/..

# Architecture detection
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),arm64)
	ARCH := ARM64
	ARCH_FLAGS := -mcpu=apple-m1 -ffp-contract=fast
else
	ARCH := x86_64
	# Detect Intel CPU generation on macOS
	ifeq ($(shell uname -s),Darwin)
		CPU_MODEL := $(shell sysctl -n machdep.cpu.model 2>/dev/null)
		CPU_BRAND := $(shell sysctl -n machdep.cpu.brand_string 2>/dev/null)
		# Intel CPU models: Sandy Bridge=42, Ivy Bridge=58, Haswell=60, Broadwell=61, etc.
		# Pre-Haswell: <=58 (Sandy Bridge, Ivy Bridge)
		ifeq ($(shell [ "$(CPU_MODEL)" -le 58 ] 2>/dev/null && echo 1 || echo 0),1)
			ARCH_FLAGS := -msse4.2
			CPU_GEN := Pre-Haswell (SSE4.2)
		else
			ARCH_FLAGS := -march=haswell -mavx2 -mfma
			CPU_GEN := Haswell+ (AVX2)
		endif
	else
		# Linux fallback: use cpuinfo to detect
		ifeq ($(shell uname -s),Linux)
			ifeq ($(shell grep -q "avx2" /proc/cpuinfo && echo 1 || echo 0),1)
				ARCH_FLAGS := -march=haswell -mavx2 -mfma
				CPU_GEN := Haswell+ (AVX2)
			else
				ARCH_FLAGS := -msse4.2
				CPU_GEN := Pre-Haswell (SSE4.2)
			endif
		else
			# Default fallback for other systems
			ARCH_FLAGS := -march=haswell -mavx2 -mfma
			CPU_GEN := Haswell+ (AVX2 - Default)
		endif
	endif
endif

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
	@echo "$(BLUE)=== Xcode Build Targets ===$(NC)"
	@echo "$(YELLOW)Architecture: $(ARCH) ($(UNAME_M))$(NC)"
	@echo "$(YELLOW)CPU Generation: $(CPU_GEN)$(NC)"
	@echo "$(YELLOW)Optimization flags: $(ARCH_FLAGS)$(NC)"
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
	@echo "$(YELLOW)Building Xcode Release ($(ARCH) | O3 + SIMD + Unrolling)...$(NC)"
	@cd $(RELEASE_DIR) && cmake --build . --config Release
	@echo "$(GREEN)✓ Xcode Release build complete$(NC)"

clean:
	@echo "$(YELLOW)Cleaning Xcode builds...$(NC)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)✓ Xcode cleanup complete$(NC)"

debug: prepare-debug
	@echo "$(YELLOW)Building Xcode Debug...$(NC)"
	@cd $(DEBUG_DIR) && cmake --build . --config Debug
	@echo "$(GREEN)✓ Xcode Debug build complete$(NC)"

run: build
	@echo "$(YELLOW)Running Xcode Release build (optimized)...$(NC)"
	@cd $(ROOT_DIR) && ./xcode/$(RELEASE_DIR)/$(PROJECT_NAME)

run-debug: debug
	@echo "$(YELLOW)Running Xcode Debug build...$(NC)"
	@cd $(ROOT_DIR) && ./xcode/$(DEBUG_DIR)/$(PROJECT_NAME)

prepare-release:
	@mkdir -p $(RELEASE_DIR)
	@cp -r $(ASSETS_SRC)/* $(RELEASE_DIR)/ 2>/dev/null || true
	@cd $(RELEASE_DIR) && \
		cmake \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_CXX_FLAGS="-O3 $(ARCH_FLAGS) -flto=full -funroll-loops -fstrict-aliasing" \
			../../..

prepare-debug:
	@mkdir -p $(DEBUG_DIR)
	@cp -r $(ASSETS_SRC)/* $(DEBUG_DIR)/ 2>/dev/null || true
	@cd $(DEBUG_DIR) && \
		cmake \
			-DCMAKE_BUILD_TYPE=Debug \
			-DCMAKE_CXX_FLAGS="-g -O0" \
			../../..
