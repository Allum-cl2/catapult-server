#pragma once
#include "traits/Traits.h"
#include <ostream>
#include <string>
#include <stddef.h>
#include <stdint.h>

namespace catapult { namespace utils {

	/// Basic raw buffer that is composed of a pointer and a size.
	template<typename T>
	class BasicRawBuffer {
	public:
		/// Creates an empty buffer.
		constexpr BasicRawBuffer() : BasicRawBuffer(nullptr, 0)
		{}

		/// Creates a buffer around the entire contents of \a container.
		template<
				typename TContainer,
				// disable when copy/move constructors should be used
				typename X = traits::disable_if_same_or_derived<BasicRawBuffer, TContainer>,
				// disable when other constructors are better match
				typename Y = typename std::enable_if<!std::is_scalar<TContainer>::value>::type
		>
		BasicRawBuffer(TContainer&& container) : BasicRawBuffer(container.data(), container.size())
		{}

		/// Creates buffer around \a pRawBuffer pointer and \a size.
		constexpr BasicRawBuffer(T* pRawBuffer, size_t size) : pData(pRawBuffer), Size(size)
		{}

	public:
		/// The data pointer.
		T* pData;

		/// The data size.
		size_t Size;
	};

	/// A const binary buffer.
	using RawBuffer = BasicRawBuffer<const uint8_t>;

	/// A mutable binary buffer.
	using MutableRawBuffer = BasicRawBuffer<uint8_t>;

	/// A const string buffer.
	class RawString : public BasicRawBuffer<const char> {
	public:
		using BasicRawBuffer<const char>::BasicRawBuffer;

	public:
		/// Creates an empty string buffer.
		constexpr RawString() : BasicRawBuffer()
		{}

		/// Creates a string buffer around a NUL-terminated string (\a str).
		RawString(const char* str);
	};

	/// A mutable string buffer.
	class MutableRawString : public BasicRawBuffer<char> {
	public:
		using BasicRawBuffer<char>::BasicRawBuffer;

	public:
		/// Creates an empty mutable string buffer.
		constexpr MutableRawString() : BasicRawBuffer()
		{}

		/// Creates a mutable string buffer around \a str.
		MutableRawString(std::string& str);
	};

	/// Insertion operator for outputting \a str to \a out.
	template<
			typename T,
			typename X = typename std::enable_if<std::is_same<char, typename std::remove_const<T>::type>::value>::type>
	std::ostream& operator<<(std::ostream& out, const BasicRawBuffer<T>& str) {
		out.write(str.pData, static_cast<std::streamsize>(str.Size));
		return out;
	}
}}