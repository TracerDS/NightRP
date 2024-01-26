#pragma once

#include <cstddef>
#include <string>

namespace NightRP::Utils {
	inline constexpr std::size_t strlen(const char* string) noexcept {
		std::size_t size = 0;
		for (; string[size]; size++);
		return size;
	}

	inline constexpr std::string stringLower(const std::string_view string) noexcept {
		std::string temp;

		std::transform(string.begin(), string.end(), temp.begin(),
			[](const auto& c) { return std::tolower(c); });

		return temp;
	}

	inline constexpr bool stringReplace(
		std::string& str,
		const std::string& from,
		const std::string& to
	) noexcept {
		auto startPos = str.find(from);
		if (startPos == std::string::npos)
			return false;

		str.replace(startPos, from.length(), to);
		return true;
	}

	inline constexpr std::vector<std::string> stringSplit(
		const std::string& string,
		const std::string_view& delimiter
	) noexcept {
		std::vector<std::string> vec;

		std::size_t last = 0;
		std::size_t next = 0;
		while ((next = string.find(delimiter, last)) != string.npos) {
			vec.push_back(string.substr(last, next - last));
			last = next + 1;
		}
		vec.push_back(string.substr(last));

		return vec;
	}

	template <typename Type>
	inline constexpr const Type& Clamp(
		const Type& value,
		const Type& min,
		const Type& max
	) noexcept { return value < min ? min : value > max ? max : value; }

	template <typename From, typename To>
	inline constexpr To Cast(const From& value) noexcept {
		return static_cast<To>(value);
	}

	template <typename Type>
	inline constexpr std::uint8_t ToUINT8(const Type& value) noexcept {
		return Cast<Type, std::uint8_t>(value);
	}
	template <typename Type>
	inline constexpr std::uint16_t ToUINT16(const Type& value) noexcept {
		return Cast<Type, std::uint16_t>(value);
	}
	template <typename Type>
	inline constexpr std::uint32_t ToUINT32(const Type& value) noexcept {
		return Cast<Type, std::uint32_t>(value);
	}

	template <typename Range, typename Value = typename Range::value_type>
	inline constexpr std::string Join(
		Range const& elements,
		const char* const delimiter
	) noexcept {
		std::ostringstream os;
		auto b = begin(elements), e = end(elements);

		if (b != e) {
			std::copy(b, prev(e), std::ostream_iterator<Value>(os, delimiter));
			b = prev(e);
		}
		if (b != e) {
			os << *b;
		}

		return os.str();
	}
}