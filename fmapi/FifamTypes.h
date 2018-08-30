#pragma once

#include <string>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_set>
#include <functional>
#include <filesystem>
#include <new>

using namespace std::experimental::filesystem;

using Byte = unsigned char;
using Char = char;
using WideChar = wchar_t;
using UChar = unsigned char;
using Short = short;
using UShort = unsigned short;
using Int = int;
using UInt = unsigned int;
using Int64 = long long int;
using UInt64 = unsigned long long int;
using Long = long;
using Float = float;
using Double = double;
using Bool = unsigned char;
using Bool1 = bool;
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
template<typename KeyType, typename ValueType>
using MultiMap = std::multimap<KeyType, ValueType>;
template<typename T>
using Set = std::set<T>;
template<typename T>
using UnorderedSet = std::unordered_set<T>;
template<typename T>
using MultiSet = std::multiset<T>;
using Path = path;

#define SELECTANY __declspec(selectany)
