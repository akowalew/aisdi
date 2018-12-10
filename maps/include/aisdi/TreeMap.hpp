#pragma once

#include <algorithm>
#include <iterator>
#include <utility>
#include <memory>
#include <stdexcept>

#include <gsl/gsl_assert>

namespace aisdi {

template<typename Key,
         typename T,
         typename Compare = std::less<Key>>
class TreeMap
{
    using This = TreeMap<Key, T, Compare>;

public:
    class ConstIterator;
    class Iterator;

    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

    TreeMap()
    {
        Ensures(empty());
        Ensures(size() == 0);
        Ensures(begin() == end());
    }

    TreeMap(const TreeMap& other)
        :   TreeMap()
    {
        insert(other.begin(), other.end());

        Ensures(*this == other);
    }

    TreeMap(TreeMap&& other)
        :   root_{nullptr, other.root_.left, nullptr}
        ,   first_{(other.first_ != &other.root_) ? other.first_ : &root_}
        ,   comp_{Compare{}}
        ,   size_{other.size_}
    {
        other.repair();

        Ensures(other.empty());
        Ensures(other.size() == 0);
        Ensures(other.begin() == other.end());
    }

    TreeMap(std::initializer_list<value_type> ilist)
        :   TreeMap()
    {
        insert(ilist);

        Ensures(size() == ilist.size());
        Ensures(std::all_of(ilist.begin(), ilist.end(),
            [this](const auto& v) { return contains(v.first); }));
    }

    TreeMap& operator=(const TreeMap& other)
    {
        if(this != &other)
        {
            clear();
            insert(other.begin(), other.end());
        }

        Ensures(*this == other);
        return *this;
    }

    TreeMap& operator=(TreeMap&& other)
    {
        if(this != &other)
        {
            clear();

            root_.left = other.root_.left;
            first_ = (other.first_ != &other.root_) ? other.first_ : &root_;
            size_ = other.size_;

            other.repair();
        }

        Ensures(other.empty());
        Ensures(other.size() == 0);
        Ensures(other.begin() == other.end());
        return *this;
    }

    ~TreeMap()
    {
        clear();
    }

    iterator begin()
    {
        Expects(first_);

        return iterator{first_};
    }

    iterator end()
    {
        return iterator{&root_};
    }

    const_iterator begin() const
    {
        This& self = const_cast<This&>(*this);
        const iterator it = self.begin();
        return it;
    }

    const_iterator end() const
    {
        This& self = const_cast<This&>(*this);
        const iterator it = self.end();
        return it;
    }

    const_iterator cbegin() const
    {
        return begin();
    }

    const_iterator cend() const
    {
        return end();
    }

    void clear() noexcept
    {
        Expects(first_);
        auto node = first_;

        while(true)
        {
            Expects(node);
            while(node->parent && !node->right)
            {
                const auto parent = node->parent;
                delete static_cast<Node*>(node);
                node = parent;
            }

            if(!node->parent)
            {
                break;
            }

            Expects(node->right);
            const auto next = node->right;
            node->right = nullptr;
            node = next;

            while(node->left)
            {
                node = node->left;
            }
        }

        repair();

        Ensures(empty());
        Ensures(size() == 0);
        Ensures(begin() == end());
    }

    iterator erase(const iterator& pos)
    {
        Expects(pos != end());
        Expects(pos.node_);
        const auto node = pos.node_;
        auto left = node->left;
        auto right = node->right;

        const auto result = std::next(pos);

        Expects(node->parent);
        const auto parent = node->parent;

        BasicNode* next = nullptr;

        if(left)
        {
            next = left;
            const auto rightOfNext = next->right;

            if(!right)
            {
                right = rightOfNext;
            }
            else
            {
                auto notLess = right;
                while(notLess->left)
                {
                    notLess = notLess->left;
                }

                notLess->left = rightOfNext;
            }

            next->right = right;
        }
        else if(right)
        {
            next = right;

            // TODO: balance this
        }

        if(next)
        {
            next->parent = parent;
        }

        if(parent->left == node)
        {
            parent->left = next;

            if(node == first_)
            {
                if(!next)
                {
                    first_ = parent;
                }
                else
                {
                    auto node = next;
                    while(node->left)
                    {
                        node = node->left;
                    }

                    first_ = node;
                }
            }
        }
        else
        {
            parent->right = next;
        }

        delete static_cast<Node*>(node);
        --size_;
        return result;
    }

