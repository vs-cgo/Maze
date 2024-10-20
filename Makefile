CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g
PROJECT := Maze
PROJECT_DIR := ${PROJECT}/
TEST_DIR := tests

MODEL := $(shell find ${PROJECT_DIR}model -type f -name "*.cc") 
CONTROLLER := $(shell find ${PROJECT_DIR}controller -type f -name "*.cc")
SRC := ${MODEL} ${CONTROLLER} ${PROJECT_DIR}view/cave/gen/gencave.cc
OBJ := $(patsubst %.cc, %.o, ${SRC})
SRC_TEST := $(shell find ${TEST_DIR} -type f -name "*.cc")
OBJ_TEST := $(patsubst %.cc, %.o, ${SRC_TEST})
TAR = $(shell find ${PROJECT} -type f -name "*.cc" -o -name "*.h" -o -name "*.ui" -o -name CMakeLists.txt )

LDFLAGS= -lgtest -lgmock
NAME := $(shell uname -s)
ifeq (${NAME}, Linux) 
		LDFLAGS= ${LDFLAGS} -lpthread
endif

.PHONY: all clean test build

all: clean install

build:
		@-mkdir build 
		cd build && cmake -DCMAKE_BUILD_TYPE=Release ../Maze/CMakeLists.txt && make
		chmod 755 build/${PROJECT}.app
install: clean build 
		@-mkdir ../../${PROJECT}
		cp -r build/Maze.app ../../${PROJECT}/Maze.app	
uninstall:
		rm -rf ../../Maze
clean:
		@rm -rf $(shell find ${PROJECT} -type f -name "*.o") tests/*.o
		@rm -rf test build latex/ html/ report
		@rm -rf $(shell find ${PROJECT} -type f -name "*.gcda" -o -name "*.gcno")
		@rm -rf tests/*.gcda tests/*.gcno test.info
dvi: 
		-doxygen Doxyfile - w html
		open html/index.html
dviclean:
		rm -rf html/ latex/
		
dist:	
		@tar cvf ../../${PROJECT}.tar ${TAR} ./Makefile ./description.h ./Doxyfile
dist2: clean build
		@tar cvf ../../${PROJECT}.tar build/Maze.app
distclean:
		rm -rf ../../${PROJECT}.tar
			
test: ${OBJ} ${OBJ_TEST}
		${CXX} $^ ${LDFLAGS} -o $@
		./test
%.o: %.cc %.h
		${CXX} ${CXXFLAGS} -c ${filter-out %.h, $^} -o $@
				
leaks: ${OBJ} ${OBJ_TEST}
		${CXX} $^ ${LDFLAGS} -o test
		leaks --atExit -- ./test
san: clean add_sanitaizer test
cpp:
		cppcheck ${PROJECT}
start: 
		open ${PROJECT_DIR}CMakeLists.txt -a "Qt Creator.app"
check: 
		@cp ../materials/linters/.clang-format ./
		clang-format -n $(shell find ${PROJECT} tests -type f -name "*.cc" -o -name "*.h")
		@rm -rf .clang-format
check2:
		@cp ../materials/linters/.clang-format ./
		clang-format -i $(shell find ${PROJECT} tests -type f -name "*.cc" -o -name "*.h")
		@rm -rf .clang-format
gcov_report: clean add_coverage ${OBJ} ${OBJ_TEST}
		@$(CXX) $(CXXFLAGS) $(OBJ) $(OBJ_TEST) $(LDFLAGS) -o test
		@./test
		@-mkdir report
		lcov --branch-coverage --ignore-errors mismatch,mismatch,empty,inconsistent,usage,unused --no-external --no-recursion -t "test" -q -o test.info -c -directory Maze/model -directory Maze/model/command -directory Maze/model/learning -directory Maze/controller -directory Maze/view/cave/gen -directory tests
#lcov --r test.info "tests/maze_test.cc" -o test.info
		genhtml --ignore-errors mismatch -o report test.info
		open report/index.html
		rm -rf *.gcda *.gcno *.out
add_coverage:
	$(eval CXXFLAGS += -fprofile-arcs -ftest-coverage -O0)
add_sanitaizer:
	$(eval CXXFLAGS += -fsanitize=address)
	$(eval LDFLAGS +=	-fsanitize=address -lasan)

