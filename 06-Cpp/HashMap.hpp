#ifndef EX3_HASHMAP_HPP
#define EX3_HASHMAP_HPP

#include <vector>
#include <string>
#include <exception>
#include <iostream>

#define CAP_I 16
#define SIZE_I 0
#define FACTOR 2
#define LOWER_I (1/4.0)
#define UPPER_I (3/4.0)
#define UPSIZE 1
#define DOWNSIZE -1

/**
 * hashmap class
 * @tparam KeyT
 * @tparam ValueT
 */
template<typename KeyT, typename ValueT>
class HashMap
{
private:
    size_t _size, _capacity;
    using pair = std::pair<KeyT, ValueT>;
    using bucket = std::vector<pair>;
    double _low_factor, _up_factor;
    bucket* _map;

    /**
     * resizing map
     * @param sign >0 upon upsize, <0 upon downsize
     */
    void _resize(int sign);

    /**
     *
     * @param k
     * @return bucket index (using std::hash)
     */
    size_t _getBucketIndex(const KeyT& k) const
    { return (std::hash<KeyT>{}(k) & (_capacity - 1)); }

    /**
     *
     * @param k
     * @return pair index of key
     */
    size_t _getPairIndex(const KeyT& k) const;

public:
    /**
     * default ctor
     */
    HashMap() : _size(SIZE_I), _capacity(CAP_I), _low_factor(LOWER_I), _up_factor(UPPER_I),
                _map(new bucket[_capacity]) {};

    /**
     * ctr1, gets upper & lower thresholds for hashmap size
     * @param upper
     * @param lower
     */
    HashMap(double upper, double lower);

    /**
     * ctor2, gets vectors of keys and values and keep them in map
     * @param keys
     * @param values
     */
    HashMap(const std::vector<KeyT>& keys, const std::vector<ValueT>& values);

    /**
     * copy ctor
     * @param other
     */
    HashMap(const HashMap& other) : _size(SIZE_I), _capacity(other.capacity()),
                                    _low_factor(other._low_factor), _up_factor(other._up_factor),
                                    _map(new bucket[_capacity])
    {
        //perform deep copy
        for (auto it = other.cbegin(); it != cend(); ++it)
        {
            insert((*it).first, (*it).second);
        }
    }

    /**
     * move ctor
     * @param other
     */
    HashMap(HashMap && other) noexcept : _size(other._size), _capacity(other.capacity()),
                                        _low_factor(other._low_factor), _up_factor(other._up_factor),
                                        _map(nullptr)
    { std::swap(_map, other._map); }

    /**
     * dtor
     */
    ~HashMap()
    { delete[] _map; };

    /**
     *
     * @return current size of map (num of values)
     */
    int size() const
    { return _size; }

    /**
     *
     * @return hashmap capacity
     */
    int capacity() const
    { return _capacity; }

    /**
     *
     * @return current load factor
     */
    double getLoadFactor() const
    { return (size() / capacity()); }

    /**
     *
     * @return true if map is empty, false otherwise
     */
    bool empty() const
    { return size() == 0; }

    /**
     *
     * @param k key of type KeyT
     * @param v value of type ValueT
     * @return true upon successful insertion to map, false otherwise
     */
    bool insert(const KeyT& k, const ValueT& v);

    /**
     *
     * @param k key of type KeyT
     * @return if key in map
     */
    bool containsKey(const KeyT& k) const;

    /**
     *
     * @param k key of type KeyT
     * @return value of given key, throws exception if key is not in map
     */
    ValueT& at(const KeyT& k) const;

    /**
     *
     * @param k key of type KeyT
     * @return if value of give key was successfully removed
     */
    bool erase(const KeyT& k);

    /**
     *
     * @param k key of type KeyT
     * @return size of bucket
     */
    int bucketSize(const KeyT& k) const;

    /**
     * removes all elements in map
     */
    void clear();

    /**
     * copy operator=
     * @param other
     * @return reference to hashmap
     */
    HashMap& operator=(const HashMap& other)
    {
        _size = SIZE_I;
        _capacity = other.capacity();
        _low_factor = other._low_factor;
        _up_factor = other._up_factor;
        _map = new bucket[_capacity];

        //perform deep copy
        for (auto it = other.cbegin(); it != cend(); ++it)
        {
            insert(it->first, it->second);
        }
    }

    /**
     * move operator=
     * @param other
     * @return
     */
    HashMap& operator=(HashMap && other) noexcept
    {
        _size = other._size;
        _capacity = other.capacity();
        _low_factor = other._low_factor;
        _up_factor = other._up_factor;
        _map(nullptr);
        std::swap(_map, other._map);
    }

