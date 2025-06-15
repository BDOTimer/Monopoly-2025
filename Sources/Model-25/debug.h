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
#include <windows.h>
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
#include <thread>
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

#ifdef _MSC_VER
    #include <windows.h>
    #pragma execution_character_set  ( "utf-8"   )
    const char* const COMPILATOR_NAME{ "VS 2022" };
    inline void noShowCursor(){ ShowCursor(FALSE); }
#elif __MINGW32__
    #include <windows.h>
    const char* const COMPILATOR_NAME{"GCC MINGW"};
    inline void noShowCursor(){ ShowCursor(FALSE); }
#else
    const char* const COMPILATOR_NAME{"???"};
    inline void noShowCursor(){ }
#endif  //  __MINGW32__
}

namespace fs = std::filesystem;
void tests();

template<typename T> using Arr1d =       std::vector<T> ;
template<typename T> using Mat2d = Arr1d<std::vector<T>>;
template<typename T> using Mat3d = Mat2d<std::vector<T>>;

struct Log2File
{
    static std::ofstream& get()
    {   static std::ofstream f{"log.txt"};
        return f;
    }
};

#define COUT   std::cout
#define COUTx  Log2File::get()

#define ln(a) COUT << #a << ":\n" << (a) << std::endl;
#define  l(a) COUT << #a << ": "  << (a) << std::endl;
#define TEST friend void ::tests(); static void test()
#define STOP std::cout << "STOP\n"; std::cin.get();
#define SIGNAL(a) std::cout << "\n...---SIGNAL---" << a << "...\n" << std::endl;
#define SIGNAP    std::cout << "\n...---SIGNAP---...\n" << std::endl;\
                  std::cin.get();


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
    {   auto p = s.rfind("Sources"); return s.substr(p, s.size() - p);
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

constexpr auto NPOS{std::string::npos};

/*
template<typename T, unsigned N>
inline std::ostream& operator<<(std::ostream& o, const std::array<T,N>& m)
{   for(const auto n : m) o << n << " ";
    std::cout << '\n';
    return o;
}
*/

struct  Test999
{       Test999(unsigned n) : n(n) { l(n) }
        Test999(          )        { l(n) }
private:        unsigned n{ 888 };
};


template<typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& m)
{   for(const auto& e : m) o << e << ' ';
    return o;
}

template<typename T>
void show(const T& m)
{   for(auto&  e : m) std::cout << e << ' ';
    std::cout << "\n\n";
}

#endif // DEBUG_H