    size_type erase(const key_type& key)
    {
        auto pos = find(key);
        if(pos == end())
        {
            return 0;
        }

        erase(pos);
        return 1;
    }

    iterator find(const key_type& key)
    {
        auto node = root_.left;
        while(node)
        {
            Expects(node->parent); // Is not the end(), root_
            const auto& nodeKey = static_cast<Node*>(node)->key();
            if(nodeKey == key)
            {
                break;
            }
            else if(comp_(key, nodeKey))
            {
                node = node->left;
            }
            else
            {
                node = node->right;
            }
        }

        return node ? iterator{node} : end();
    }

    const_iterator find(const key_type& key) const
    {
        This& self = const_cast<This&>(*this);
        iterator it = self.find(key);
        return it;
    }

    bool contains(const Key& key) const
    {
        return (find(key) != end()) ? true : false;
    }

    mapped_type& at(const key_type& key)
    {
        auto it = find(key);
        Expects(it != end());

        return it->second;
    }

    const mapped_type& at(const key_type& key) const
    {
        return const_cast<This&>(*this).at(key);
    }

    mapped_type& operator[](const key_type& key)
    {
        return this->try_emplace(key).first->second;
    }

    std::pair<iterator, bool> insert(const value_type& value)
    {
        return try_emplace(value.first, value.second);
    }

    template<typename InputIt>
    void insert(InputIt first, InputIt last)
    {
        std::for_each(first, last,
            [this](const auto& value)
            {
                this->insert(value);
            });
    }

    void insert(std::initializer_list<value_type> ilist)
    {
        insert(ilist.begin(), ilist.end());
    }

    template<typename... Args>
    std::pair<iterator, bool> try_emplace(const key_type& key, Args&&... args)
    {
        auto result = locate(key);
        auto status = result.first;
        Expects(result.second);
        if(status == LocateStatus::Found)
        {
            const auto node = result.second;
            return std::make_pair(iterator{node}, false);
        }

        auto parent = result.second;
        const auto node = new Node(parent,
                                   std::piecewise_construct,
                                   std::forward_as_tuple(key),
                                   std::forward_as_tuple(std::forward<Args>(args)...));

        if(status == LocateStatus::IsLess)
        {
            parent->left = node;

            if(parent == first_)
            {
                first_ = node;
            }
        }
        else
        {
            parent->right = node;
        }

        ++size_;
        return std::make_pair(iterator{node}, true);
    }

    size_type size() const noexcept
    {
        return size_;
    }

    bool empty() const noexcept
    {
        return (size_ == 0);
    }

private:
    struct BasicNode
    {
        BasicNode* parent = nullptr;
        BasicNode* left = nullptr;
        BasicNode* right = nullptr;
    };

    struct Node
        :   public BasicNode
    {
        template<typename... Args>
        Node(BasicNode* parent, Args&&... args)
            :   BasicNode{parent, nullptr, nullptr}
            ,   value{std::forward<Args>(args)...}
        {}

        const key_type& key() const noexcept
        {
            return value.first;
        }

        mapped_type& mapped()
        {
            return value.second;
        }

        const mapped_type& mapped() const
        {
            return value.second;
        }

        value_type value;
    };

    enum class LocateStatus
    {
        Found,
        IsLess,
        IsGreater
    };

