#pragma once

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>

#include <gsl/gsl_assert>

#include "aisdi/List.hpp"
#include "aisdi/Vector.hpp"

namespace aisdi {

template<typename Key,
         typename T,
         typename Hash = std::hash<Key>,
         typename KeyEqual = std::equal_to<Key>>
class HashMap
{
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    struct Node
    {
        value_type value;

        const key_type& key() const
        {
            return value.first;
        }

        const mapped_type& mapped() const
        {
            return value.second;
        }

        mapped_type& mapped()
        {
            return value.second;
        }
    };

private:
    friend class ConstIterator;
    friend class Iterator;

    using Bucket = aisdi::List<Node>;
    using HashTable = aisdi::Vector<Bucket>;

    using BucketIterator = typename HashTable::iterator;
    using ConstBucketIterator = typename HashTable::const_iterator;
    using NodeIterator = typename Bucket::iterator;
    using ConstNodeIterator = typename Bucket::const_iterator;

    constexpr static size_type DefaultBucketCount = 10;
    constexpr static float DefaultMaxLoadFactor = 1.0f;

public:
    class Iterator;
    class ConstIterator;

    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using node_type = Node;

    explicit HashMap(size_type bucketCount = DefaultBucketCount)
    {
        init(bucketCount);
    }

    template<typename InputIt>
    HashMap(InputIt first, InputIt last,
            size_type bucketCount = DefaultBucketCount)
        :   HashMap(bucketCount)
    {
        std::for_each(first, last,
                      [this](const auto& value)
                      {
                          const auto& key = value.first;
                          const auto& mapped = value.second;
                          this->operator[](key) = mapped;
                      });
    }

    ~HashMap() = default;

    HashMap(const HashMap& other)
    {
        *this = other;
    }

    HashMap(HashMap&& other) noexcept
    {
        *this = std::move(other);
    }

    HashMap(std::initializer_list<value_type> init,
            size_type bucketCount = DefaultBucketCount)
        :   HashMap(init.begin(), init.end(), bucketCount)
    {}

    HashMap& operator=(const HashMap& other)
    {
        if(&other != this)
        {
            hashTable_ = other.hashTable_;
            maxLoadFactor_ = other.maxLoadFactor_;
            size_ = other.size_;
            keyEqual_ = other.keyEqual_;
            hasher_ = other.hasher_;
        }

        return *this;
    }

    HashMap& operator=(HashMap&& other) noexcept
    {
        if(&other != this)
        {
            hashTable_ = std::move(other.hashTable_);
            maxLoadFactor_ = other.maxLoadFactor_;
            size_ = other.size_;
            keyEqual_ = std::move(other.keyEqual_);
            hasher_ = std::move(other.hasher_);

            other.init();
        }

        return *this;
    }

    iterator begin()
    {
        Expects(hashTable_.size() > 0);

        auto bucketPos = hashTable_.begin();
        const auto lastBucketPos = std::prev(hashTable_.end());
        while(bucketPos->empty() && (bucketPos != lastBucketPos))
        {
            ++bucketPos;
        }

        return {bucketPos, bucketPos->begin(), *this};
    }

    const_iterator begin() const
    {
        return const_cast<HashMap&>(*this).begin();
    }

    const_iterator cbegin() const
    {
        return begin();
    }

    iterator end()
    {
        Expects(hashTable_.size() > 0);
        const auto bucketPos = std::prev(hashTable_.end());
        const auto nodePos = bucketPos->begin();
        return Iterator{bucketPos, nodePos, *this};
    }

    const_iterator end() const
    {
        return const_cast<HashMap&>(*this).end();
    }

    const_iterator cend() const
    {
        return end();
    }

    T& at(const key_type& key)
    {
        auto pos = find(key);
        if(pos == end())
        {
            throw std::out_of_range("Key not exist");
        }

        return pos->second;
    }

    const T& at(const key_type& key) const
    {
        return const_cast<HashMap&>(*this).at(key);
    }

    T& operator[](const key_type& key)
    {
        const auto result = insert({key, mapped_type{}});
        const auto pos = result.first;
        auto& mapped = pos->second;
        return mapped;
    }

