#pragma once

#include <string>
#include <sstream>

namespace fusion {

template <class CharType, class Traits = std::char_traits<CharType>, class Allocator = std::allocator<CharType>>
class basic_stringbuilder
{
public:
	typedef std::basic_string<CharType, Traits, Allocator> string_type;

	template <typename T>
	basic_stringbuilder& operator<<(const T& t)
	{
		m_ss << t;
		return *this;
	}

	basic_stringbuilder& operator<<(const string_type& str)
	{
		m_ss << str;
		return *this;
	}

	string_type str() const
	{
		return m_ss.str();
	}

	const CharType* c_str() const
	{
		return m_ss.str().c_str();
	}

	operator string_type() const
	{
		return m_ss.str();
	}

private:
	std::basic_ostringstream<CharType, Traits, Allocator> m_ss;
};

typedef basic_stringbuilder<char> stringbuilder;
typedef basic_stringbuilder<wchar_t> wstringbuilder;

} // namespace fusion
