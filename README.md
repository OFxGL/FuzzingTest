## Fuzzing Keynotes  

### Getting to know:  
**Fuzzing** or **fuzz testing** is an automated software testing technique that involves providing invalid, unexpected, or random data as inputs to a computer program. The program is then monitored for exceptions such as crashes, failing built-in code assertions, or potential memory leaks. Typically, fuzzers are used to test programs that take structured inputs. This structure is specified, e.g., in a file format or protocol and distinguishes valid from invalid input. An effective fuzzer generates semi-valid inputs that are "valid enough" in that they are not directly rejected by the parser, but do create unexpected behaviors deeper in the program and are "invalid enough" to expose corner cases that have not been properly dealt with – [Wikipedia](https://en.wikipedia.org/wiki/Fuzzing)  

[Fuzz Testing(Fuzzing) Tutorial: What is, Types, Tools & Example](https://www.guru99.com/fuzz-testing.html)  
[An informative guide on using AFL and libFuzzer](https://blog.f-secure.com/super-awesome-fuzzing-part-one/)  
[Awesome-Fuzzing Collection](https://github.com/secfigo/Awesome-Fuzzing)  
[Google fuzzing tutorials and examples](https://github.com/google/fuzzing)  
[Google SE experience](https://www.usenix.org/sites/default/files/conference/protected-files/enigma_slides_serebryany.pdf)  
[CI Fuzz](https://help.code-intelligence.com/continuous-fuzzing-setup)  

### Tools:  
[**libFuzzer**](https://llvm.org/docs/LibFuzzer.html), [google libFuzzer tutorial](https://github.com/google/fuzzing/blob/master/tutorial/libFuzzerTutorial.md)  
[AFL](https://lcamtuf.coredump.cx/afl/) ([repo](https://github.com/google/AFL)), [**AFL++**](https://aflplus.plus/) ([repo](https://github.com/AFLplusplus/AFLplusplus), [example](https://github.com/antonio-morales/Fuzzing101/tree/main/Exercise%201)), [more](https://github.com/google/fuzzing/blob/master/docs/afl-based-fuzzers-overview.md)  
[Hoggfuzz](https://honggfuzz.dev/) ([repo](https://github.com/google/honggfuzz))  
[Microsoft OneFuzz](https://github.com/microsoft/onefuzz)  
[Radamsa](https://gitlab.com/akihe/radamsa)  
[OSS-Fuzz](https://google.github.io/oss-fuzz/) ([repo](https://github.com/google/oss-fuzz)) - for **open-source software only**  
[ClusterFuzz](https://google.github.io/clusterfuzz/) ([repo](https://github.com/google/clusterfuzz)) - runs **on GCP only**  
[ClusterFuzzLite](https://google.github.io/clusterfuzzlite/) ([repo](https://github.com/google/clusterfuzzlite/)) - supports libFuzzer only  
[GitLab fuzzing](https://docs.gitlab.com/ee/user/application_security/coverage_fuzzing/) ([C++ example](https://gitlab.com/gitlab-org/security-products/demos/coverage-fuzzing/c-cpp-fuzzing-example))  

### TestCase  
Fuzz target:  
```cpp
bool fuzz(const uint8_t* data, size_t size)
{
    return (size >= 3 && data[0] == 'F' && data[1] == 'U' && data[2] == 'Z' && data[3] == 'Z');
}
```
[Criteria for good fuzzing test cases](https://github.com/google/fuzzing/blob/master/docs/good-fuzz-target.md)  

#### LibFuzzer  
[LibFuzzer](https://llvm.org/docs/LibFuzzer.html) is in-process, coverage-guided, evolutionary fuzzing engine.LibFuzzer is linked with the library under test, and feeds fuzzed inputs to the library via a specific fuzzing entrypoint (aka “target function”); the fuzzer then tracks which areas of the code are reached, and generates mutations on the corpus of input data in order to maximize the code coverage.  
```
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    fuzz(data, size);
    return 0; // Non-zero return values are reserved for future use.
}

clang++ -std=c++17 -g -O1 -fsanitize=fuzzer fuzzer.cpp -o fuzzer
./fuzzer -jobs=4 -max_total_time=600 -max_len=5 -dict=dicts/fuzz.dict -artifact_prefix=errors/ corpus/ seeds/
```
[Dictionaries](https://llvm.org/docs/LibFuzzer.html#dictionaries):  
-dict key. LibFuzzer supports user-supplied dictionaries compatible with AFL  

[Corpus](https://llvm.org/docs/LibFuzzer.html#corpus):  
When a libFuzzer-based fuzzer is executed with one more directory as arguments, it will first read files from every directory recursively and execute the target function on all of them. Then, any input that triggers interesting code path(s) will be written back into the first corpus directory. (Useful `-merge=1`)  

[Structure-Aware Fuzzing](https://github.com/google/fuzzing/blob/master/docs/structure-aware-fuzzing.md) (custom mutators for: compression, protobufs([libprotobuf-mutator](https://github.com/google/libprotobuf-mutator)), g RPC API, etc)  
This can be done by defining LLVMFuzzerCustomMutator function where the data contains a random input to be mutated:  
```
extern "C" size_t LLVMFuzzerCustomMutator(uint8_t* data, size_t size, size_t max_size, unsigned int seed);
```
[Using libFuzzer as a library](https://llvm.org/docs/LibFuzzer.html#using-libfuzzer-as-a-library)  
The lib can be found at `/usr/lib/<llvm-version>/lib/clang/<clang-version>/lib/linux/libclang_rt.fuzzer_no_main-<architecture>.a`  
In our main function to start fuzzing, it can call LLVMFuzzerRunDriver, passing in the program arguments and a callback. This callback is invoked just like LLVMFuzzerTestOneInput, and has the same signature.  
It’s possible to build our fuzzer with gcc:  
```
g++ -g –O1 -std=c++17 fuzzer.cpp main.cpp /usr/lib/llvm-13/lib/clang/13.0.1/lib/linux/libclang_rt.fuzzer_no_main-x86_64.a -lpthread -o fuzzer
```

#### AFL++  
[AFL++](https://aflplus.plus/) is the daughter of the American Fuzzy Lop fuzzer by Michał “lcamtuf” Zalewski and was created initially to incorporate all the best features developed in the years for the fuzzers in the AFL family and not merged in AFL cause it is not updated since November 2017.  

Install:  
```
git clone https://github.com/AFLplusplus/AFLplusplus && cd AFLplusplus
make all
sudo make install
export PATH=$PATH:</path/to/AFLplusplus>
```
Build:  
```
afl-g++-fast -std=c++17 main.cpp -o afl-app
```
Run:  
```
afl-fuzz -x dicts/fuzz.dict -i seeds/ -o corpus/ -D -V 600 -- afl-app
```
It also supports [Custom Mutators](https://aflplus.plus/docs/custom_mutators/)  
```
Export AFL_CUSTOM_MUTATOR_LIBRARY="path"
size_t afl_custom_fuzz(void *data, unsigned char *buf, size_t buf_size, unsigned char **out_buf, unsigned char *add_buf, size_t add_buf_size, size_t max_size);
```

### CI/CD  
CI/CD formula: **GitHab Actions / GitLab runner + ClusterFuzzLite + libFuzzer**  

### Comparison  
LibFuzzer:  
* needs test cases preparation  
* integrated into LLVM/clang  
* can be used as a static lib  
* highly compatible with CI/CD tools  
* seamlessly paired with other sanitizers  
* highly flexible with test scenarios  
* supports custom mutators  
* AFL compatible test corpus  

AFL++:
* automatically creates test cases (this brings more cons - makes it unmodifiable, less relevant and relatively slow)  
* almost none of CI/CD tools declare AFL/AFL++ support  
* supports gcc and clang  
* rich test results and stats reports  
* supports custom mutators  

### Conclusion  
LibFuzzer is an excellent tool for fuzzing - functional, highly customizable, lighting fast and easily integrated with other tools and CI/CD services.  
