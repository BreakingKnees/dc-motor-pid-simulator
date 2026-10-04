CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Iinclude -pthread

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Automatically find all .cpp files in src/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Create object file names in build/
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = $(BUILD_DIR)/simulator

.PHONY: all clean run debug

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

debug: CXXFLAGS += -g -O0
debug: all

clean:
	rm -rf $(BUILD_DIR)
