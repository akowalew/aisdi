#ifndef AISDI_TREEMAP_HPP
#define AISDI_TREEMAP_HPP

#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

#include <gsl/gsl_assert>
#include <gsl/pointers>

namespace aisdi {

template<typename Key,
         typename T,
         typename Compare = std::less<Key>>
class TreeMap
{
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
        // Postconditions
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

    TreeMap(TreeMap&& other) noexcept
        :   TreeMap()
    {
        *this = std::move(other);

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

    TreeMap& operator=(TreeMap&& other) noexcept
    {
        if(this != &other)
        {
            clear();

            root_.left = other.root_.left;
            first_ = (other.first_ != &other.root_) ? other.first_ : gsl::make_not_null(&root_);
            size_ = other.size_;
            other.repair();
            if(root_.left)
            {
                root_.left->parent = &root_;
            }
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
        auto& self = const_cast<TreeMap&>(*this);
        const iterator it = self.begin();
        return it;
    }

    const_iterator end() const
    {
        auto& self = const_cast<TreeMap&>(*this);
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
        auto node = first_;
        while(true)
        {
            while(node->parent && !node->right)
            {
                const auto parent = gsl::make_not_null(node->parent);
                delete static_cast<Node*>(node.get());
                node = parent;
            }

            if(!node->parent)
            {
                break;
            }

            const auto next = gsl::make_not_null(node->right);
            node->right = nullptr;
            node = next->leftmost();
        }

        repair();

        Ensures(empty());
        Ensures(size() == 0);
        Ensures(begin() == end());
    }

    iterator erase(const iterator& pos)
    {
        Expects(pos != end()); // Implies also (node->parent) precondition

        const auto result = std::next(pos);
        const auto node = gsl::make_not_null(pos.node_);
        const auto parent = gsl::make_not_null(node->parent);

        auto left = node->left;
        auto right = node->right;
        Node* next = nullptr;

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
                right->leftmost()->left = rightOfNext;
            }

            next->right = right;
        }
        else if(right)
        {
            next = right;
        }

        if(parent->left == node)
        {
            parent->left = next;
            if(node == first_)
            {
                first_ = (next) ? next->leftmost() : parent;
            }
        }
        else
        {
            parent->right = next;
        }

        if(next)
        {
            next->parent = parent;
        }

        delete static_cast<Node*>(node.get());
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
        const auto result = locate(key);
        const auto status = result.first;
        if(status == LocateStatus::Found)
        {
            const auto node = result.second;
            return iterator{node};
        }

        return end();
    }

    const_iterator find(const key_type& key) const
    {
        auto& self = const_cast<TreeMap&>(*this);
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
        return const_cast<TreeMap&>(*this).at(key);
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
        if(status == LocateStatus::Found)
        {
            const auto node = result.second;
            return std::make_pair(iterator{node}, false);
        }

        auto parent = result.second;
        const auto node =
            gsl::make_not_null(
                new Node(parent,
                         std::piecewise_construct,
                         std::forward_as_tuple(key),
                         std::forward_as_tuple(std::forward<Args>(args)...)));

        if(status == LocateStatus::OnLeft)
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
    struct Node; // Forward declaration

    struct BasicNode
    {
        BasicNode* parent = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;

        gsl::not_null<BasicNode*> leftmost()
        {
            auto node = gsl::make_not_null(this);
            while(node->left)
            {
                node = gsl::make_not_null(node->left);
            }

            return node;
        }

        gsl::not_null<BasicNode*> rightmost()
        {
            auto node = gsl::make_not_null(this);
            while(node->right)
            {
                node = gsl::make_not_null(node->right);
            }

            return node;
        }

        gsl::not_null<BasicNode*> leftparent()
        {
            auto node = gsl::make_not_null(this);
            while(true)
            {
                const auto prev = node;
                node = gsl::make_not_null(node->parent); // Also, was not the end()
                if(node->right != prev)
                {
                    break;
                }
            }

            return node;
        }

        gsl::not_null<BasicNode*> rightparent()
        {
            auto node = gsl::make_not_null(this);
            while(true)
            {
                const auto prev = node;
                node = gsl::make_not_null(node->parent); // Also, was not the end()
                if(node->left != prev)
                {
                    break;
                }
            }

            return node;
        }
    };

    struct Node
        :   public BasicNode
    {
        template<typename... Args>
        explicit Node(gsl::not_null<BasicNode*> parent, Args&&... args)
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
        OnLeft,
        OnRight
    };

    std::pair<LocateStatus, gsl::not_null<BasicNode*>> locate(const key_type& key)
    {
        auto parent = gsl::make_not_null(&root_);
        auto node = parent->left;
        auto status = LocateStatus::OnLeft;

        while(node)
        {
            const auto current = gsl::make_not_null(node);

            Expects(current->parent); // Is not the end()
            const auto& nodeKey = static_cast<Node*>(current.get())->key();
            if(nodeKey == key)
            {
                return std::make_pair(LocateStatus::Found, current);
            }

            parent = current;
            if(comp_(key, nodeKey))
            {
                status = LocateStatus::OnLeft;
                node = current->left;
            }
            else
            {
                status = LocateStatus::OnRight;
                node = current->right;
            }
        }

        return std::make_pair(status, parent);
    }

    void repair()
    {
        root_.left = nullptr;
        first_ = gsl::make_not_null(&root_);
        size_ = 0;
    }

    BasicNode root_;
    gsl::not_null<BasicNode*> first_ = gsl::make_not_null(&root_);
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
        const auto node = gsl::make_not_null(node_);
        Expects(node->parent); // Is not the end()
        return static_cast<Node*>(node.get())->value;
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

    ConstIterator& operator++()
    {
        auto node = gsl::make_not_null(node_);
        if(node->right)
        {
            node = node->right->leftmost();
        }
        else
        {
            node = node->leftparent();
        }

        node_ = node;
        return *this;
    }

    const ConstIterator operator++(int)
    {
        const auto result = *this;
        operator++();
        return result;
    }

    ConstIterator& operator--()
    {
        auto node = gsl::make_not_null(node_);
        if(node->left)
        {
            node = node->left->rightmost();
        }
        else
        {
            node = node->rightparent();
        }

        node_ = node;
        return *this;
    }

    const ConstIterator operator--(int)
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

    const Iterator operator++(int)
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

    const Iterator operator--(int)
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

} // namespace aisdi

#endif
