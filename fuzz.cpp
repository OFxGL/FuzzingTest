#include <string_view>

using namespace std;

class FUZZ
{
private:
	const string_view m_pattern;
public:
    FUZZ(const string_view& pattern) : m_pattern(pattern) {}

	void operator()(const string_view& data) const;
};

void FUZZ::operator()(const string_view& data) const
{
    if (m_pattern == data)
        throw m_pattern;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    FUZZ fuzz(string_view((string_view::value_type*)data, size));
    fuzz("FUZZ");
    fuzz("BUZZ");

    return 0;  // Non-zero return values are reserved for future use.
}