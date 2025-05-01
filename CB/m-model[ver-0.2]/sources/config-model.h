#ifndef CONFIG_MODEL_H
#define CONFIG_MODEL_H
///----------------------------------------------------------------------------|
/// "config-model.h"
/// Дефолтный дизайн для детей! (детский вариант)
///----------------------------------------------------------------------------:
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <functional>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <format>
#include <tuple>
#include <map>


namespace win
{
#ifndef     __MINGW32__
    #include <windows.h>
    #pragma execution_character_set( "utf-8" )
#endif  //  __MINGW32__
}


#define  l(v)          std::cout << #v << " = " << (v) << std::endl;
#define ln(v)          std::cout << #v << ":\n" << (v) << std::endl;
#define TESTCLASS(F)   std::cout << "RUN: "#F; std::cout << '\n';\
                  F(); std::cout << '\n';

#define TEST friend void ::tests(); static void test()


///----------------------------------|
/// Для удобства чтения?             |
/// Array1U это одномерный           |
/// массив из натуральных числе.     |
///----------------------------------:
using Array1U = std::vector<unsigned>;
using Array2U = std::vector<Array1U >;

void tests();

namespace model
{
    using  FooEvent = std::function<std::string()>;
    struct Referee;

    ///------------------------------------------------------------------------|
    /// Управляющий событиями.
    ///---------------------------------------------------------- ManagerEvents:
    struct  ManagerEvents
    {       ManagerEvents()
            {
            }

        ///----------------------------------|
        /// Событие случилось.               |
        ///----------------------------------:
        void push(unsigned i)
        {   events.push_back(eventsInstall[i]);
        }

        ///----------------------------------|
        /// Выполнить действие события.      |
        ///----------------------------------:
        void make()
        {   for(auto foo : events) foo();
            events.clear();
        }

        ///----------------------------------|
        /// Количество всех событий.         |
        ///----------------------------------:
        unsigned getSize() const
        {   return eventsInstall.size();
        }

        ///----------------------------------|
        /// Пусто?                           |
        ///----------------------------------:
        bool empty() const { return events.empty(); }

        ///----------------------------------|
        /// Все события игры.                |
        ///----------------------------------:
        ///----------------------------------|
        /// Событие Шанс случается на ячейки,|
        /// промаркированной cahnce > 0      |
        ///----------------------------------:
        ///----------------------------------|
        /// TODO: Всё свалено в кучу...      |
        ///----------------------------------:
        template<typename T>
        void initEvents(T* referee)
        {
            eventsInstall =
            {
                ///----------------------------------|
                /// #0: Тестовое событие.            |
                ///----------------------------------:
                [referee]()
                {
                    std::cout <<
                    "  ///------------------|\n"
                    "  /// Событие Шанс #0. |\n"
                    "  ///------------------|\n";

                    l(referee->cfg->moneyBank)

                    std::cout << '\n';

                    return "";
                },

                ///----------------------------------|
                /// #1: ...                          |
                ///----------------------------------:
                [referee]()
                {
                    std::cout <<
                    "  ///------------------|\n"
                    "  /// Событие Шанс #1. |\n"
                    "  ///------------------|\n";

                    return "";
                },

                ///----------------------------------|
                /// #2: ...                          |
                ///----------------------------------:
                [referee]()
                {
                    std::cout <<
                    "  ///------------------|\n"
                    "  /// Событие Шанс #2. |\n"
                    "  ///------------------|\n";

                    return "";
                }

                ///----------------------------------|
                /// #3: ...                          |
                ///----------------------------------:
                /// ...
            };
        }

    private:
        std::vector<FooEvent> events;
        std::vector<FooEvent> eventsInstall;
    };


    ///------------------------------------------------------------------------|
    /// Правила.
    ///------------------------------------------------------------------ Rules:
    struct Rules
    {
        std::vector<std::vector<std::vector<unsigned>>> prices
        {
            /// Ребенок:
            {   {   120,  1, 13, 22 },
                {   100, 16, 28,  7 },
                {    80,  7, 19, 25 },
                {    60, 10         }
            },

            /// Взрослый:
            {   {   120,  8, 17, 29 },
                {   100,  2, 11, 23 },
                {    80, 14, 26, 20 },
                {    60,  5         }
            },

            /// Родитель:
            {   {   120, 16, 18, 24 },
                {   100,  9, 21, 27 },
                {    80,  3, 12, 30 },
                {    60, 15         }
            }
        };
    };


    ///------------------------------------------------------------------------|
    /// Конфигурация Модели.
    ///----------------------------------------------------------------- Config:
    struct Field;
    struct Config
    {
        bool loadFromFile(std::string_view filename)
        {   /// TODO ...
            return true;
        }

        ///------------------------------|
        /// Количество ячеек на поле.    |
        ///------------------------------:
        unsigned amountCells      { 30 };
        unsigned amountSatusesCell{ 3  };

        std::vector<std::string> statuses
        { "123", "231", "312"
        };

        unsigned W{ 9 };
        unsigned H{ 8 };

        ///------------------------------|
        /// Ячейки с шансами.            |
        ///------------------------------:
        std::vector<unsigned> chances
        {   4, 7, 9, 12, 16, 19, 24, 26, 29
        };

        ///------------------------------|
        /// Денеги у игроков на старте.  |
        ///------------------------------:
        unsigned startMoney{300};

        ///------------------------------|
        /// Денеги Банка.                |
        ///------------------------------:
        unsigned moneyBank{3200};

        ///------------------------------|
        /// Количество игроков.          |
        ///------------------------------:
        unsigned amountPlayers{3};

        Field* pfield{nullptr};

        ///------------------------------|
        /// Скроллить консоль?.          |
        ///------------------------------:
        bool isScrollConsole{/*ДА:*/true};

        ///------------------------------|
        /// Особые Правила.              |
        ///------------------------------:
        Rules rules;

        ///------------------------------|
        /// Управление событиями.        |
        ///------------------------------:
        ManagerEvents managerEvents;

        const char* statusNames[4]
        {   "Ребёнок ",
            "Взрослый",
            "Родитель",
            "Чужой   "
        };

        ///------------------------------|
        /// Расшифровка статуса.         |
        ///------------------------------:
        std::string_view decodeStatus(unsigned status) const
        {   constexpr unsigned N = sizeof statusNames / sizeof *statusNames ;
            return statusNames[status >= N ? N - 1 : status];
        }

        ///------------------------------|
        /// Проверка конфига на ошибки.  |
        ///------------------------------:
        bool doValidation() const
        {
            if (W * 2 + (H - 2) * 2 != amountCells) return false;

            /// TODO ...

            return true;
        }

        ///------------------------------|
        /// Получить базовый конфиг.     |
        ///------------------------------:
        static const Config& getDefault()
        {   static   Config  config;
            return           config;
        }

        void infoValidation() const
        {   std::cout << "\nmodel::Config::doValidation() ---> "
                      << (doValidation() ? "SUCCESS!" : "FAIL ...") << "\n\n";
        }

        void info() const
        {
            infoValidation();
        }

        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {
            {   Config config;
                     l(config.getDefault().amountCells)
                       config.info();
            }
        }
    };

}

#endif // CONFIG_MODEL_H
