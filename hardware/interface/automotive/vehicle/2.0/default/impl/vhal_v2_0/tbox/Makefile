CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinc 
LDFLAGS =
TARGET = communication_program
SRC_DIR = src
OBJ_DIR = obj

# 源文件列表
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# 对应的目标文件列表
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

