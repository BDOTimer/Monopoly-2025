#ifndef DEBUG_H
#define DEBUG_H
/// "debug.h"
///-----------------------------------------------------------------------------
/// ...
///----------------------------------------------------------------------------:
///--------------------------------------------------|
/// Прописано в IDE:                                 |
/// #define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS  |
/// #define _CRT_SECURE_NO_WARNINGS                  |
///--------------------------------------------------|
#include <functional>
#include <filesystem> /// C++17
#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <format>  /// C++20
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <array>
#include <tuple>
#include <ctime>
#include <list>
#include <map>
#include <set>


#if __has_include(<SFML/Graphics.hpp>)
  #include <SFML/Graphics.hpp>
#endif

namespace win
{
#ifndef     __MINGW32__
    #include <windows.h>
    #pragma execution_character_set  ( "utf-8"   )
	const char* const COMPILATOR_NAME{ "VS 2022" };
#else
	const char* const COMPILATOR_NAME{"GCC MINGW"};
#endif  //  __MINGW32__
}

namespace fs = std::filesystem;
void tests();

template<typename T> using Arr1d =       std::vector<T> ;
template<typename T> using Mat2d = Arr1d<std::vector<T>>;
template<typename T> using Mat3d = Mat2d<std::vector<T>>;

#define ln(a) std::cout << #a << ":\n" << (a) << '\n';
#define  l(a) std::cout << #a << ": "  << (a) << '\n';
#define TEST friend void ::tests(); static void test()
#define STOP std::cout << "STOP\n"; std::cin.get();
#define SIGNAL(a) std::cout << "\n...---SIGNAL---" << a << "...\n" << std::endl;

using Strv = std::string_view;

///----------------------------------------------------------------------------|
/// Начинка для ASSERT.
///------------------------------------------------------------------------ Ass:
constexpr char ERR1  []{ "ASSERT_ERROR--->FILE: \"{}\", LINE: {} - {}\n" };
constexpr char WARNING[]{ "WARNING--->FILE: \"{}\", LINE: {} - {}\n" };

struct  Ass
{
    static void error(bool pred, Strv filename, int line, Strv str = "...")
    {   if(!pred)
        {   std::cout << std::format(ERR1, cutStr(filename), line, str);
            throw(-1);
        }
    }

    static void warn(bool b, Strv filename, int line, Strv str)
    {   if(b)
        {   std::cout << std::format(WARNING, cutStr(filename), line, str);
        }
    }

    static Strv cutStr(Strv s)
    {   auto p = s.rfind("sources"); return s.substr(p, s.size() - p);
    }
};

#define  ASSERT(a)       Ass::error(a, __FILE__, __LINE__);
#define ASSERTM(a, mess) Ass::error(a, __FILE__, __LINE__, mess);
#define WARNING(a, mess) Ass::warn (a, __FILE__, __LINE__, mess);
#define TRY(a) try{a;}catch(...){std::cout << "ERROR exeption: " << #a << '\n';}

#define INFOSTART std::cout << "START "    << __METHOD_NAME__  << ":\n\n"
#define INFOEND   std::cout << "FINISHED " << __METHOD_NAME__  << ":\n\n"

inline std::string methodName(const std::string& prettyFunction)
{
    size_t colons = prettyFunction.find("::");
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = prettyFunction.rfind("(") - begin;
    return prettyFunction.substr(begin,end) + "()";
}

#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)

#endif // DEBUG_H
