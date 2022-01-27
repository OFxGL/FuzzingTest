#include "fuzz.h"

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        FUZZ fuzz(argv[1]);
        fuzz("FUZZ");
        fuzz("BUZZ");
    }
    return 0;
}