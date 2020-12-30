CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

MODULE = _pedometer
SOFILE = $(MODULE)$(shell python3-config --extension-suffix)

.PHONY: all test clean
all: $(SOFILE)

$(SOFILE): pedometer.cc pedometer.h
	$(CXX) $(CXXFLAGS) `python3 -m pybind11 --includes` pedometer.cc -o $@

clean:
	@rm -rf $(SOFILE) __pycache__ .pytest_cache performance.txt *.png
