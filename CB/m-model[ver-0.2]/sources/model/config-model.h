#ifndef CONFIG_MODEL_H
#define CONFIG_MODEL_H
const char* const LOGO = "Model::Monopoly-2025[ver::0.2.2]";
///----------------------------------------------------------------------------|
/// "config-model.h"
/// Дефолтный дизайн для детей! (детский вариант)
///----------------------------------------------------------------------------:
#include "../debug.h"
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
    const std::string_view $S{ "£"};
    const std::string_view $s{" £"};


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
        unsigned idGame             { 0 };

        ///------------------------------|
        /// Количество ячеек на поле.    |
        ///------------------------------:
        unsigned amountCells       { 30 };

        ///------------------------------|
        /// Мировая геометрия.           |
        ///------------------------------:
        std::vector<std::vector<int>> worldGeometry
        {   {  0,  1,  2,  3,  4,  5,  6,  7,  8 },
            { 29, -1, -1, -1, -1, -1, -1, -1,  9 },
            { 28, -1, -1, -1, -1, -1, -1, -1, 10 },
            { 27, -1, -1, -1, -1, -1, -1, -1, 11 },
            { 26, -1, -1, -1, -1, -1, -1, -1, 12 },
            { 25, -1, -1, -1, -1, -1, -1, -1, 13 },
            { 24, -1, -1, -1, -1, -1, -1, -1, 14 },
            { 23, 22, 21, 20, 19, 18, 17, 16, 15 }
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

        implants::TuneIQs tuneIQs
        {   implants::TuneIQ{ "Срединий", "02", "02"},
            implants::TuneIQ{ "Умный"   , "01", "01"},
            implants::TuneIQ{ "Дурак"   , "12", "12"}
        };

        implants::HolderTuneIQ _holderTuneIQ{tuneIQs};

        const implants::IBotIQ* getIBotIQ(unsigned id) const
        {   return _holderTuneIQ.get(id);
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
        {   ASSERT(status < 3)
        /// constexpr unsigned N = sizeof statusNames / sizeof *statusNames ;
        /// return statusNames[status >= N ? N - 1 : status];
            return statusNames[status];
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
            std::stringstream ss ;
            ss  << "TIME Run: " << std::ctime(&end_time);
            return ss.str();
        }

        const Cell& getCell(const unsigned position) const;
              Cell& getCell(const unsigned position);


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
