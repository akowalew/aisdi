#ifndef AISDI_VECTOR_HPP
#define AISDI_VECTOR_HPP

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <memory>

#include "aisdi/util.hpp"

#include "gsl/gsl_assert"

namespace aisdi {

template<typename T>
class Vector
{
	constexpr static auto ResizeMultiplier = 2;

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
		Ensures(_size == ilist.size());
		Ensures(_capacity == _size);
	}

	Vector(const Vector& other)
		:	_buffer(std::make_unique<T[]>(other._capacity))
		,	_size(other._size)
		,	_capacity(other._capacity)
	{
		std::copy(other.begin(), other.end(), begin());

		// Postconditions
		Ensures(_size == other.size());
		Ensures(_capacity == other._capacity);
	}

	Vector(Vector&& other) noexcept
		:	_buffer(std::move(other._buffer))
		,	_size(other._size)
		,	_capacity(other._capacity)
	{
		other._size = 0;
		other._capacity = 0;
	}

	~Vector() = default;

	Vector&
	operator=(const Vector& other)
	{
		if(this != &other)
		{
			// NOTE: Strong exception safety

			auto newBuffer = std::make_unique<T[]>(other._capacity);
			std::copy(other.begin(), other.end(), newBuffer.get());

			_buffer = std::move(newBuffer);
			_size = other._size;
			_capacity = other._capacity;
		}

		return *this;
	}

	Vector&
	operator=(Vector&& other) noexcept
	{
		if(this != &other)
		{
			_buffer = std::move(other._buffer);
			_size = other._size;
			_capacity = other._capacity;

			other._size = other._capacity = 0;
		}

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
		Expects(_size > 0);
		Expects(_buffer);

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
		Expects(_size > 0);
		Expects(_buffer);

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
		return insertImpl(const_cast<iterator>(pos), value);
	}

	T
	popBack()
	{
		// NOTE: Basic exception safety
		// Preconditions
		Expects(_size > 0);

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
		Expects(_size > 0);

		const auto result = front();

		const auto first = cbegin();
		const auto last = std::next(first);
		erase(first, last);

		return result;
	}

	iterator
	erase(const_iterator pos)
	{
		return eraseImpl(const_cast<iterator>(pos),
			const_cast<iterator>(std::next(pos)));
	}

	iterator
	erase(const_iterator first, const_iterator last)
	{
		return eraseImpl(const_cast<iterator>(first),
			const_cast<iterator>(last));
	}

	void resize(size_type count)
	{
		if(count < _size)
		{
			const auto first = std::next(begin(), static_cast<difference_type>(count));
			const auto last = end();
			erase(first, last);
			return;
		}

		if(count > _capacity)
		{
			auto newBuffer = std::make_unique<T[]>(count);
			std::move(begin(), end(), newBuffer.get());
			_buffer = std::move(newBuffer);
			_capacity = count;
		}

		_size = count;
		const auto last = end();
		const auto first = std::prev(last, static_cast<difference_type>(count));
		std::for_each(first, last,
			[](auto& item)
			{
				new (&item) T;
			});
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
	insertImpl(iterator pos, const T& value)
	{
		// Preconditions
		Expects(std::distance(begin(), pos) >= 0
			&& std::distance(pos, end()) >= 0);

		// NOTE: Basic exception safety

		const auto newSize = (_size + 1);
		if(newSize > _capacity)
		{
			const auto newCapacity = (newSize * ResizeMultiplier);
			auto newBuffer = std::make_unique<T[]>(newCapacity);

			const auto newPos = std::move(begin(), pos, newBuffer.get());
			*newPos = value;
			std::move(pos, end(), newPos + 1);

			_buffer = std::move(newBuffer);
			_capacity = newCapacity;
			_size = newSize;
			return newPos;
		}

		// Preconditions
		Expects(_buffer);

		std::move_backward(pos, end(), end() + 1);
		*pos = value;

		_size = newSize;
		return pos;
	}

	iterator
	eraseImpl(iterator first, iterator last)
	{
		if(first == last)
		{
			return last;
		}

		// NOTE: Basic exception safety
		// Preconditions
		Expects(std::distance(begin(), first) >= 0
			&& std::distance(first, end()) >= 0);
		Expects(std::distance(begin(), last) >= 0
			&& std::distance(last, end()) >= 0);
		Expects(std::distance(first, last) >= 0);

		const auto afterLastMoved = std::move(last, end(), first);
		util::destroy(afterLastMoved, end());

		const auto count = std::distance(first, last);
		Expects(count > 0);
		_size -= static_cast<size_type>(count);

		return first;
	}

	std::unique_ptr<T[]> _buffer;
	size_type _size = 0;
	size_type _capacity = 0;
};

template<typename T>
inline bool
operator==(const Vector<T>& lhs, const Vector<T>& rhs)
{
	if(lhs.size() != rhs.size())
	{
		return false;
	}

	return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template<typename T>
inline bool
operator!=(const Vector<T>& lhs, const Vector<T>& rhs)
{
	return !(lhs == rhs);
}

} // namespace aisdi

#endif
