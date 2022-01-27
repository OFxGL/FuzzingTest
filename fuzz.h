#pragma once

#include <string_view>
#include <stdexcept>

using namespace std;

class FUZZ
{
private:
	const std::string_view m_pattern;

public:
    FUZZ(const std::string_view& pattern) : m_pattern(pattern) {}

	inline void operator()(const std::string_view& data) const {
        if (m_pattern == data)
            throw runtime_error(m_pattern.data());
    }
};
