# Simple Makefile for CryptoFile
# This assumes MinGW-w64 is installed

CC = gcc
CXX = g++
CFLAGS = -std=c99 -Wall -O2
CXXFLAGS = -std=c++17 -Wall -O2
LDFLAGS = -shared -Wl,--out-implib,libCryptoFile.a
LIBS = -lcrypto -lssl

# Source files
C_SOURCES = crypto_c/aes.c
CPP_SOURCES = crypto_cpp/CryptoManager.cpp
HEADERS = crypto_c/aes.h crypto_cpp/CryptoManager.h

# Output
DLL_NAME = CryptoFile.dll
BUILD_DIR = build

# Default target
all: $(BUILD_DIR)/$(DLL_NAME)

# Create build directory
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Build DLL
$(BUILD_DIR)/$(DLL_NAME): $(C_SOURCES) $(CPP_SOURCES) $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(C_SOURCES) $(CPP_SOURCES) $(LIBS)

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Build C# project
csharp: $(BUILD_DIR)/$(DLL_NAME)
	cd gui_csharp && dotnet build --configuration Release
	copy $(BUILD_DIR)\$(DLL_NAME) gui_csharp\bin\Release\net6.0-windows\

# Run
run: csharp
	gui_csharp\bin\Release\net6.0-windows\CryptoFileGUI.exe

.PHONY: all clean csharp run
