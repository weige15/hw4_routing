CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -pipe -Wall -Wextra -pedantic
TARGET := RMST
TEST_TARGET := /tmp/RMST_selftest

.PHONY: all test clean

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

$(TEST_TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -DRMST_SELF_TEST -o $(TEST_TARGET) main.cpp

test: $(TEST_TARGET)
	$(TEST_TARGET)

clean:
	$(RM) $(TARGET) $(TEST_TARGET)