    std::pair<iterator, bool> insert(const value_type& value)
    {
        const auto& key = value.first;
        const auto location = locate(key);
        const auto& bucketPos = location.first; // Replace these two lines in C++17
        const auto& nodePos = location.second;
        const auto bucketEnd = bucketPos->end();
        if(nodePos != bucketEnd)
        {
            return {iterator{bucketPos, nodePos, *this}, false};
        }

        const auto& mapped = value.second;
        const auto node = Node{{std::piecewise_construct,
                                std::forward_as_tuple(key),
                                std::forward_as_tuple(mapped)}};
        const auto insertedNodePos = bucketPos->insert(bucketEnd, node);
        ++size_;

        return {iterator{bucketPos, insertedNodePos, *this}, true};
    }

    template<class M>
    iterator insert_or_assign(const key_type& key, M&& obj)
    {
        const auto location = locate(key);
        const auto& bucketPos = location.first; // Replace these two lines in C++17
        const auto& nodePos = location.second;
        const auto bucketEnd = bucketPos->end();
        if(nodePos != bucketEnd)
        {
            nodePos->mapped() = std::move(obj);
            return iterator{bucketPos, nodePos, *this};
        }

        const auto node = Node{{std::piecewise_construct,
                                std::forward_as_tuple(key),
                                std::forward_as_tuple(std::forward<M>(obj))}};
        const auto insertedNodePos = bucketPos->insert(bucketEnd, node);
        ++size_;

        return iterator{bucketPos, insertedNodePos, *this};
    }

