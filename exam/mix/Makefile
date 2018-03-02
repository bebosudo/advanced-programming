SRC         = ./../c++/src
DOCTEST     = $(SRC)/../doctest/doctest
DEST        = ./src

PY_VER      = python3
MOD_NAME    = bestbst

SUFFIX      = $(shell $(PY_VER)-config --extension-suffix)
TARGET      = ./$(MOD_NAME)$(SUFFIX)

CC          = g++
CFLAGS      = -Wall -shared -std=c++14 -fPIC -O2 -DPYTHON_BUILD
INCLUDES    = $(shell $(PY_VER) -m pybind11 --includes) $(shell $(PY_VER)-config --includes) -I$(DOCTEST)

default: clean $(SRC)
	cp -r $(SRC) $(DEST)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEST)/*.cc -o $(TARGET)

clean:
	rm -rf $(DEST) $(TARGET)

test: default
	$(PY_VER) ./test_bestbst.py
