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

#define SELECTANY __declspec(selectany)