    iterator erase(const const_iterator& pos)
    {
        auto bucketPos = pos.bucketPos_;
        auto nodePos = pos.nodePos_;
        auto nextNodePos = bucketPos->erase(nodePos);
        --size_;
        const auto lastBucketPos = std::prev(hashTable_.end());
        while((nextNodePos == bucketPos->end())
            && (bucketPos != lastBucketPos))
        {
            ++bucketPos;
            nodePos = bucketPos->begin();
        }

        return iterator{bucketPos, nodePos, *this};
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

    bool contains(const key_type& key)
    {
        return (find(key) != end());
    }

    iterator find(const key_type& key)
    {
        const auto location = locate(key);
        const auto& bucketPos = location.first; // Replace these two lines in C++17
        const auto& nodePos = location.second;
        Expects(bucketPos != hashTable_.end());
        if(nodePos == bucketPos->end())
        {
            return end();
        }

        return iterator{bucketPos, nodePos, *this};
    }

    const_iterator find(const key_type& key) const
    {
        return const_cast<HashMap&>(*this).find(key);
    }

    size_type bucket(const Key& key) const
    {
        Expects(bucket_count() > 0);
        const auto hash = hasher_(key);
        return (hash % bucket_count());
    }

    float load_factor() const
    {
        Expects(bucket_count() > 0);
        const auto bucketCount = static_cast<float>(bucket_count());
        return (size() / bucketCount);
    }

    float max_load_factor() const noexcept
    {
        return maxLoadFactor_;
    }

    void max_load_factor(float maxLoadFactor)
    {
        maxLoadFactor_ = maxLoadFactor;
    }

    size_type bucket_count() const noexcept
    {
        return (hashTable_.size() - 1);
    }

    bool empty() const noexcept
    {
        return (size_ == 0);
    }

    size_type size() const noexcept
    {
        return size_;
    }

private:
    void init(size_type bucketCount = DefaultBucketCount)
    {
        Expects(hashTable_.empty());

        hashTable_.resize(bucketCount + 1);
        maxLoadFactor_ = DefaultMaxLoadFactor;
        size_ = 0;
        keyEqual_ = key_equal();
        hasher_ = hasher();

        Ensures(begin() == end());
        Ensures(empty());
        Ensures(size() == 0);
    }

    std::pair<BucketIterator, NodeIterator> locate(const key_type& key)
    {
        const auto bucketPos = bucketPosAt(key);
        const auto bucketBegin = bucketPos->begin();
        const auto bucketEnd = bucketPos->end();
        const auto nodePos =
            std::find_if(bucketBegin, bucketEnd,
                        [&key, this](const auto& node)
                        {
                            return keyEqual_(key, node.key());
                        });

        return std::make_pair(bucketPos, nodePos);
    }

    std::pair<ConstBucketIterator, ConstNodeIterator> locate(const key_type& key) const
    {
        return const_cast<HashMap&>(*this).locate(key);
    }

    BucketIterator bucketPosAt(const key_type& key)
    {
        const auto bucketIndex = bucket(key);
        Ensures(bucketIndex < bucket_count());
        const auto bucketPos = std::next(hashTable_.begin(),
                                         static_cast<difference_type>(bucketIndex));
        Ensures(bucketPos != hashTable_.end());
        return bucketPos;
    }

    ConstBucketIterator bucketPosAt(const key_type& key) const
    {
        return const_cast<HashMap&>(*this).bucketPosAt(key);
    }

    HashTable hashTable_;
    float maxLoadFactor_;
    size_type size_;
    key_equal keyEqual_;
    hasher hasher_;
};

template<typename Key,
         typename T,
         typename Hash,
         typename KeyEqual>
bool operator==(const HashMap<Key, T, Hash, KeyEqual>& lhs,
                const HashMap<Key, T, Hash, KeyEqual>& rhs)
{
    if(lhs.size() != rhs.size())
    {
        return false;
    }

    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename Key,
         typename T,
         typename Hash,
         typename KeyEqual>
bool operator!=(const HashMap<Key, T, Hash, KeyEqual>& lhs,
                const HashMap<Key, T, Hash, KeyEqual>& rhs)
{
    return !(lhs == rhs);
}

template<typename Key,
         typename T,
         typename Hash,
         typename KeyEqual>
class HashMap<Key, T, Hash, KeyEqual>::ConstIterator
{
    friend class HashMap;

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename HashMap::value_type;
    using difference_type = typename HashMap::difference_type;
    using pointer = typename HashMap::const_pointer;
    using reference = typename HashMap::const_reference;

    ConstIterator(BucketIterator bucketPos,
                  NodeIterator nodePos,
                  const HashMap& hashMap)
        :   bucketPos_(bucketPos)
        ,   nodePos_(nodePos)
        ,   hashMap_(hashMap)
    {}

    reference operator*() const
    {
        return nodePos_->value;
    }

    pointer operator->() const
    {
        return std::addressof(operator*());
    }

    ConstIterator& operator--()
    {
        while(nodePos_ == bucketPos_->begin())
        {
            Expects(bucketPos_ != hashMap_.hashTable_.begin());
            --bucketPos_;
            nodePos_ = bucketPos_->end();
        }

        --nodePos_;
        return *this;
    }

    ConstIterator operator--(int)
    {
        const auto result = *this;
        operator--();
        return result;
    }

    ConstIterator& operator++()
    {
        Expects(bucketPos_ != std::prev(hashMap_.hashTable_.end()));
        Expects(nodePos_ != bucketPos_->end());

        ++nodePos_;
        const auto lastBucketPos = std::prev(hashMap_.hashTable_.end());
        while((nodePos_ == bucketPos_->end())
            && (bucketPos_ != lastBucketPos))
        {
            ++bucketPos_;
            nodePos_ = bucketPos_->begin();
        }

        return *this;
    }

    ConstIterator operator++(int)
    {
        const auto result = *this;
        operator++();
        return result;
    }

    bool operator==(const ConstIterator& rhs) const
    {
        return (bucketPos_ == rhs.bucketPos_ && nodePos_ == rhs.nodePos_);
    }

    bool operator!=(const ConstIterator& rhs) const
    {
        return !(*this == rhs);
    }

private:
    BucketIterator bucketPos_;
    NodeIterator nodePos_;
    const HashMap& hashMap_;
};

template<typename Key,
         typename T,
         typename Hash,
         typename KeyEqual>
class HashMap<Key, T, Hash, KeyEqual>::Iterator
    :   public ConstIterator
{
    friend class HashMap;

public:
    using reference = typename HashMap::reference;
    using pointer = typename HashMap::pointer;
    using const_reference = typename HashMap::const_reference;
    using const_pointer = typename HashMap::const_pointer;

    Iterator(BucketIterator bucketPos,
             NodeIterator nodePos,
             const HashMap& hashMap)
        :   ConstIterator(bucketPos, nodePos, hashMap)
    {}

    reference operator*() const
    {
        return const_cast<reference>(ConstIterator::operator*());
    }

    pointer operator->() const
    {
        return std::addressof(operator*());
    }

    Iterator& operator--()
    {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int)
    {
        const auto result = *this;
        operator--();
        return result;
    }

    Iterator& operator++()
    {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        const auto result = *this;
        operator++();
        return result;
    }
};

} // namespace aisdi
