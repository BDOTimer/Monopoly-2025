///----------------------------------------------------------------------------|
/// Модель игры Монополия-2025.
    const char* const LOGO = "Model::Monopoly-2025[ver::0.0.2]";
///----------------------------------------------------------------------------:
#include "config-model.h"

#include <locale>
#include <codecvt>


namespace win
{
#ifndef     __MINGW32__
    #include <windows.h>
    #pragma execution_character_set( "utf-8" )
#endif  //  __MINGW32__
}

std::string WstrToUtf8(const std::wstring& str)
{   std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.to_bytes(str);
}


std::wstring utf8ToWstr(const std::string& str)
{   std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.from_bytes(str);
}

namespace model
{
    ///------------------------------------------------------------------------|
    /// Карточка.
    ///------------------------------------------------------------------- Card:
    struct  Card
    {
        std::string_view type; /// Тип.
        std::string_view name; /// Название.
        std::string_view txtr; /// Texture.

        std::string infoName() const
        {   return std::format("\"{}\"", name.data());
        }
    };

    ///------------------------------------------------------------------------|
    /// Описание(класс) ячейки на карте.
    ///------------------------------------------------------------------- Cell:
    struct  Cell
    {       Cell() {}
            Cell(const Array1U& init) : statuses(init)
            {
            }

        ///------------------------------|
        /// Получить статус по номеру.   |
        ///------------------------------:
        unsigned getStatus(unsigned number) const { return statuses[number]; }

        void info() const
        {
            std::cout << "id::" << id << "\t";
            for (const auto n : statuses) std::cout << n;
            std::cout << '\n';
        }

    private:
        ///------------------------------|
        /// Массив со статусами.         |
        ///------------------------------:
        Array1U statuses;

        ///------------------------------|
        /// Ячейка может иметь ID.       |
        ///------------------------------:
        unsigned id;

        ///------------------------------|
        /// Шанс.                        |
        ///------------------------------:
        unsigned chance {0};

        Card* card{nullptr};

        ///------------------------------|
        /// Тут тест этого класса.       |
        ///------------------------------:
        TEST
        {
            std::cout << "statusesCells: \n";

            {   ///----------------------|
                /// Создаем объект ячейки|
                ///----------------------:
                Cell         cell({1,2,3});

                ///----------------------|
                /// Дебажим в консоль.   |
                ///----------------------:
                std::cout << cell << std::endl;
            }

            {   ///----------------------|
                /// Данные для ячейки    |
                ///----------------------:
                Array1U           m{2,3, 1};

                ///----------------------|
                /// Создаем объект ячейки|
                ///----------------------:
                Cell         cell(m);

                ///----------------------|
                /// Дебажим в консоль.   |
                ///----------------------:
                std::cout << cell << std::endl;
            }
        }

        ///------------------------------|
        /// Друзья приватных методов.    |
        ///------------------------------:
        friend std::ostream& operator<<(std::ostream& o, const Cell& cell);
        friend struct Field;
        friend struct Person;
        friend struct Referee;
    };


    ///----------------------------------|
    /// Вывод Cell в консоль.            |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const Cell& cell)
    {   for (const auto status : cell.statuses) o << status;
        return o;
    }


    ///------------------------------------------------------------------------|
    /// Поле Модели.
    ///------------------------------------------------------------------ Field:
    struct  Field : std::vector<Cell>
    {       Field(const Config& cfg)
            {   init(cfg);
            }

        struct PersonQ { unsigned pos; bool isStart{ false }; };

        PersonQ add(unsigned pos, unsigned step) const
        {
            size_t res = pos + step;
            if (res >= size()) { return { unsigned(res -= size()), true }; }
            return  { (unsigned)res };
        }

        unsigned sub(unsigned pos, unsigned step) const
        {
            size_t res = pos - step;
            if (res >= size()) res += size();
            return (unsigned)res;
        }

        void info() const
        {
            l(size())

            for (const auto& cell : *this)
            {   cell.info();
            }
        }

