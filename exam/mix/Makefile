VENV=./venv
PIP=$(VENV)/bin/pip

default: all

clean:
	@rm -rf $(VENV)
	rm -rf ./build
	rm -rf *.so
	rm -rf *.cpp

all: clean build

venv:
	virtualenv -p python3 $(VENV)
	$(PIP) install cython
	$(PIP) install binarytree

build: setup.py
	python3 setup.py build_ext --inplace

.PHONY: build
