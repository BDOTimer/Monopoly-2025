#ifndef CONFIG_MODEL_H
#define CONFIG_MODEL_H
const char* const LOGO = "Model::Monopoly-2025[ver::0.2.1]";
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
#include <array>
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
        {   return unsigned(eventsInstall.size());
        }

        ///----------------------------------|
        /// Пусто?                           |
        ///----------------------------------:
        bool empty() const { return events.empty(); }


        ///----------------------------------|
        /// Рандомный выбор события.         |
        ///----------------------------------:
        unsigned getNRnd() const
        {   unsigned r = rand() % 100;
            if(r < 30) return 0;
            if(r < 80) return 1;
            return 2;
        }

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
                    "  ///--------------------|\n"
                    "  /// Событие Шанс +50$  |\n"
                    "  ///--------------------|\n";
                    referee->persNow->money += 50;

                    std::cout << '\n';

                    return "нашли на дороге";
                },

                ///----------------------------------|
                /// #1: ...                          |
                ///----------------------------------:
                [referee]()
                {
                    std::cout <<
                    "  ///--------------------|\n"
                    "  /// Событие Шанс -30$  |\n"
                    "  ///--------------------|\n";
                    referee->persNow->money -= 30;

                    return "ремонт компа";
                },

                ///----------------------------------|
                /// #2: ...                          |
                ///----------------------------------:
                [referee]()
                {
                    std::cout <<
                    "  ///--------------------|\n"
                    "  /// Событие Шанс +200$ |\n"
                    "  ///--------------------|\n";
                    referee->persNow->money += 200;

                    return "выгрыш в казино";
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
        /// ???
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

        std::vector<std::string> worldGeometry
        {   "OOOOOOOOO",
            "O.......O",
            "O.......O",
            "O.......O",
            "O.......O",
            "O.......O",
            "O.......O",
            "OOOOOOOOO"
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

        ///------------------------------|
        /// Профиль игрока.              |
        ///------------------------------:
        struct Players
        {   bool       isBot;
            std::string name;
        };

        ///------------------------------|
        /// Состав игроков.              |
        ///------------------------------:
        std::vector<Players> players
        {   {true , "bot::gudleifr"},
            {true , "bot::aliskda" },
            {true , "bot::Noname"  }
        //  {false, "Вася Пупкин"  }
        };

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
            {   unsigned cnt{};
                for(    const auto& s : worldGeometry)
                {   for(const auto  c : s)
                    {   cnt += c == 'O' ? 1 : 0;
                    }
                }
                if (cnt != amountCells) return false;
            }


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
