#include <cstddef>
#include <stdint.h>
#include <string_view>

using namespace std;

class FUZZ
{
private:
	static constexpr string_view m_pattern = "FUZZ";
    bool m_got;

public:
    FUZZ(const uint8_t* data, size_t size) 
        : m_got(m_pattern.size() == size && 
                m_pattern == string_view((string_view::value_type*)data, size)) {}

    inline operator bool() const {return m_got;}
	inline void operator()() const {if (m_got) throw m_pattern;}
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    FUZZ fuzz(data, size);
    fuzz();

    return 0;  // Non-zero return values are reserved for future use.
}