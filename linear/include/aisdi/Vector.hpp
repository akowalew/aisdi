#pragma once

#include <algorithm>
#include <memory>
#include <cassert>
#include <initializer_list>
#include <iterator>

#include "aisdi/util.hpp"

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
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using size_type = std::size_t;
	using difference_type = typename std::iterator_traits<iterator>::difference_type;

	Vector() noexcept = default;

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

	reverse_iterator
	rbegin()
	{
		return reverse_iterator{end()};
	}

	const_reverse_iterator
	rbegin() const
	{
		return const_reverse_iterator{end()};
	}

	reverse_iterator
	rend()
	{
		return reverse_iterator{begin()};
	}

	const_reverse_iterator
	rend() const
	{
		return const_reverse_iterator{begin()};
	}

	const_reverse_iterator
	crbegin() const
	{
		return rbegin();
	}

	const_reverse_iterator
	crend() const
	{
		return rend();
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
	front()
	{
		// Preconditions
		assert(_size > 0);
		assert(_buffer);

		return _buffer[0];
	}

	const_reference
	front() const
	{
		return const_cast<Vector*>(this)->front();
	}

	reference
	back()
	{
		// Preconditions
		assert(_size > 0);
		assert(_buffer);

		return _buffer[_size - 1];
	}

	const_reference
	back() const
	{
		return const_cast<Vector*>(this)->back();
	}

	void
	append(const T& item)
	{
		insert(end(), item);
	}

	void
	prepend(const T& item)
	{
		insert(begin(), item);
	}

	iterator
	insert(const_iterator pos, const T& value)
	{
		// Preconditions
		assert(std::distance(cbegin(), pos) >= 0
			&& std::distance(pos, cend()) >= 0);

		// NOTE: Basic exception safety

		const auto newSize = (_size + 1);
		if(newSize > _capacity)
		{
			const auto newCapacity = (newSize * Multiplier);
			auto newBuffer = std::make_unique<T[]>(newCapacity);

			const auto newPos = std::copy(cbegin(), pos, newBuffer.get());
			*(newPos++) = value;
			std::copy(pos, cend(), newPos);

			_buffer = std::move(newBuffer);
			_capacity = newCapacity;
			_size = newSize;
			return newPos;
		}
		else
		{
			// Preconditions
			assert(_buffer);

			std::copy_backward(pos, cend(), end() + 1);
			*const_cast<iterator>(pos) = value;

			_size = newSize;
			return const_cast<iterator>(pos);
		}
	}

	T
	popBack()
	{
		// NOTE: Basic exception safety
		// Preconditions
		assert(_size > 0);

		const auto result = back();

		const auto last = cend();
		const auto first = std::prev(last);
		erase(first, last);

		return result;
	}

	T
	popFront()
	{
		// NOTE: Basic exception safety
		// Preconditions
		assert(_size > 0);

		const auto result = front();

		const auto first = cbegin();
		const auto last = std::next(first);
		erase(first, last);

		return result;
	}

	iterator
	erase(const_iterator first, const_iterator last)
	{
		return eraseImpl(const_cast<iterator>(first),
			const_cast<iterator>(last));
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
	iterator
	eraseImpl(iterator first, iterator last)
	{
		if(first == last)
		{
			return last;
		}

		// NOTE: Basic exception safety
		// Preconditions
		assert(std::distance(begin(), first) >= 0
			&& std::distance(first, end()) >= 0);
		assert(std::distance(begin(), last) >= 0
			&& std::distance(last, end()) >= 0);
		assert(std::distance(first, last) >= 0);

		const auto afterLastCopied = std::copy(last, end(), first);
		util::destroy(afterLastCopied, end());

		_size -= std::distance(first, last);

		return first;
	}

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
