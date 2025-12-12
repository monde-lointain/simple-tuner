.DEFAULT_GOAL := help
.PHONY: help configure configure-release configure-debug configure-coverage \
        configure-sanitize configure-no-tests build rebuild clean test coverage \
        coverage-html format format-check tidy cppcheck complexity analyze docs \
        sloccount all \
        configure-android build-android clean-android \
        configure-ios build-ios-device build-ios-simulator build-ios clean-ios \
        build-mobile clean-mobile

# Build directory
BUILD_DIR := build

# Default build type
BUILD_TYPE ?= Debug

# Sanitizer options (empty by default)
SANITIZER ?=

# Build tests (default: ON)
BUILD_TESTS ?= ON

# CMake generator (default: Unix Makefiles)
CMAKE_GENERATOR ?= "Unix Makefiles"

# Number of parallel jobs
JOBS ?= $(shell nproc 2>/dev/null || echo 4)

# Android directory
ANDROID_DIR := JUCEBuilds/Android

# Android build type (debug or release)
ANDROID_BUILD_TYPE ?= debug

# iOS directory
IOS_DIR := JUCEBuilds/iOS

# iOS build configuration
IOS_BUILD_CONFIG ?= Debug

# iOS SDK selection
IOS_SDK_DEVICE := iphoneos
IOS_SDK_SIM := iphonesimulator

#######################
# Configuration Targets
#######################

configure: ## Configure Debug build with all analysis enabled
	@echo "Configuring Debug build with analysis..."
	cmake -S . -B $(BUILD_DIR) \
		-G $(CMAKE_GENERATOR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DBUILD_TESTING=$(BUILD_TESTS)

configure-release: ## Configure Release build
	@echo "Configuring Release build..."
	cmake -S . -B $(BUILD_DIR) \
		-G $(CMAKE_GENERATOR) \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_TESTING=$(BUILD_TESTS)

configure-debug: ## Configure Debug build (no analysis)
	@echo "Configuring Debug build..."
	cmake -S . -B $(BUILD_DIR) \
		-G $(CMAKE_GENERATOR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTING=$(BUILD_TESTS)

configure-coverage: ## Configure Debug build with coverage analysis
	@echo "Configuring Debug build with coverage..."
	cmake -S . -B $(BUILD_DIR) \
		-G $(CMAKE_GENERATOR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DENABLE_COVERAGE_ANALYSIS=ON \
		-DBUILD_TESTING=ON

configure-sanitize: ## Configure Debug build with sanitizers (set SANITIZER=address,undefined)
	@echo "Configuring Debug build with sanitizers ($(SANITIZER))..."
	cmake -S . -B $(BUILD_DIR) \
		-G $(CMAKE_GENERATOR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DUSE_SANITIZER=$(SANITIZER) \
		-DBUILD_TESTING=$(BUILD_TESTS)

configure-no-tests: ## Configure build without tests (set BUILD_TYPE for Debug/Release)
	@echo "Configuring $(BUILD_TYPE) build without tests..."
	$(MAKE) BUILD_TESTS=OFF configure-$(shell echo $(BUILD_TYPE) | tr A-Z a-z)

#################
# Build Targets
#################

build: ## Build the project (requires prior configure)
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		echo "Error: Build not configured. Run 'make configure' first."; \
		exit 1; \
	fi
	cmake --build $(BUILD_DIR) -j $(JOBS)

rebuild: clean configure build ## Clean, configure, and build from scratch

clean: ## Remove build directory
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)

##################
# Testing Targets
##################

test: ## Run tests
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		echo "Error: Build not configured. Run 'make configure && make build' first."; \
		exit 1; \
	fi
	ctest --test-dir $(BUILD_DIR)/tests --output-on-failure

coverage: ## Generate coverage reports (requires configure-coverage)
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		echo "Error: Build not configured. Run 'make configure-coverage && make build' first."; \
		exit 1; \
	fi
	cmake --build $(BUILD_DIR)/tests --target coverage

coverage-html: coverage ## Generate coverage and open HTML report
	@if [ -f "$(BUILD_DIR)/coverage/index.html" ]; then \
		xdg-open $(BUILD_DIR)/coverage/index.html 2>/dev/null || \
		open $(BUILD_DIR)/coverage/index.html 2>/dev/null || \
		echo "Coverage report: $(BUILD_DIR)/coverage/index.html"; \
	else \
		echo "Error: Coverage HTML not found."; \
	fi

####################
# Analysis Targets
####################

format: ## Format code in-place
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR) -G $(CMAKE_GENERATOR); \
	fi
	cmake --build $(BUILD_DIR) --target format

format-check: ## Check code formatting (dry-run)
	@echo "Checking code formatting..."
	find include src tests -name '*.cpp' -o -name '*.h' | \
		xargs clang-format --dry-run --Werror

tidy: ## Run clang-tidy
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		echo "Error: Build not configured. Run 'make configure' first."; \
		exit 1; \
	fi
	cmake --build $(BUILD_DIR) --target tidy

cppcheck: ## Run cppcheck
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR) -G $(CMAKE_GENERATOR); \
	fi
	cmake --build $(BUILD_DIR) --target cppcheck

complexity: ## Run complexity analysis
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR) -G $(CMAKE_GENERATOR); \
	fi
	cmake --build $(BUILD_DIR) --target complexity

