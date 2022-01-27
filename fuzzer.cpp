#include "fuzz.h"

using namespace std;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    FUZZ fuzz(string_view((string_view::value_type*)data, size));
    fuzz("FUZZ");
    fuzz("BUZZ");

    return 0;  // Non-zero return values are reserved for future use.
}