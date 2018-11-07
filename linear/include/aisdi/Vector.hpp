#pragma once

#include <algorithm>
#include <memory>
#include <cassert>
#include <initializer_list>
#include <iterator>

namespace aisdi {

template<typename T>
class Vector
{
	constexpr static auto Multiplier = 2;

public:
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = T*;
	using const_iterator = const T*;
	using size_type = std::size_t;
	using difference_type = typename std::iterator_traits<iterator>::difference_type;

	Vector() noexcept = default;

	explicit
	Vector(size_type count)
		:	_buffer(std::make_unique<T[]>(count))
		,	_size(count)
		,	_capacity(count)
	{
		// Postconditions
		assert(_size == count);
		assert(_capacity == _size);
	}

	Vector(size_type count, const T& item)
		:	_buffer(std::make_unique<T[]>(count))
		,	_size(count)
		,	_capacity(count)
	{
		std::fill(begin(), end(), item);

		// Postconditions
		assert(_size == count);
		assert(_capacity == _size);
	}

	Vector(std::initializer_list<T> ilist)
		:	_buffer(std::make_unique<T[]>(ilist.size()))
		,	_size(ilist.size())
		,	_capacity(ilist.size())
	{
		std::copy(ilist.begin(), ilist.end(), begin());

		// Postconditions
		assert(_size == ilist.size());
		assert(_capacity == _size);
	}

	Vector(const Vector& other)
		:	_buffer(std::make_unique<T[]>(other._capacity))
		,	_size(other._size)
		,	_capacity(other._capacity)
	{
		std::copy(other.begin(), other.end(), begin());

		// Postconditions
		assert(_size == other.size());
		assert(_capacity == other._capacity);
	}

	Vector(Vector&& other) noexcept
		:	_buffer(std::move(other._buffer))
		,	_size(other._size)
		,	_capacity(other._capacity)
	{
		other._size = 0;
		other._capacity = 0;
	}

	Vector& 
	operator=(const Vector& other)
	{
		// NOTE: Strong exception safety

		auto newBuffer = std::make_unique<T[]>(other._capacity);
		std::copy(other.begin(), other.end(), newBuffer.get());

		_buffer = std::move(newBuffer);
		_size = other._size;
		_capacity = other._capacity;

		return *this;
	}

	Vector& 
	operator=(Vector&& other) noexcept
	{
		_buffer = std::move(other._buffer);
		_size = other._size;
		_capacity = other._capacity;

		other._size = other._capacity = 0;

		return *this;
	}

	iterator
	begin()
	{
		return {_buffer.get()};
	}

	const_iterator
	begin() const
	{
		return {_buffer.get()};
	}

	iterator
	end()
	{
		return {_buffer.get() + _size};
	}

	const_iterator
	end() const
	{
		return {_buffer.get() + _size};
	}

	const_iterator
	cbegin() const
	{
		return begin();
	}

	const_iterator
	cend() const
	{
		return end();
	}

	pointer
	data() noexcept
	{
		return _buffer.get();
	}

	const_pointer
	data() const noexcept
	{
		return _buffer.get();
	}

	reference 
	operator[](size_type pos)
	{
		// Preconditions
		assert(pos < _size);
		assert(_buffer);

		return _buffer[pos];
	}

	const_reference
	operator[](size_type pos) const
	{
		// Preconditions
		assert(pos < _size);
		assert(_buffer);

		return _buffer[pos];
	}

	T
	popBack()
	{
		// Preconditions
		assert(_size > 0);
		assert(_buffer);

		// NOTE: Basic exception safety

		--_size;
		return _buffer[_size];
	}

	T
	popFront()
	{
		// Preconditions
		assert(_size > 0);
		assert(_buffer);

		// NOTE: Basic exception safety

		// Copy the first item as a result
		const auto result = _buffer[0];

		// Shift items from right to left by one
		for(auto it = begin(); it != std::prev(end()); ++it)
		{
			*it = *std::next(it);
		}

		--_size;
		return result;
	}

	void 
	append(const T& item)
	{
		const auto newSize = (_size + 1);
		if(newSize > _capacity)
		{
			// There is no space in current buffer. Make a new one, larger.
			const auto newCapacity = (newSize * Multiplier);
			auto newBuffer = std::make_unique<T[]>(newCapacity);

			// Copy current data into new buffer
			std::copy(begin(), end(), newBuffer.get());

			// Append desired element at the end of new buffer
			newBuffer[_size] = item;

			// Switch into new buffer
			_buffer = std::move(newBuffer);
			_capacity = newCapacity;
		}
		else
		{
			// Buffer is big enough to hold new item
			// Append desired element at the end
			assert(_buffer);
			_buffer[_size] = item;
		}

		_size = newSize;
	}

	void
	prepend(const T& item)
	{	
		const auto newSize = (_size + 1);
		if(newSize > _capacity)
		{
			// There is no space in current buffer. Make a new one, larger.
			const auto newCapacity = (newSize * Multiplier);
			auto newBuffer = std::make_unique<T[]>(newCapacity);

			// Copy current data into new buffer from the second position
			std::copy(begin(), end(), newBuffer.get() + 1);

			// Prepend desired element to the begin
			newBuffer[0] = item;

			// Switch into new buffer and change capacity
			_buffer = std::move(newBuffer);
			_capacity = newCapacity;
		}
		else
		{
			// Buffer is big enough to hold new item
			// Prepend desired element to the begin
			assert(_buffer);
			_buffer[0] = item;
		}

		_size = newSize;
	}

	size_type 
	size() const noexcept
	{
		return _size;
	}

	bool 
	empty() const noexcept
	{
		return (_size == 0);
	}

private:
	std::unique_ptr<T[]> _buffer;
	size_type _size = 0;
	size_type _capacity = 0;
};

template<typename T>
inline bool 
operator==(const T& lhs, const T& rhs)
	noexcept(noexcept(*lhs.data() == *rhs.data()))
{
	if(lhs.size() != rhs.size())
	{
		return false;
	}

	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T>
inline bool 
operator!=(const T& lhs, const T& rhs)
	noexcept(noexcept(lhs == rhs))
{
	return !(lhs == rhs);
}

} // namespace aisdi