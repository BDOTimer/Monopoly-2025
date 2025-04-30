#ifndef M_MODEL_H
#define M_MODEL_H

const char* const LOGO = "Model::Monopoly-2025[ver::0.0.2]";
///----------------------------------------------------------------------------|
/// "!m-model.h"
/// Модель игры Монополия-2025.
///----------------------------------------------------------------------------:
#include "myl.h"

#include <locale>
#include <codecvt>


inline std::string WstrToUtf8(const std::wstring& str)
{   std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.to_bytes(str);
}

inline std::wstring utf8ToWstr(const std::string& str)
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
    /// Банк.
    ///------------------------------------------------------------------- Bank:
    struct  Bank
    {       Bank(const Config& cfg) : money(cfg.moneyBank)
            {
            }

        unsigned money;
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
    /// Описание(класс) ячейки на карте.
    ///------------------------------------------------------------------ Cell2:
    struct  Cell2
    {
        unsigned                  id;
        std::string             type;
        std::string             name;
        unsigned              chance;
        unsigned              status;
        unsigned           priseBase;
        std::array<unsigned,3>   buy;
        std::array<unsigned,3>  sell;
    };


    ///------------------------------------------------------------------------|
    /// Поле Модели.
    ///------------------------------------------------------------------ Field:
    struct  Field2   :  std::vector<Cell2>
    {       Field2() :  std::vector<Cell2>
                     {
                        #include "field.inc"
                     }
            {
            }

        void debug()
        {   for(const auto& e : *this)
            {
                std::cout << std::setw(4) << e.id
                          << std::setw(4) << e.chance
                          << std::setw(4) << e.status
                          << std::setw(4) << e.priseBase
                          << std::setw(4) << e.buy [0]
                          << std::setw(4) << e.buy [1]
                          << std::setw(4) << e.buy [2]
                          << std::setw(4) << e.sell[0]
                          << std::setw(4) << e.sell[1]
                          << std::setw(4) << e.sell[2]
                          << "  " << e.name << '\n';
            }
        }

    private:
        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   {   Field2  field;
                        field.debug();
            }
        }
    };


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

		friend struct Referee;
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

        void make()
        {   const_cast<model::Config*>(cfg)->managerEvents.make();
        }

        void info() const
        {
            unsigned chance = (*(cfg->pfield))[position].chance;
            Card*    card   = (*(cfg->pfield))[position].card;

        /// auto n = 15 - nn + name.size();
            std::cout << ">> "                           << name     << "\n"
                      << "Деньги  = "    << std::setw(4) << money    << "\n"
                      << "Позиция = "    << std::setw(4) << position << "\n"
                      << "Статус  = "    << std::setw(4)
                      <<  cfg->decodeStatus(status)                  << "\n"
                      << "Круг    = "    << std::setw(4) << circle   << "\n"
                      << "Шанс    = "    << std::setw(4) << chance   << "\n"
                      <<  card->infoName()                           << "\n\n";
        }

        ///------------------------------|
        /// Для utf8 в std::setw.        |
        ///------------------------------:
        unsigned nn;
        void   init()
        {
        /// const auto     s = utf8ToWstr(name);
        /// nn = (unsigned)s.size();

            money = cfg->startMoney;
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
    {       Referee(const Config& Cfg) :
                field(Cfg),
                whoFirstPlayer(Cfg.amountPlayers),
                bank (Cfg)
            {
                srand(unsigned(time(NULL)));

                for (auto& pers : perses)
                {   pers.cfg   = &Cfg;
                    pers.init();
                }

                cfg = const_cast<model::Config*>(&Cfg);

                cfg->pfield = &field;
                cfg->managerEvents.initEvents<Referee>(this);
            //  cfg->managerEvents.push(0);
            //  cfg->managerEvents.make( );

                getCards   ();
                cards2Field();

                if(cfg->isMixerCards) mixerCards();

                order = myl::WhoFirstPlayer::getFastOrder(Cfg.amountPlayers);

                std::cout << "Жеребьевка: ";
                for(const auto n : order) std::cout << (n+1) << " ";

                std::cout << '\n';

                for(unsigned i = 0; i < perses.size(); ++i)
                {   std::cout << perses[order[i]].name << '\n';
                }
            }

    protected:
        void info() const
        {   for(unsigned i = 0; i < perses.size(); ++i)
            {   perses[order[i]].info();
            }
        }

        std::vector<model::Person> perses
        {   {"Bot:Pete"   },
            {"Bot:Ann"    },
            {"Вася Пупкин"}
        };

        model::Field  field;

        model::Config* cfg;

        std::vector<model::Card >  cards;
        std::vector<model::Card*> pcards;

        myl::WhoFirstPlayer whoFirstPlayer;
        std::vector<unsigned>        order;

        Bank bank;

        ///------------------------------|
        /// Получить карточки из конфига.|
        ///------------------------------:
        void getCards()
        {
            cards.reserve(cfg->amountCells);

            for(const auto& type : cfg->cardNames)
            {
                std::string_view t = type.front();

                for(size_t i = 2; i < type.size(); i += 2)
                {   cards.emplace_back( Card{t, type[i], type[i+1]});
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

        ///------------------------------|
        /// Все игроки делают по 1 ходу. |
        ///------------------------------:
        bool  step()
        {
            for(unsigned i = 0; i < perses.size(); ++i)
            {
                if(unsigned randN = (rand() % 10); randN < 3)
                {   cfg->managerEvents.push(0);
                }

                auto& pers = perses[order[i]];

                const unsigned randNumber = rand() % 6 + 1;
                ///          l(randNumber)

                const auto& [pos, isStart]
                    = field.add(pers.position, randNumber);

                if (isStart)
                {
                    if (++pers.status == 3) pers.status = 0;

                    ++pers.circle;
                }

                pers.position = pos;
                pers.make();
                pers.info();
            }
            return true;
        }

        friend struct TestGame;
        friend struct ManagerEvents;
    };

}   /// namespace model


#endif // M_MODEL_H
