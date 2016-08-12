GTEST_HOME?=$(HOME)/tools/googletest-test-1.7.0
GTEST_LIBS?=$(GTEST_HOME)
GTEST_CFLAGS?=-I$(GTEST_HOME)/include
GTEST_LFLAGS?=-L$(GTEST_LIBS) -lgtest_main -lgtest -pthread

CPP14_FLAG?=-std=c++14 # clang || modern gcc
#CPP14_FLAG?=-std=c++1y
CXXFLAGS+=-pthread $(CPP14_FLAG) $(GTEST_CFLAGS)
LDFLAGS+=$(GTEST_LFLAGS)

PYTHON?=python3

test: tests
	./tests
tests: a_star_tests.o board_tests.o point_tests.o
	$(CXX) $(LDFLAGS) $^ -o tests

a_star_tests.o: a_star_tests.cc a_star.hpp
board_tests.o: board_tests.cc board.hpp
point_tests.o: point_tests.cc point.hpp

.PHONY: clean python
python_ext: a_star.hpp board.hpp point.hpp py_module.cc
	rm -fR _rover.so _rover.*.so build
	CFLAGS="$(CXXFLAGS)" $(PYTHON) setup.py build_ext --inplace
python: python_ext test.py
	$(PYTHON) test.py
clean:
	rm -fR tests *.o build *.so