    /**
     *
     * @param k key of type KeyT
     * @return value of given key in map
     */
    const ValueT& operator[](const KeyT& k) const noexcept;

    /**
     *
     * @param k key of type KeyT
     * @return assigning value of given key in map
     */
    ValueT& operator[](KeyT k) noexcept;

    /**
     *
     * @param other
     * @return true if maps are the same, false otherwise
     */
    bool operator==(const HashMap& other) const;

    /**
     *
     * @param other
     * @return true if maps are different, false otherwise
     */
    bool operator!=(const HashMap& other) const
    { return !(*this == other); }

    // ************** const_iterator ************** //
    /**
     * HashMap iterator
     */
    class const_iterator
    {
    private:
        bucket* _pBucket;
        int _capacity;
        int _cur_bucket;
        typename bucket::iterator _it;
    public:
        /**
         * default ctor
         */
        explicit const_iterator(bucket* p, int capacity, int bucket_index = 0) :
            _pBucket(p), _capacity(capacity), _cur_bucket(bucket_index), _it((*_pBucket).begin())
        {
            if (bucket_index == capacity)
            {
                _it = p[capacity - 1].end();
            }
            while (_cur_bucket < _capacity - 1 && _it == _pBucket[_cur_bucket].end())
            {
                ++_cur_bucket;
                _it = _pBucket[_cur_bucket].begin();
            }
        };

        /**
         * dereference operator for reading
         * @return value of obj
         */
        const std::pair<KeyT, ValueT>& operator*() const
        { return *_it; }

        /**
         * pre-increment
         * @return
         */
        const_iterator& operator++()
        {
            if (_cur_bucket == _capacity)
            {
                _it = _pBucket[_capacity - 1].end();
                return *this;
            }

            if (_it != _pBucket[_cur_bucket].end())
            {
                ++_it;
            }
            while (_cur_bucket < _capacity - 1 && _it == _pBucket[_cur_bucket].end())
            {
                ++_cur_bucket;
                _it = _pBucket[_cur_bucket].begin();
            }
            return *this;
        }

        /**
         * post-increment
         * @return
         */
        const_iterator& operator++(int)
        {
            auto result = *this;
            ++(*this);
            return result;
        }

        /**
         *
         * @param other
         * @return true if 2 iterators point to same obj, false otherwise
         */
        bool operator==(const const_iterator& other) const
        { return _it == other._it; }

        /**
         *
         * @param other
         * @return true if 2 iterators point to different obj, false otherwise
         */
        bool operator!=(const const_iterator& other) const
        { return !(*this == other); }
    };

    /**
    *
    * @return iterator to first pair in map
    */
    const_iterator begin() const
    { return const_iterator(_map, capacity()); }

    /**
     *
     * @return iterator that indicates end of objects in map
     */
    const_iterator end() const
    { return const_iterator(_map, capacity(), capacity()); }

    /**
     *
     * @return iterator to first pair in map
     */
    const_iterator cbegin() const
    { return const_iterator(_map, capacity()); }

    /**
     *
     * @return iterator that indicates end of objects in map
     */
    const_iterator cend() const
    { return const_iterator(_map, capacity(), capacity()); }
};

/**
 * ctor with upper and lower load factors
 * @tparam KeyT
 * @tparam ValueT
 * @param upper
 * @param lower
 */
template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap(double upper, double lower)
{
    if (upper < 0 || upper > 1 || lower < 0 || lower > 1 || upper < lower)
    {
        std::cerr << "exiting ctor due to illegal params\n";
        throw std::invalid_argument("exiting ctor due to illegal params\n");
    }
    HashMap(), _low_factor = lower, _up_factor = upper;
}

/**
 * inserts new pait to map
 * @tparam KeyT
 * @tparam ValueT
 * @param k
 * @param v
 * @return true upon success
 */
template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::insert(const KeyT& k, const ValueT& v)
{
    if (containsKey(k))
    {
        return false;
    }
    ++_size; _resize(UPSIZE);
    size_t index = _getBucketIndex(k);
    _map[index].push_back(std::make_pair(k, v));
    return true;
}

/**
 * erase a pair by key
 * @tparam KeyT
 * @tparam ValueT
 * @param k
 * @return true upon success
 */
template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::erase(const KeyT& k)
{
    if (!containsKey(k))
    {
        return false;
    }
    _map[_getBucketIndex(k)].erase(_map[_getBucketIndex(k)].cbegin() + _getPairIndex(k));
    --_size; _resize(DOWNSIZE);
    return true;
}

/**
 * resizing map
 * @tparam KeyT
 * @tparam ValueT
 * @param sign indicates if upsize/downsize
 */
