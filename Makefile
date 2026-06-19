CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -pipe -Wall -Wextra -pedantic
TARGET := RMST
TEST_TARGET := /tmp/RMST_selftest
STUDENT_ID := 314511048
SMOKE_INPUT := /tmp/RMST_pdf_sample.dat
SMOKE_EXPECT := /tmp/RMST_pdf_sample.expect
SMOKE_OUTPUT := /tmp/RMST_pdf_sample.out

.PHONY: all test lint format-check typecheck static smoke check evaluator package clean

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

$(TEST_TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -DRMST_SELF_TEST -o $(TEST_TARGET) main.cpp

test: $(TEST_TARGET)
	$(TEST_TARGET)

lint:
	$(CXX) $(CXXFLAGS) -Werror -fsyntax-only main.cpp

format-check:
	@if grep -n '[[:blank:]]$$' main.cpp Makefile readme.txt; then exit 1; fi

typecheck:
	$(CXX) $(CXXFLAGS) -fsyntax-only main.cpp

static:
	$(CXX) $(CXXFLAGS) -Werror -fanalyzer -fsyntax-only main.cpp

smoke: $(TARGET)
	printf '5\n0 0\n2 0\n2 3\n5 1\n6 4\n' > $(SMOKE_INPUT)
	printf '13\n' > $(SMOKE_EXPECT)
	./$(TARGET) $(SMOKE_INPUT) $(SMOKE_OUTPUT)
	cmp $(SMOKE_EXPECT) $(SMOKE_OUTPUT)

check: test lint format-check typecheck static smoke

evaluator: check

package: evaluator
	mkdir -p $(STUDENT_ID)
	cp main.cpp Makefile readme.txt $(STUDENT_ID)/
	tar cvf $(STUDENT_ID).tar $(STUDENT_ID)

clean:
	$(RM) $(TARGET) $(TEST_TARGET)