analyze: tidy cppcheck complexity ## Run all static analysis

##################
# Utility Targets
##################

docs: ## Generate documentation
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR) -G $(CMAKE_GENERATOR); \
	fi
	cmake --build $(BUILD_DIR) --target docs

sloccount: ## Generate code metrics
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR) -G $(CMAKE_GENERATOR); \
	fi
	cmake --build $(BUILD_DIR) --target sloccount

####################
# Mobile Targets
####################

configure-android: ## Configure Android build
	@echo "Saving Projucer project..."
	Projucer --resave SimpleTuner.jucer
	@echo "Configuring Android build ($(ANDROID_BUILD_TYPE))..."
	@if [ ! -f "$(ANDROID_DIR)/gradlew" ]; then \
		echo "Error: Android build not found at $(ANDROID_DIR)"; \
		exit 1; \
	fi

build-android: configure-android ## Build Android APK
	@echo "Building Android APK ($(ANDROID_BUILD_TYPE))..."
	cd $(ANDROID_DIR) && ./gradlew assemble$(shell echo $(ANDROID_BUILD_TYPE) | sed 's/\(.\)/\u\1/')

clean-android: ## Clean Android build artifacts
	@echo "Cleaning Android build..."
	cd $(ANDROID_DIR) && ./gradlew clean

configure-ios: ## Check iOS build prerequisites
	@echo "Saving Projucer project..."
	Projucer --resave SimpleTuner.jucer
	@echo "Checking iOS build prerequisites..."
	@if [ ! -d "$(IOS_DIR)/SimpleTuner.xcodeproj" ]; then \
		echo "Error: iOS Xcode project not found at $(IOS_DIR)"; \
		exit 1; \
	fi
	@if ! command -v xcodebuild >/dev/null 2>&1; then \
		echo "Error: xcodebuild not found. iOS builds require macOS with Xcode."; \
		exit 1; \
	fi

build-ios-device: configure-ios ## Build iOS app for physical devices (arm64)
	@echo "Building iOS app for device ($(IOS_BUILD_CONFIG))..."
	xcodebuild -project $(IOS_DIR)/SimpleTuner.xcodeproj \
		-scheme SimpleTuner \
		-configuration $(IOS_BUILD_CONFIG) \
		-sdk $(IOS_SDK_DEVICE) \
		-arch arm64 \
		build

build-ios-simulator: configure-ios ## Build iOS app for simulator (x86_64/arm64)
	@echo "Building iOS app for simulator ($(IOS_BUILD_CONFIG))..."
	xcodebuild -project $(IOS_DIR)/SimpleTuner.xcodeproj \
		-scheme SimpleTuner \
		-configuration $(IOS_BUILD_CONFIG) \
		-sdk $(IOS_SDK_SIM) \
		build

build-ios: build-ios-device build-ios-simulator ## Build iOS app for both device and simulator

clean-ios: ## Clean iOS build artifacts
	@echo "Cleaning iOS build..."
	@if command -v xcodebuild >/dev/null 2>&1; then \
		xcodebuild -project $(IOS_DIR)/SimpleTuner.xcodeproj \
			-scheme SimpleTuner \
			clean; \
	else \
		rm -rf $(IOS_DIR)/build; \
	fi

build-mobile: build-android ## Build mobile apps (Android + iOS if on macOS)
	@if command -v xcodebuild >/dev/null 2>&1; then \
		$(MAKE) build-ios; \
	else \
		echo "Skipping iOS build (xcodebuild not available)"; \
	fi

clean-mobile: clean-android clean-ios ## Clean all mobile build artifacts

all: configure build test build-mobile ## Configure, build, test, and build mobile apps

help: ## Display this help message
	@echo "Usage: make [target]"
	@echo ""
	@echo "Available targets:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
		awk 'BEGIN {FS = ":.*?## "}; {printf "  %-20s %s\n", $$1, $$2}'
	@echo ""
	@echo "Variables:"
	@echo "  BUILD_DIR    Build directory (default: build)"
	@echo "  BUILD_TYPE   Build type (default: Debug)"
	@echo "  BUILD_TESTS  Build unit tests (default: ON)"
	@echo "  SANITIZER    Sanitizer(s) to use (e.g., address,undefined)"
	@echo "  JOBS         Parallel jobs (default: nproc)"
	@echo ""
	@echo "Examples:"
	@echo "  make all                         # Quick start: configure + build + test + mobile"
	@echo "  make BUILD_TESTS=OFF configure build  # Build without tests"
	@echo "  make configure-no-tests build    # Build without tests"
	@echo "  make configure-coverage build test coverage  # Coverage workflow"
	@echo "  make SANITIZER=address configure-sanitize build test  # Sanitizer build"
	@echo "  make analyze                     # Run all static analysis"
	@echo "  make build-android               # Build Android APK (arm64-v8a, debug)"
	@echo "  make ANDROID_BUILD_TYPE=release build-android  # Build release APK"
	@echo "  make build-ios-device            # Build iOS for physical devices"
	@echo "  make build-ios-simulator         # Build iOS for simulator"
	@echo "  make build-mobile                # Build all mobile platforms"
