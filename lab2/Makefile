srcDir := src/
includeDir := include/
libDir := lib/
binDir := bin/
testsDir := tests/
buildDir := build/

LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(PWD)/$(libDir)

srcfiles := data.cpp Logger.cpp main.cpp MatrixOp.cpp readInput.cpp ScalarOp.cpp
src_files := $(addprefix $(srcDir), $(srcfiles))

files := $(addprefix $(srcDir), data readInput Logger)
static_obj := $(addsuffix _static.o, $(files) $(srcDir)ScalarOp)
dynamic_obj := $(addsuffix _dynamic.o, $(files) $(srcDir)MatrixOp)

header_files := $(wildcard $(includeDir)*.h)
executable_file := $(binDir)main
##add test case file below
TestCases := $(addprefix $(testsDir), Test1 Test2 Test3 Test4 Test5)
Args1 := -a -T -X -s -m
Args2 := -a -A -T -s -X
Args3 := -m -s -A -X
Args4 := -a -s -X -d -A
Args5 := -a -A -s -m -S -d -T -X

all: initial build

initial:
	@mkdir -p $(binDir) $(buildDir) $(libDir)

.PHONY: build
build: main_target libs
	g++ main.o -I $(includeDir) -L$(PWD)/lib -lscalarOp -lmatrixOp -o $(executable_file)
	echo "Build Successful"

main_target: src/main.cpp
	g++ -c -I $(includeDir) $< -o main.o

$(static_obj): %_static.o : %.cpp
	g++ -c -I $(includeDir) $< -o $@


$(dynamic_obj): %_dynamic.o : %.cpp
	g++ -c -fPIC -I $(includeDir) $< -o $@


.PHONY: libs
libs: $(libDir)libscalarOp.a $(libDir)libmatrixOp.so
	echo "created libraries"
$(libDir)libscalarOp.a: $(static_obj)
	ar rcs -o $@ $^
	mv $(srcDir)*.o $(buildDir)


$(libDir)libmatrixOp.so: $(dynamic_obj)
	g++ -shared -o $@ $^	
	mv $(srcDir)*.o $(buildDir)



.PHONY: run
run: | build
	@read -p "Enter the arguments: " args;\
	echo $$args > $(buildDir)arguments.txt
	@read -p "Enter the path for input file: " inputFile;\
	echo $$inputFile > $(buildDir)inputFileName.txt
	./$(executable_file) $(shell cat $(buildDir)arguments.txt) $(shell cat $(buildDir)inputFileName.txt) run.out

.PHONY: tests
tests: $(TestCases)

tests/Test%: $(testsDir)Test%.in $(testsDir)Test%.out | build
	@$(shell export LD_LIBRARY_PATH:=$(PWD)/lib)
	./$(executable_file) $(Args$(patsubst tests/Test%,%,$@)) $< $(buildDir)Test.out
	@diff $(buildDir)Test.out $(word 2,$^) > $(buildDir)test.log
	@if [ $$? ]; then\
		echo "Test $(patsubst tests/Test%,%,$@) Passed";\
	else\
		echo "Test $(patsubst tests/Test%,%,$@) Failed";\
	fi;
	@rm -rf $(buildDir)Test.out $(buildDir)test.log 

.PHONY: clean
clean:
	@rm -rf $(binDir) $(buildDir)
