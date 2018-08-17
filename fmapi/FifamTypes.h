#pragma once

#include <string>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <functional>
#include <filesystem>
#include <new>

using namespace std::experimental::filesystem;

using Byte = unsigned char;
using Char = char;
using UChar = unsigned char;
using Short = short;
using UShort = unsigned short;
using Int = int;
using UInt = unsigned int;
using Int64 = long long int;
using UInt64 = unsigned long long int;
using Float = float;
using Double = double;
using Bool = unsigned char;
using String = std::wstring;
template<typename A, typename B>
using Pair = std::pair<A, B>;
template<typename T, size_t S>
using Array = std::array<T, S>;
template<typename T>
using Vector = std::vector<T>;
template<typename T>
using List = std::list<T>;
template<typename KeyType, typename ValueType>
using Map = std::map<KeyType, ValueType>;
template<typename T>
using Set = std::set<T>;
using Path = path;

template<typename _Type, size_t _Capacity, _Type _NoneValue = _Type()>
class LimitedUnorderedSet {
    Vector<_Type> _vec;
    inline static _Type _unused_one = _NoneValue;
public:
    size_t size() { return _vec.size(); }
    size_t capacity() { return _Capacity; }
    void clear() { _vec.clear(); }
    bool empty() { return size() == 0; }
    bool contains(_Type const &value) { return std::find(_vec.begin(), _vec.end(), value) != _vec.end(); }

    bool insert(_Type const &value, int index = -1) {
        if (_Capacity == 0 || value == _NoneValue || contains(value) || size() == _Capacity)
            return false;
        if (index == -1)
            _vec.push_back(value);
        else {
            if (index >= _Capacity)
                return false;
            if (index >= size())
                _vec.push_back(value);
            else
                _vec[index] = value;
        }
        return true;
    }

    bool put_at(_Type const &value, int index = -1) {

    }

    bool erase(size_t index) {
        if (index < size()) {
            _vec.erase(_vec.begin() + index);
            return true;
        }
        return false;
    }

    void remove(_Type const &value) {
        for (size_t i = 0; i < _size; i++) {
            if (_vec[i] == value) {
                erase(i);
                break;
            }
        }
    }

    _Type at(size_t index) {
        if (index < size())
            return _vec[index];
        return _unused_one;
    }
};

#define SELECTANY __declspec(selectany)