    private:
        void init(const Config& cfg)
        {
            const Array2U statuses = cfg.getGen();
            reserve(cfg.amountCells);

            for (unsigned i = 0; i < cfg.amountCells; ++i)
            {
                emplace_back(Cell());

                Cell& cell = back();
                cell.id = i;
                cell.statuses.reserve(cfg.amountSatusesCell);

                for (unsigned j = 0; j < cfg.amountSatusesCell; ++j)
                {
                    cell.statuses.push_back(statuses[i][j]);
                }
            }

            ///-------------------------------|
            /// Установка шансов.             |
            ///-------------------------------:
            for(auto& i : cfg.chances) (*this)[i].chance = 1;
        }

        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   {   Field   field(Config::getDefault());
                        field.info();

                Cell& cell = field[0];
                        cell.info();
            }
        }
    };

    ///------------------------------------------------------------------------|
    /// Персонаж.
    ///----------------------------------------------------------------- Person:
    struct  Person
    {
        ///------------------------------|
        /// Имя персонажа.               |
        ///------------------------------:
        std::string name{ "Noname" };

        ///------------------------------|
        /// Позиция на поле.             |
        /// Поле у нас лента...          |
        ///------------------------------:
        unsigned position{ 0 };
        unsigned status  { 0 };
        unsigned circle  { 1 };

        ///------------------------------|
        /// Кошелёк.                     |
        ///------------------------------:
        unsigned money   { 0 };

        ///------------------------------|
        /// Список собственности.        |
        ///------------------------------:
        std::map<std::string, unsigned>  cargo;

        ///------------------------------|
        /// Награды.                     |
        ///------------------------------:
        std::map<std::string, unsigned>  medals;

        ///------------------------------|
        /// Особые условия.              |
        ///------------------------------:
        std::map<std::string, unsigned>  specs;

        const Config* cfg{nullptr};

        void info() const
        {
            unsigned chance = (*(cfg->pfield))[position].chance;
            Card*    card   = (*(cfg->pfield))[position].card;

            auto n = 15 - nn + name.size();
            std::cout << ">>"         << std::setw(n) << name       << ", "
                      << "Позиция = " << std::setw(4) << position   << ", "
                      << "Статус = "  << std::setw(2)
                      <<  cfg->decodeStatus(status)                 << ", "
                      << "Круг = "    << std::setw(3) << circle     << ", "
                      << "Шанс = "    << std::setw(2) << chance     << ", "
                      << card->infoName()
                      << '\n';
        }

        ///------------------------------|
        /// Для utf8 в std::setw.        |
        ///------------------------------:
        unsigned nn;
        void   init()
        {   const auto     s = utf8ToWstr(name);
            nn = (unsigned)s.size();
        }

    private:
        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Person  person;
                    person.info();
        }
    };


    ///------------------------------------------------------------------------|
    /// Арбитер.
    ///---------------------------------------------------------------- Referee:
    struct  Referee
    {       Referee(const Config& Cfg) : field(Cfg), config(Cfg)
            {
                for (auto& pers : perses)
                {   pers.init();

                    pers.cfg = &Cfg;
                }

                model::Config* cfg
                    = const_cast<model::Config*>(&Cfg);

                cfg->pfield = &field;

                getCards   ();
                cards2Field();

                if(config.isMixerCards) mixerCards();
            }

    protected:
        void info() const
        {   for (const auto& pers : perses)
            {   pers.info();
            }
        }

        std::vector<model::Person> perses
        {   {"Bot:Pete"   },
            {"Bot:Ann"    },
            {"Вася Пупкин"}
        };

        model::Field  field;

        const Config& config;

        std::vector<model::Card >  cards;
        std::vector<model::Card*> pcards;

        ///------------------------------|
        /// Получить карточки из конфига.|
        ///------------------------------:
        void getCards()
        {
            cards.reserve(config.amountCells);

            for(const auto& type : config.cardNames)
            {
                std::string_view t = type.front();

                for(size_t i = 2; i < type.size(); i += 2)
                {   cards.emplace_back( Card{t, type[i], });
                    pcards.push_back  (&cards.back());
                }
            }
        }

        void cards2Field()
        {   for(size_t i = 0; i < field.size(); ++i)
            {   field [i].card = pcards[i];
            }
        }

        void infoCards() const
        {   for(const auto pc : pcards)
            {   std::cout << pc->type << " : " << pc->name << '\n';
            }   std::cout << '\n';
        }

        void mixerCards()
        {   for(size_t i = 0; i < pcards.size(); ++i)
            {   const unsigned   randNumber = rand() % pcards.size();
                std::swap(pcards[randNumber], pcards[i]);
            }
        }

        bool  step()
        {
            for (auto& pers : perses)
            {
                const unsigned randNumber = rand() % 6 + 1;
                             l(randNumber)

                const auto& [pos, isStart]
                    = field.add(pers.position, randNumber);

                if (isStart)
                {
                    if (++pers.status == 3) pers.status = 0;

                    ++pers.circle;
                }

                pers.position = pos;

                pers.info();

                std::cout << std::endl;
            }
            return true;
        }

        friend struct TestGame;
    };

}   /// namespace model


///----------------------------------------------------------------------------|
/// Тестовая игровая площадка.
///------------------------------------------------------------------- TestGame:
struct  TestGame : model::Referee
{       TestGame() : model::Referee(model::Config::getDefault())
        {   model::Config::getDefault().infoValidation();

            infoCards();
        }

    void run()
    {
        info();

        unsigned cnt{ 0 };

        for (bool isDone = true; isDone;)
        {
            std::cout   << "\nПАУЗА::Нажмите ENTER, чтобы сделать "
                        << ++cnt << " шаг ... \n";

            std::cin.get();

            std::system("cls"); std::cout << "Процесс " << LOGO << "\n\n";

            isDone = step();
        }
    }

    ///------------------------------|
    /// Тест класса.                 |
    ///------------------------------:
    TEST
    {   TestGame    testGame;
                    testGame.run();
    }
};


///----------------------------------------------------------------------------|
/// Все тесты классов.
///---------------------------------------------------------------------- tests:
void tests()
{
    /// TESTCLASS(model::Cell::test  );
    /// TESTCLASS(model::Config::test);
    /// TESTCLASS(model::Field::test );
    /// TESTCLASS(model::Person::test);

    TESTCLASS(TestGame::test);
}


///----------------------------------------------------------------------------|
/// Старт программы.
///----------------------------------------------------------------------- main:
int main()
{
    std::system( "chcp 65001>nul" );
/// SetConsoleOutputCP(65001);

    srand(unsigned(time(NULL)));

    std::cout << "Старт " << LOGO << "\n\n";

    tests();

    std::cout << "Программа закончила работу.\n" << std::endl;

    return 0;
}
