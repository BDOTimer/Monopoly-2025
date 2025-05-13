#ifndef CONFIG_MODEL_H
#define CONFIG_MODEL_H
const char* const LOGO = "Model::Monopoly-2025[ver::0.2.1]";
///----------------------------------------------------------------------------|
/// "config-model.h"
/// Дефолтный дизайн для детей! (детский вариант)
///----------------------------------------------------------------------------:
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <functional>
#include <algorithm>
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

#include "../debug.h"

namespace win
{
#ifndef     __MINGW32__
    #include <windows.h>
    #pragma execution_character_set( "utf-8" )
#endif  //  __MINGW32__
}


#ifndef l
	#define  l(v)     std::cout << #v << " = " << (v) << std::endl;
	#define ln(v)     std::cout << #v << ":\n" << (v) << std::endl;
#endif


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

    ///------------------------------------------------------------------------|
    /// Умственные способности бота описываются здесь.
    ///------------------------------------------------------------------------|
    constexpr auto NPOS{std::string::npos};

    struct BotProfileIQ
    {   ///----------------------------------|
        /// Запрет на продажу                |
        /// от статуса(0,1,2) товара.        |
        ///----------------------------------:
        std::string botSell;

        ///----------------------------------|
        /// Запрет на покупку от ранга(0,1,2)|
        /// товара. Чем выше ранг, тем       |
        /// выгоднее сделка.                 |
        ///----------------------------------:
        std::string botBuy;

        bool canSellBot(unsigned priceRang) const
        {   return botSell.find('0' + priceRang) == NPOS;
        }

        bool canBuyBot(unsigned priceRang) const
        {   return botBuy.find('0' + priceRang) == NPOS;
        }
    };


    struct MessagesFooEvent
    {   std::string_view messEventWhat;
        std::string_view messEventAdd;
    };


    using  FooEvent = std::function<MessagesFooEvent()>;
    struct Referee;

    /// XXX
    ///------------------------------------------------------------------------|
    /// Управляющий событиями.
    ///---------------------------------------------------------- ManagerEvents:
    /// НЕ ИСПОЛЬЗУЕТСЯ!
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
        [[nodiscard]]
        const std::string make()
        {   std::stringstream ss;
            for(auto foo : events)
            {   const auto&[mess1, mess2] = foo();
                ss << mess1;
                ss << mess2 << "\n\n";
            }
            events.clear();
            return ss.str();
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
        /// Cобытия Шанс(3 карточки).        |
        ///----------------------------------:
        ///----------------------------------|
        /// Событие Шанс случается на ячейке,|
        /// промаркированной cahnce > 0      |
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
                    referee->persNow->money += 50;

                    return MessagesFooEvent
                    {   "  ///--------------------|\n"
                        "  /// Событие Шанс +50$  |\n"
                        "  ///--------------------|\n",
                        "Подарок от жены!"
                    };
                },


                ///----------------------------------|
                /// #1: ...                          |
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
        /// ... ?
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
        /// Индекс игры.                 |
        ///------------------------------:
        unsigned  idGame{0};

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
        /// Если 0, то сид от часов.     |
        ///------------------------------:
        unsigned isSeed{12345};

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
        {   {true , "bot::aliskda" }, /// Срединий
            {true , "bot::Noname"  }, /// Умный
            {true , "bot::gudleifr"}, /// Дурак
        //  {false, "Slava-rusi11" }
        //  {false, "Вася Пупкин"  }
        };

        std::string_view getNamePlayer(unsigned id) const
        {   /// TODO: ASSERT(id < players.size())
            return players[id].name;
        }

        const char* statusNames[4]
        {   "Ребёнок ",
            "Взрослый",
            "Родитель",
            "Чужой   "
        };

        ///------------------------------|
        /// % выпадение Шанс на ячейке.  |
        ///------------------------------:
        unsigned percentDoChanse{100};

        ///------------------------------|
        /// Интеллект ботов.             |
        ///------------------------------:
        enum eSMARTNESS_BOT
        {    ORDINARY,
             SMART,
             FOOL
        };

        std::array<BotProfileIQ, 3> botsIQ
        {   BotProfileIQ{"02", "02"}, /// Срединий
            BotProfileIQ{"01", "01"}, /// Умный
            BotProfileIQ{"12", "12"}  /// Дурак
        };

        const BotProfileIQ* getBotIQ(unsigned id) const
        {   /// TODO: ASSERT(id < botsIQ.size())
            return &botsIQ[id];
        }

        ///-----------------------------------------------.

        ///-----------------------------------------------|
        /// Дебажные нстройки.                            |
        ///-----------------------------------------------:
        ///------------------------------|
        /// Кол-во итераций в файл.      |
        /// 0 - вывод в консоль.         |
        ///------------------------------:
        static unsigned& isDump2File()
        {   static unsigned CountCircles{0};
            return CountCircles;
        }
        ///-----------------------------------------------.

        static std::string getLogo()
        {   std::stringstream ss;
            ss  << "/// " << "Старт " << LOGO  << "\n"
                << "/// " << Config::getTimeNow()
                << "/// Compilator::" << win::COMPILATOR_NAME << "\n\n";
            return ss.str();
        }

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
                ASSERTM(cnt == amountCells, "Число ячеек != геометрии!")
            }

			ASSERTM(3 == players.size(), "Поддерживается только 3 игрока!")

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

        [[nodiscard]]
        const std::string infoValidation() const
        {   std::stringstream ss;
            ss << "\nmodel::Config::doValidation() ---> "
               << (doValidation() ? "SUCCESS!" : "FAIL ...") << "\n\n";
            return ss.str();
        }

        std::string info() const
        {   std::stringstream ss;
            ss  << infoValidation().c_str()
                << "amountCells: " << amountCells << '\n'
                << "isSeed     : " << isSeed      << '\n'
                << "startMoney : " << startMoney  << '\n'
                << "moneyBank  : " << moneyBank   << '\n';
            return ss.str();
        }

        static std::string getTimeNow()
        {   auto end = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            std::stringstream ss;
            ss  << "TIME Run: " << std::ctime(&end_time);
            return ss.str();
        }

        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Config config1;
                ln(config1.info())

            Config config2;
                   config2 = config1;
                ln(config2.info())
        }
    };

}

#endif // CONFIG_MODEL_H
