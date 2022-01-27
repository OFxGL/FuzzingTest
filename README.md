## Fuzzing Keynotes  

### Getting to know:  
[Definition](https://en.wikipedia.org/wiki/Fuzzing)  
[Simple explanation](https://www.guru99.com/fuzz-testing.html)  
[An informative guide on using AFL and libFuzzer](https://blog.f-secure.com/super-awesome-fuzzing-part-one/)  
[Awesome-Fuzzing Collection](https://github.com/secfigo/Awesome-Fuzzing)  
[Google fuzzing](https://github.com/google/fuzzing)  
[Google experience](https://www.usenix.org/sites/default/files/conference/protected-files/enigma_slides_serebryany.pdf)  

### Tools:  
[**libFuzzer**](https://llvm.org/docs/LibFuzzer.html), [google libFuzzer tutorial](https://github.com/google/fuzzing/blob/master/tutorial/libFuzzerTutorial.md) - LLVM integrated  
[AFL](https://lcamtuf.coredump.cx/afl/) ([repo](https://github.com/google/AFL)), [**AFL++**](https://aflplus.plus/) ([repo](https://github.com/AFLplusplus/AFLplusplus)), [more](https://github.com/google/fuzzing/blob/master/docs/afl-based-fuzzers-overview.md)  
[Hoggfuzz](https://honggfuzz.dev/) ([repo](https://github.com/google/honggfuzz))  
[OSS-Fuzz](https://google.github.io/oss-fuzz/) ([repo](https://github.com/google/oss-fuzz)) - for **open-source software only**  
[ClusterFuzz](https://google.github.io/clusterfuzz/) ([repo](https://github.com/google/clusterfuzz)) - runs **on GCP only**  
[ClusterFuzzLite](https://google.github.io/clusterfuzzlite/) ([repo](https://github.com/google/clusterfuzzlite/)) - supports libFuzzer only  
[Microsoft OneFuzz](https://github.com/microsoft/onefuzz)  

### TestCase  
```cpp
bool FUZZ(const uint8_t* data, size_t size)
{
    return (size >= 3 && data[0] == 'F' && data[1] == 'U' && data[2] == 'Z' && data[3] == 'Z');
}
```
#### libFuzzer:  
```
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    FUZZ(data, size);
    return 0; // Non-zero return values are reserved for future use.
}

clang++ -std=c++17 -stdlib=libstdc++ -g -fsanitize=address,fuzzer fuzzer.cpp -o fuzzer
./fuzzier -max_total_time=600 -max_len=5 -artifact_prefix=errors/ corpus/
```
[Dictionaries](https://llvm.org/docs/LibFuzzer.html#dictionaries):  
```
./fuzzer -dict=dicts/fuzz.dict
```
[Corpus](https://llvm.org/docs/LibFuzzer.html#corpus) (problematic test cases we have had before / regression check):  
```
./fuzzer corpus/ seeds/
```
When a libFuzzer-based fuzzer is executed with one more directory as arguments, it will first read files from every directory recursively and execute the target function on all of them. Then, any input that triggers interesting code path(s) will be written back into the first corpus directory (in this case, `corpus`).
Useful `-merge=1` opt.  

#### AFL++:  
preparation:  
```
git clone https://github.com/AFLplusplus/AFLplusplus && cd AFLplusplus
make all
sudo make install
export PATH=$PATH:</path/to/AFLplusplus>
afl-fuzz
```
build:  
```
afl-clang-fast++ -std=c++17 main.cpp -o afl-app
or
afl-g++-fast -std=c++17 main.cpp -o afl++-app
```
run:  
```
afl-fuzz -i seeds/ -o corpus/ -V 300 -- afl++-app
```
With Dictionaries:  
```
afl-fuzz -x dicts/fuzz.dict -i seeds/ -o corpus/ -V 300 -- afl++-app
```

### CI/CD:  
**GitHab Actions + ClusterFuzzLite + libFuzer**  
GitLab runner + ClusterFuzzLite + libFuzer  
[GitLab fuzzing](https://docs.gitlab.com/ee/user/application_security/coverage_fuzzing/) (libFuzzer), [C++ example](https://gitlab.com/gitlab-org/security-products/demos/coverage-fuzzing/c-cpp-fuzzing-example)  

### Notes  
The choice is mostly betwen libFuzzer vs AFL++.  
* libFuzzer needs test cases to be prepared explicitly whereas AFL++ creates it's internal tast cases automatically (and this is not only the proc but brings more cons - make it unmodifiable, irrelevant and slow)  
* libFuzzer is integrated into LLVM/clang what make it highly compatible/integrated with other CI/CD tools. But also can be used as a dedicated static lib.  
  AFL++ supports gcc and clang. But almost none of CI/CD tools declare AFL/AFL++ support.  
* GitHub & GitLab (+GCP, +Azure) can run libFuzzer-based fuzzers.  
* libFuzzer is highly flexible with test scenarios - we can customize dictionaries and test corpus/seeds as well keep it orginized.  
  AFL++ also supports customized dictionaries and test corpus (same file format/can be shared) but less flexibility with its structuring.  

### Conclusion  
libFuzzer is an excellent tool for fuzzing - functional, lighting fast and integrated in other tools and services.  
