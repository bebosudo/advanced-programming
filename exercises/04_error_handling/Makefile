SRC = $(wildcard *.cc)
CXX = clang++
CXXFLAGS = -g -std=c++11 -Wall -Wextra -I ../../lectures/04_error_handling
EXE = $(SRC:.cc=.x)
INC = $(wildcard *.h)

.SUFFIXES:
.SUFFIXES: .cc .x

all: $(EXE)

%.x: %.cc
	$(CXX) $< -o ../build/$@ $(CXXFLAGS)

format: $(SRC) $(INC)
	@clang-format -i $^ 2>/dev/null || echo "Please install clang-format to run this commands"

clean:
	rm -f $(EXE) *~

.PHONY: clean



