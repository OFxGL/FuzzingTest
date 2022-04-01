#include "fuzzer.h"

extern "C" int LLVMFuzzerRunDriver(int *argc, char ***argv,
                  int (*UserCb)(const uint8_t *Data, size_t Size));

int main(int argc, char** argv)
{
    LLVMFuzzerRunDriver(&argc, &argv, LLVMFuzzerTestOneInput);
    return 0;
}