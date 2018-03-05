SRC         = ./../c++/src
DOCTEST     = $(SRC)/../doctest/doctest
DEST        = ./src

PY_VER      = python
MOD_NAME    = bestbst

SUFFIX      = $(shell $(PY_VER)-config --extension-suffix)
TARGET      = ./$(MOD_NAME)$(SUFFIX)

CXX         = g++
CXXFLAGS    = -Wall -shared -std=c++14 -fPIC -O2 -DPYTHON_BUILD
PYBIND_INC  = ./pybind11/include
INCLUDES    = $(shell $(PY_VER) -m pybind11 --includes) $(shell $(PY_VER)-config --includes) -I$(DOCTEST) -I$(PYBIND_INC)

default: clean $(SRC)
	cp -r $(SRC) $(DEST)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEST)/*.cc -o $(TARGET)

clean:
	rm -rf $(DEST) $(TARGET)

test: default
	$(PY_VER) ./test_bestbst.py
