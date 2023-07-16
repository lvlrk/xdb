CXX = g++
CXXFLAGS = -O2 -c
LDFLAGS = -lraylib -lfmt
TARGET = xdb

all: $(TARGET)

build: src/*.cpp
	$(CXX) $(CXXFLAGS) $^

$(TARGET): build
	$(CXX) $(LDFLAGS) *.o -o $(TARGET)