    std::pair<LocateStatus, BasicNode*> locate(const key_type& key)
    {
        auto parent = &root_;
        auto node = parent->left;
        auto status = LocateStatus::IsLess;
        while(node)
        {
            Expects(node->parent); // Is not the end(), root_
            const auto& nodeKey = static_cast<Node*>(node)->key();
            if(nodeKey == key)
            {
                return std::make_pair(LocateStatus::Found, node);
            }

            parent = node;
            if(comp_(key, nodeKey))
            {
                status = LocateStatus::IsLess;
                node = node->left;
            }
            else
            {
                status = LocateStatus::IsGreater;
                node = node->right;
            }
        }

        return std::make_pair(status, parent);
    }

    void repair()
    {
        root_.left = nullptr;
        first_ = &root_;
        size_ = 0;
    }

    BasicNode root_;
    BasicNode* first_ = &root_;
    Compare comp_ = Compare{};
    size_type size_ = 0;
};

template<typename Key, typename T, typename Compare>
bool operator==(const TreeMap<Key, T, Compare>& lhs,
                const TreeMap<Key, T, Compare>& rhs)
{
    if(lhs.size() != rhs.size())
    {
        return false;
    }

    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename Key, typename T, typename Compare>
bool operator!=(const TreeMap<Key, T, Compare>& lhs,
                const TreeMap<Key, T, Compare>& rhs)
{
    return !(lhs == rhs);
}

template<typename Key,
         typename T,
         typename Compare>
class TreeMap<Key, T, Compare>::ConstIterator
{
    friend TreeMap<Key, T, Compare>;

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::pair<const Key, T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const std::pair<const Key, T>*;
    using reference = const std::pair<const Key, T>&;

    ConstIterator() = default;

    reference operator*() const
    {
        Expects(node_);
        Expects(node_->parent); // Is not then end(), root_

        return static_cast<Node*>(node_)->value;
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

    ConstIterator& operator++()
    {
        Expects(node_);

        auto node = node_;
        if(node->right)
        {
            node = node->right;
            while(node->left)
            {
                node = node->left;
            }
        }
        else
        {
            while(true)
            {
                Expects(node->parent); // Also, was not the last (most-right), end(), root_
                if(node->parent->right != node)
                {
                    break;
                }

                node = node->parent;
            }

            node = node->parent;
        }

        node_ = node;
        return *this;
    }

    ConstIterator operator++(int)
    {
        const auto result = *this;
        operator++();
        return result;
    }

    ConstIterator& operator--()
    {
        Expects(node_);

        auto node = node_;
        if(node->left)
        {
            node = node->left;
            while(node->right)
            {
                node = node->right;
            }
        }
        else
        {
            while(true)
            {
                Expects(node->parent); // Also, was not the first (most-left)
                if(node->parent->left != node)
                {
                    break;
                }

                node = node->parent;
            }

            node = node->parent;
        }

        node_ = node;
        return *this;
    }

    ConstIterator operator--(int)
    {
        const auto result = *this;
        operator--();
        return result;
    }

    bool operator==(const ConstIterator& rhs) const
    {
        return (rhs.node_ == node_);
    }

    bool operator!=(const ConstIterator& rhs) const
    {
        return !(rhs == *this);
    }

private:
    explicit ConstIterator(BasicNode* node)
        :   node_(node)
    {
        Ensures(node_);
    }

    BasicNode* node_ = nullptr;
};

template<typename Key,
         typename T,
         typename Compare>
class TreeMap<Key, T, Compare>::Iterator
    :   public ConstIterator
{
    friend TreeMap<Key, T, Compare>;

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::pair<const Key, T>;
    using difference_type = std::ptrdiff_t;
    using pointer = std::pair<const Key, T>*;
    using reference = std::pair<const Key, T>&;

    Iterator() = default;

    Iterator& operator++()
    {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        const auto result = *this;
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
        const auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    reference operator*() const
    {
        return const_cast<reference>(ConstIterator::operator*());
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

private:
    explicit Iterator(BasicNode* node)
        :   ConstIterator{node}
    {}
};

} // aisdi
