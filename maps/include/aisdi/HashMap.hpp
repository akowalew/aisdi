#pragma once

#include <cstdint>
#include <utility>
#include <algorithm>
#include <stdexcept>

#include <gsl/gsl_assert>

#include "aisdi/List.hpp"
#include "aisdi/Vector.hpp"

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
            return value->key;
        }

        const mapped_type& mapped() const
        {
            return value->mapped;
        }

        mapped_type& mapped()
        {
            return value->mapped;
        }
    };

private:
    friend class ConstIterator;
    friend class Iterator;

    using Bucket = aisdi::List<Node>;
    using HashTable = aisdi::Vector<Bucket>;

    using global_iterator = typename HashTable::iterator;
    using const_global_iterator = typename HashTable::const_iterator;

    constexpr static size_type DefaultBucketCount = 10;
    constexpr static float DefaultMaxLoadFactor = 1.0f;

public:
    class Iterator;
    class ConstIterator;

    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using local_iterator = typename Bucket::iterator;
    using const_local_iterator = typename Bucket::const_iterator;
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
                          const auto& key = value->key();
                          const auto& mapped = value->mapped();
                          operator[](key) = mapped;
                      });
    }

    ~HashMap() = default;

    HashMap(const HashMap& other)
    {
        *this = other;
    }

    HashMap(HashMap&& other)
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

            const auto firstBucketOffset =
                std::distance(other.hashTable_.begin(), other.firstBucket_);
            firstBucket_ = std::next(hashTable_.begin(), firstBucketOffset);
        }

        return *this;
    }

    HashMap& operator=(HashMap&& other)
    {
        if(&other != this)
        {
            hashTable_ = std::move(other.hashTable_);
            firstBucket_ = std::move(other.firstBucket_);
            maxLoadFactor_ = other.maxLoadFactor_;
            size_ = other.size_;
            keyEqual_ = std::move(other.keyEqual_);
            hasher_ = std::move(other.hasher_);

            other.init();
        }

        return *this;
    }

    reference at(const key_type& key)
    {
        const auto pos = find(key);
        if(pos == end())
        {
            throw std::out_of_range("Key not exist");
        }

        return pos->mapped();
    }

    const_reference at(const key_type& key) const
    {
        return const_cast<HashMap&>(*this).at(key);
    }

    reference operator[](const key_type& key)
    {
        const auto location = locate(key);
        const auto& bucketPos = location.first; // Replace these two lines in C++17
        const auto& nodePos = location.second;
        const auto bucketEnd = bucketPos->end();
        if(nodePos != bucketEnd)
        {
            return nodePos->mapped();
        }

        const auto node = Node{{std::piecewise_construct,
                                std::forward_as_tuple(key),
                                std::forward_as_tuple(mapped_type{})}};
        const auto insertedNodePos = bucketPos->insert(bucketEnd, node);
        return insertedNodePos->mapped();
    }

    iterator erase(const const_iterator& pos)
    {
        auto& bucketPos = pos->bucketPos_;
        auto& nodePos = pos->nodePos_;
        auto nextNodePos = bucketPos->erase(nodePos);
        while((nextNodePos == bucketPos->end())
            && (bucketPos != hashTable_.end()))
        {
            ++bucketPos;
            nodePos = bucketPos->begin();
        }

        return iterator{bucketPos, nodePos};
    }

    size_type erase(const key_type& key)
    {
        const auto pos = find(key);
        if(pos == end())
        {
            return 0;
        }

        erase(pos);
        return 1;
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

        return iterator{bucketPos, nodePos};
    }

    const_iterator find(const key_type& key) const
    {
        return const_cast<HashMap&>(*this).find(key);
    }

    iterator begin()
    {
        const auto bucketPos = firstBucket_;
        const auto nodePos = bucketPos.begin();
        return {bucketPos, nodePos};
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
        return Iterator{bucketPos, nodePos};
    }

    const_iterator end() const
    {
        return const_cast<HashMap&>(*this).end();
    }

    const_iterator cend() const
    {
        return end();
    }

    size_type bucket(const Key& key) const
    {
        Expects(bucket_count() > 0);
        const auto hash = hasher(key);
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
        firstBucket_ = std::prev(hashTable_.end());
        maxLoadFactor_ = DefaultMaxLoadFactor;
        size_ = 0;
        keyEqual_ = key_equal();
        hasher_ = hasher();

        Ensures(begin() == end());
        Ensures(empty());
        Ensures(size() == 0);
    }

    std::pair<global_iterator, local_iterator> locate(const key_type& key)
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

    std::pair<const_global_iterator, const_local_iterator> locate(const key_type& key) const
    {
        return const_cast<HashMap&>(*this).locate(key);
    }

    global_iterator bucketPosAt(const key_type& key)
    {
        const auto bucketIndex = bucket(key);
        Ensures(bucketIndex < bucket_count());
        const auto bucketPos = std::next(hashTable_.begin(), bucketIndex);
        Ensures(bucketPos != hashTable_.end());
        return bucketPos;
    }

    const_global_iterator bucketPosAt(const key_type& key) const
    {
        return const_cast<HashMap&>(*this).bucketPosAt(key);
    }

    HashTable hashTable_;
    global_iterator firstBucket_;
    float maxLoadFactor_;
    size_type size_;
    key_equal keyEqual_;
    hasher hasher_;
};

template<typename Key,
         typename T,
         typename Hash,
         typename KeyEqual>
class HashMap<Key, T, Hash, KeyEqual>::ConstIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename HashMap::value_type;
    using difference_type = typename HashMap::difference_type;
    using pointer = typename HashMap::const_pointer;
    using reference = typename HashMap::const_reference;

    ConstIterator() = default;

    ConstIterator(const_global_iterator bucketPos, const_local_iterator nodePos)
        :   bucketPos_(bucketPos)
        ,   nodePos_(nodePos)
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
        ++nodePos_;
        while(nodePos_ == bucketPos_->end())
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

private:
    const_global_iterator bucketPos_;
    const_local_iterator nodePos_;
};

template<typename Key,
         typename T,
         typename Hash,
         typename KeyEqual>
class HashMap<Key, T, Hash, KeyEqual>::Iterator
    :   public ConstIterator
{
public:
    using reference = typename HashMap::reference;
    using pointer = typename HashMap::pointer;

    Iterator(global_iterator bucketPos, local_iterator nodePos)
        :   ConstIterator(bucketPos, nodePos)
    {}

    reference operator*()
    {
        return const_cast<reference>(ConstIterator::operator*());
    }

    pointer operator->()
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
