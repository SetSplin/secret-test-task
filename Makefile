build: test.cpp hashtable.h
	g++ --std=c++17 -I. test.cpp -o dist/run_tests

test: test.cpp hashtable.h
	make build && ./dist/run_tests

lint: hashtable.h
	./run-clang-format.py hashtable.h

all: test.cpp hashtable.h
	make lint && make build && ./dist/run_tests

coverage: test.cpp hashtable.h
	clang++ -fprofile-instr-generate -fcoverage-mapping -std=c++17 test.cpp -I. -o dist/tests_coverage && cd dist && ./tests_coverage && llvm-profdata merge -sparse default.profraw -o test.profdata && llvm-cov show --format html ./tests_coverage -instr-profile=test.profdata > result.html && llvm-cov report ./tests_coverage -instr-profile=test.profdata