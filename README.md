# <R/Trading> test task
## Abstract
`Hashtable.h` header only c++ lib provides a hash table class.

According to task `Hashtable` is open addressing table (only uses `new` for relocating buffer).

`Hashtable` is a template class, has four template parameters:
- `KeyType`. Type of table's key
- `ValueType`. Type of table's value
- `HasherType`. Type that implements the `std::hash<KeyType>`(`HasherType` by default) interface
- `ComparatorType`. Type that implements the `std::equal_to<KeyType>`(`ComparatorType` by default) interface

Class almost implements std::unordered_map interface

## Tests
### Framework
[Catch2](https://github.com/catchorg/Catch2/) as test framework.
### Coverage
Clang's [Source-based code coverage](https://clang.llvm.org/docs/SourceBasedCodeCoverage.html) for coverage analysis.

## Linter
[ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) and Yandex guidelines as linter. See `.clang-format` for details.

## Up and running
There are some `make` targets:
- `build`. Compile test runner `dist/run_tests`
- `test`. Run `dist/run_tests`
- `lint`. Lint `hashtable.h`
- `all`. Compile and run `dist/run_tests`
- `coverage`. Analyze test coverage. Generates `dist/result.html` and outputs coverage percentage