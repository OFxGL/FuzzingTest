#include <iostream>
#include "fuzz.h"

using namespace std;

int main(int argc, char** argv)
{
    const string input = string(istreambuf_iterator<char>(cin), istreambuf_iterator<char>());
    
    FUZZ fuzz(input);
    fuzz("FUZZ");
    fuzz("BUZZ");

    return 0;
}