#pragma once

#include <algorithm>
#include <memory>
#include <cassert>
#include <initializer_list>
#include <iterator>

namespace aisdi {

template<typename T>
class List
{
public:
	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using const_pointer = const T*;
	using const_reference = const T&;

	class ConstIterator;
	class Iterator;
	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	using difference_type = std::ptrdiff_t;
	using size_type = std::size_t;

	List()
	{
		_head.next = &_tail;
		_tail.prev = &_head;

		// Postconditions
		assert(empty());
		assert(size() == 0);
	}

	~List()
	{
		clear();
	}

	List(const List& other)
		:	List()
	{
		assign(other.begin(), other.end());
	}

	List(List&& other) noexcept
		:	List()
	{
		*this = std::move(other);
	}

	List&
	operator=(const List& other)
	{
		if(this != &other)
		{
			assign(other.begin(), other.end());
		}

		return *this;
	}

	List&
	operator=(List&& other)
	{
		if(this != &other)
		{
			clear();

			_head.next = other._head.next;
			_head.next->prev = &_head;

			_tail.prev = other._tail.prev;
			_tail.prev->next = &_tail;

			_size = other._size;

			other._head.next = &other._tail;
			other._tail.prev = &other._head;
			other._size = 0;

			// Postconditions
			assert(other.empty());
			assert(other.size() == 0);
		}

		return *this;
	}

	List(std::initializer_list<T> ilist)
		:	List()
	{
		assign(ilist.begin(), ilist.end());
	}

	iterator
	begin()
	{
		return {_head.next};
	}

	const_iterator
	begin() const
	{
		return {_head.next};
	}

	iterator
	end()
	{
		return {&_tail};
	}

