test_files := tests/test_init.cpp\
	tests/test_constructors.cpp\
	tests/test_insert.cpp\
	tests/test_erase.cpp\
	tests/test_range_for.cpp\
	tests/test_find.cpp\
	tests/test_square_brackets.cpp\
	tests/test_constness.cpp\
	tests/test_relocating.cpp\
	tests/test_iterators.cpp\
	tests/test_custom.cpp\
	tests/test_comparator.cpp\
	tests/test_hasher.cpp

compiler := g++ --std=c++17 -O2

build: $(test_files) hashtable.h
	$(compiler) -I. -Itests $(test_files) -o dist/run_tests

test: $(test_files) hashtable.h
	make build && ./dist/run_tests

lint: hashtable.h
	./run-clang-format.py hashtable.h

all: $(test_files) hashtable.h
	make lint && make build && ./dist/run_tests

coverage: $(test_files) hashtable.h
	clang++ -fprofile-instr-generate -fcoverage-mapping -std=c++17 $(test_files) -I. -Itests -o dist/tests_coverage &&\
	cd dist &&\
	./tests_coverage &&\
	llvm-profdata merge -sparse default.profraw -o test.profdata &&\
	llvm-cov show --format html ./tests_coverage -instr-profile=test.profdata > result.html &&\
	llvm-cov report ./tests_coverage -instr-profile=test.profdata

perf: hashtable.h perf_test.cpp
	$(compiler) perf_test.cpp -fno-omit-frame-pointer -ggdb -gdwarf -I. -o dist/perf_test &&\
	./dist/perf_test &&\
	cd dist &&\
	sudo perf record -F 24750 -a -g -- ./perf_test &&\
	sudo chmod 666 ./perf.data &&\
	perf script | ../FlameGraph/stackcollapse-perf.pl > out.perf-folded &&\
	../FlameGraph/flamegraph.pl out.perf-folded > perf-kernel.svg