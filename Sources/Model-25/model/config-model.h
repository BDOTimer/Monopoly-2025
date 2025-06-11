#ifndef CONFIG_MODEL_H
#define CONFIG_MODEL_H
///----------------------------------------------------------------------------|
/// "config-model.h"
/// Дефолтный дизайн для детей! (детский вариант)
///----------------------------------------------------------------------------:
#include "../debug.h"
#include "../user-model.h"
#include "implants/bot-iq.h"


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
    struct Cell;

    const char* const currencySymbol{
    "///---------------------------------------------------|\n"
    "/// https://ru.wikipedia.org/wiki/Список_знаков_валют |\n"
    "///     ₽,$,¥,€,£(эскудо),ƒ,₿,Ξ,Ł,ℕ,§,Au,฿,؋, ...     |\n"
    "///---------------------------------------------------|\n"
    };
    constexpr std::string_view $S { "£"  };
    constexpr std::string_view $s {" £"  };
    constexpr std::string_view $sn{" £\n"};


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
            events.clear ();
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
    struct Config : ConfigShare
    {
        bool loadFromFile(std::string_view filename)
        {   /// TODO ...
            return true;
        }

        ///------------------------------|
        /// Статы игрока на шаге.        |
        ///------------------------------:
        StateGame               stateGame;

        ///------------------------------|
        /// Статы от клиента.            |
        ///------------------------------:
        StateGame4Server      stateGame4S;

        ///------------------------------|
        /// Если 0, то сид от часов.     |
        ///------------------------------:
        unsigned isSeed{1234560};

        ///------------------------------|
        /// Денеги у игроков на старте.  |
        ///------------------------------:
        unsigned startMoney{300};

        ///------------------------------|
        /// Количество игроков.          |
        ///------------------------------:
        unsigned amountPlayers{3};

        Field* pfield{nullptr};

        ///------------------------------|
        /// Особые Правила.              |
        ///------------------------------:
        Rules rules;

        ///------------------------------|
        /// Управление событиями.        |
        ///------------------------------:
        ManagerEvents managerEvents;

        ///------------------------------|
        /// Разрешение клиенту.          |
        ///------------------------------:
        bool isActiveBackDoor{true};

        ///------------------------------|
        /// Условие победы.              |
        ///------------------------------:
        struct
        {   int        percentCapital{51};

            int             minus2x{-100};

            int        cellsOccupied1{10};
            int         cellsOccupied2{2};
        }victoryCondition;

        std::string_view getNamePlayer(unsigned id) const
        {   /// TODO: ASSERT(id < players.size())
            return players[id].name;
        }

        ///------------------------------|
        /// % выпадение Шанс на ячейке.  |
        ///------------------------------:
        unsigned percentDoChanse{100};

        implants::TuneIQs tuneIQs
        {   implants::TuneIQ{ "Умный"  , "01", "01"},
            implants::TuneIQ{ "Средний", "02", "02"},
            implants::TuneIQ{ "Дурак"  , "12", "12"}
        };

        implants::HolderTuneIQ _holderTuneIQ{tuneIQs};

        const implants::IBotIQ* getIBotIQ(unsigned id) const
        {   return _holderTuneIQ.get(id);
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
        std::string_view decodeStatus(unsigned i) const
        {   return decode2Str.getPlayer(i);
        }

        ///------------------------------|
        /// Проверка конфига на ошибки.  |
        ///------------------------------:
        bool doValidation() const
        {
            const_cast<Config*>(this)->_init();

            {   unsigned cnt{};
                for(    const auto& s : _worldGeometry[worldGeometryN1])
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
            std::stringstream ss ;
            ss  << "TIME Run: " << std::ctime(&end_time);
            return ss.str();
        }

        const Cell& getCell(const unsigned position) const;
              Cell& getCell(const unsigned position);

        void _init()
        {   stateGame.dat[StateGame::E_GAMEOVER] = -1;
            //worldGeometryN = rand()% _worldGeometry.size();
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