	const_iterator
	end() const
	{
		return {&_tail};
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

	reference
	front()
	{
		// Preconditions
		assert(_size > 0);
		assert(_head.next);

		return reinterpret_cast<Node*>(_head.next)->data;
	}

	const_reference
	front() const
	{
		return const_cast<List*>(this)->front();
	}

	reference
	back()
	{
		// Preconditions
		assert(_size > 0);
		assert(_tail.prev);

		return reinterpret_cast<Node*>(_tail.prev)->data;
	}

	const_reference
	back() const
	{
		return const_cast<List*>(this)->back();
	}

	template<typename TInputIt>
	void
	assign(TInputIt first, TInputIt last)
	{
		clear();

		auto prevNode = &_head;
		std::for_each(first, last, [&prevNode](auto& value)
			{
				auto node = new Node{prevNode, nullptr, value};
				prevNode->next = node;
				prevNode = node;
			});

		prevNode->next = &_tail;
		_tail.prev = prevNode;
		_size = std::distance(first, last);
	}

	void
	clear()
	{
		auto node = _head.next;
		while(node->next)
		{
			const auto next = node->next;
			delete reinterpret_cast<Node*>(node);
			node = next;
		}

		_head.next = &_tail;
		_tail.prev = &_head;
		_size = 0;
	}

	void
	append(const T& value)
	{
		insert(end(), value);
	}

	void
	prepend(const T& value)
	{
		insert(begin(), value);
	}

	iterator
	insert(const const_iterator& pos, const T& value)
	{
		// NOTE: Strong exception safety

		auto nextNode = const_cast<BasicNode*>(pos._node);

		// Preconditions
		assert(nextNode);

		const auto prevNode = nextNode->prev;
		auto newNode = new Node(prevNode, nextNode, value);
		prevNode->next = newNode;
		nextNode->prev = newNode;
		++_size;

		return {newNode};
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
	erase(const const_iterator& pos)
	{
		return erase(pos, std::next(pos));
	}

	iterator
	erase(const const_iterator& first, const const_iterator& last)
	{
		if(first == last)
		{
			return last;
		}

		auto node = const_cast<BasicNode*>(first._node);
		auto lastNode = const_cast<BasicNode*>(last._node);
		assert(node);
		assert(lastNode);

		auto prevNode = node->prev;
		assert(prevNode); // prevNode should not be the head

		while(node != lastNode)
		{
			auto nextNode = node->next;
			assert(nextNode);

			delete reinterpret_cast<Node*>(node);
			node = nextNode;

			_size--;
		}

		prevNode->next = lastNode;
		lastNode->prev = prevNode;
		return last;
	}

	std::size_t
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
	struct BasicNode
	{
		BasicNode() = default;

		BasicNode(BasicNode* prev, BasicNode* next)
			:	prev(prev)
			,	next(next)
		{}

		BasicNode* prev = nullptr;
		BasicNode* next = nullptr;
	};

	struct Node
		:	public BasicNode
	{
		Node() = default;

		template<typename U>
		Node(BasicNode* prev, BasicNode* next, U&& data)
			:	BasicNode(prev, next)
			,	data(std::forward<U>(data))
		{}

		T data;
	};

	// NOTE: I've decided not to use RAII for managing Nodes lifetime
	//  because of perfomance impact during List's destruction.
	//  When using e.g. unique_ptr all items will be recursive deleted,
	//  so when large number of elements will be allocated, there could
	//  be risk of stack overflow.
	//  Without RAII, all items will be removed with simple loop in destructor.
	BasicNode _head;
	BasicNode _tail;
	std::size_t _size = 0;
};

template<typename T>
inline bool
operator==(const List<T>& lhs, const List<T>& rhs)
{
	if(lhs.size() != rhs.size())
	{
		return false;
	}

	return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template<typename T>
inline bool
operator!=(const List<T>& lhs, const List<T>& rhs)
{
	return !(lhs == rhs);
}

template<typename T>
class List<T>::ConstIterator
{
	using Node = List<T>::Node;
	friend List<T>;

public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename List::value_type;
	using difference_type = typename List::difference_type;
	using pointer = typename List::const_pointer;
	using const_pointer = typename List::const_pointer;
	using reference = typename List::const_reference;
	using const_reference = typename List::const_reference;

	ConstIterator(const BasicNode* node)
		:	_node(node)
	{
		// Pre/Postconditions
		assert(node);
	}

	const BasicNode*
	node() const
	{
		return _node;
	}

	const_reference
	operator*() const
	{
		// Preconditions
		assert(_node->next); // Is not a tail

		return reinterpret_cast<const Node*>(_node)->data;
	}

	const_pointer
	operator->() const
	{
		return std::addressof(operator*());
	}

	ConstIterator&
	operator++()
	{
		// Preconditions
		assert(_node->next);

		_node = _node->next;
		return *this;
	}

	ConstIterator
	operator++(int)
	{
		// Preconditions
		assert(_node->next);

		const auto result = *this;
		_node = _node->next;
		return result;
	}

	ConstIterator&
	operator--()
	{
		// Preconditions
		assert(_node->prev->prev); // Is not a head

		_node = _node->prev;
		return *this;
	}

	ConstIterator
	operator--(int)
	{
		// Preconditions
		assert(_node->prev->prev); // Is not a head

		const auto result = *this;
		_node = _node->prev;
		return result;
	}

	ConstIterator
	operator+(difference_type n) const
	{
		auto result = *this;
		while(n--)
		{
			++result;
		}

		return result;
	}

	ConstIterator
	operator-(difference_type n) const
	{
		auto result = *this;
		while(n--)
		{
			--result;
		}

		return result;
	}

	bool
	operator==(const ConstIterator& other) const noexcept
	{
		return (_node == other._node);
	}

	bool
	operator!=(const ConstIterator& other) const noexcept
	{
		return !(*this == other);
	}

private:
	const BasicNode* _node;
};

template<typename T>
class List<T>::Iterator
	:	public List<T>::ConstIterator
{
	friend List<T>;

public:
	using pointer = typename List::pointer;
	using reference = typename List::reference;

	Iterator(BasicNode* node)
		:	ConstIterator(node)
	{}

	Iterator(const ConstIterator& other)
		:	ConstIterator(other)
	{}

	BasicNode*
	node()
	{
		return const_cast<Node*>(ConstIterator::node());
	}

	Iterator& operator++()
	{
		ConstIterator::operator++();
		return *this;
	}

	Iterator operator++(int)
	{
		auto result = *this;
		ConstIterator::operator++();
		return result;
	}

	Iterator& operator--()
	{
		ConstIterator::operator--();
		return *this;
	}

	Iterator operator--(int)
	{
		auto result = *this;
		ConstIterator::operator--();
		return result;
	}

	Iterator operator+(difference_type n) const
	{
		return ConstIterator::operator+(n);
	}

	Iterator operator-(difference_type n) const
	{
		return ConstIterator::operator-(n);
	}

	reference operator*() const
	{
		// ugly cast, yet reduces code duplication.
		return const_cast<reference>(ConstIterator::operator*());
	}

	pointer operator->() const
	{
		return std::addressof(operator*());
	}
};

} // namespace aisdi