template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::_resize(int sign)
{
    if ((sign == UPSIZE && getLoadFactor() > _up_factor) ||
        (sign == DOWNSIZE && getLoadFactor() < _low_factor))
    {
        //hold all obj to be inserted
        std::vector<pair> all;
        all.reserve(_size);
        for (auto it = cbegin(); it != cend(); ++it)
        {
            all.push_back(*it);
        }

        //create new map
        _capacity = (sign == UPSIZE) ? (_capacity * FACTOR) : (_capacity / FACTOR);
        delete[] _map;
        _map = new bucket[_capacity];
        _size = 0;
        for (auto it = all.cbegin(); it != all.cend(); ++it)
        {
            insert(it->first, it->second);
        }
    }
}

/**
 *
 * @tparam KeyT
 * @tparam ValueT
 * @param k
 * @return true if key in map, false otherwise
 */
template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::containsKey(const KeyT& k) const
{
    if (empty())
    {
        return false;
    }

    int hash = _getBucketIndex(k);
    bucket b = _map[hash];
    for (int i = 0; i < (int) b.size(); ++i)
    {
        if (b[i].first == k)
        {
            return true;
        }
    }
    return false;
}

/**
 * ctor with given keys anf values
 * @tparam KeyT
 * @tparam ValueT
 * @param keys
 * @param values
 */
template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap(const std::vector<KeyT>& keys, const std::vector<ValueT>& values)
{
    try
    {
        if (keys.size() != values.size())
        {
            throw std::invalid_argument("exiting ctor due to illegal params\n");
        }
        HashMap();
        for (int i = 0; i < keys.size(); ++i)
        {
            if (!insert(keys[i], values[i]))
            {
                int b = _getBucketIndex(keys[i]);
                int p = _getPairIndex(keys[i]);
                _map[b][p] = std::pair<KeyT, ValueT>(keys[i], values[i]);
            }
        }
    }
    catch (std::invalid_argument& e)
    {
        std::cerr << "exiting ctor due to illegal params\n";
        throw;
    }
}

/**
 *
 * @tparam KeyT
 * @tparam ValueT
 * @param k
 * @return size of given key's bucket
 */
template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucketSize(const KeyT& k) const
{
    if (!containsKey(k))
    {
        std::cerr << "exiting bucketsize() due to exception\n";
        throw std::out_of_range("exiting bucketsize() due to exception\n");
    }
    return _map[_getBucketIndex(k)].size();
}

/**
 * operator [] for reading
 * @tparam KeyT
 * @tparam ValueT
 * @param k
 * @return
 */
template<typename KeyT, typename ValueT>
const ValueT& HashMap<KeyT, ValueT>::operator[](const KeyT& k) const noexcept
{
    ValueT undefined;
    if (!containsKey(k)) return undefined; //undefined
    return _map[_getBucketIndex(k)][_getPairIndex(k)].second;
}

/**
 * operator [] for writing
 * @tparam KeyT
 * @tparam ValueT
 * @param k
 * @return
 */
template<typename KeyT, typename ValueT>
ValueT& HashMap<KeyT, ValueT>::operator[](KeyT k) noexcept
{
    if (!containsKey(k))
    {
        insert(k, ValueT());
    }

    return _map[_getBucketIndex(k)][_getPairIndex(k)].second;
}


template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::clear()
{
    for (int i = 0; i < capacity(); ++i)
    {
        _map[i].clear();
    }
    _size = 0;
}

template<typename KeyT, typename ValueT>
ValueT& HashMap<KeyT, ValueT>::at(const KeyT& k) const
{
    int hash = _getBucketIndex(k);
    bucket b = _map[hash];
    for (int i = 0; i < b.size(); ++i)
    {
        if (b[i].first == k)
        {
            return b[i].second;
        }
    }
    std::cerr << "exiting at() due to exception\n";
    throw std::out_of_range("exiting at() due to exception\n");
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::operator==(const HashMap& other) const
{
    if (size() != other.size() || capacity() != other.capacity() ||
        _low_factor != other._low_factor || _up_factor != other._up_factor)
    {
        return false;
    }

    for (auto it = cbegin(); it != cend(); ++it)
    {
        KeyT key = (*it).first;
        if (!containsKey(key) || (at(key) != other.at(key)))
        {
            return false;
        }
    }
    return true;
}

template<typename KeyT, typename ValueT>
size_t HashMap<KeyT, ValueT>::_getPairIndex(const KeyT& k) const
{
    bucket b = _map[_getBucketIndex(k)];
    for (int i = 0; i < b.size(); ++i)
    {
        if (b[i].first == k)
        {
            return i;
        }
    }
    throw std::out_of_range("exiting inner func _getPairIndex due to exception\n");
}

#endif //EX3_HASHMAP_HPP
