#ifndef M_MODEL_H
#define M_MODEL_H

const char* const LOGO = "Model::Monopoly-2025[ver::0.2.1]";
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
    ///------------------------------------------------------------------ Cell2:
    struct  Cell
    {
        unsigned                  id;
        std::string             type;
        std::string             name;
        unsigned              chance;
        unsigned              status;
        unsigned           priseBase;
        std::array<unsigned,3>   buy;
        std::array<unsigned,3>  sell;
        unsigned        difference{};
        float              persent{};

        void debug() const
        {
        }

        friend std::ostream& operator<<(std::ostream& o, const Cell& cell);
    };


    ///----------------------------------|
    /// Вывод Cell в консоль.            |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const Cell& e)
    {   o   << std::setw(4) << e.id
            << std::setw(4) << e.chance
            << std::setw(4) << e.status
            << std::setw(4) << e.priseBase
            << std::setw(4) << e.buy [0]
            << std::setw(4) << e.buy [1]
            << std::setw(4) << e.buy [2]
            << std::setw(4) << e.sell[0]
            << std::setw(4) << e.sell[1]
            << std::setw(4) << e.sell[2]
            << "  "         << e.name;
        return o;
    }


    ///------------------------------------------------------------------------|
    /// Поле Модели.
    ///------------------------------------------------------------------ Field:
    struct  Field                    :  std::vector<Cell>
    {       Field(const Config& cfg) :  std::vector<Cell>
                                     {
                                        #include "field.inc"
                                     },
                                     cfg(cfg)
            {
            }

        struct PersonQ { unsigned pos; bool isStart{ false }; };

        PersonQ add(unsigned pos, unsigned step) const
        {   size_t res = pos + step;
            if (res >= size()) { return { unsigned(res -= size()), true }; }
            return  { (unsigned)res };
        }

        unsigned sub(unsigned pos, unsigned step) const
        {   size_t res = pos - step;
            if (res >= size()) res += size();
            return (unsigned)res;
        }

    friend  std::ostream& operator<<(std::ostream& o, const Field& field);

    private:
        const Config& cfg;

        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   {   Field   field(Config::getDefault());
                     ln(field)
            }
        }
    };


    ///----------------------------------|
    /// Вывод Cell в консоль.            |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const Field& field)
    {   for(const auto& cell : field)
        {          o << cell << '\n';
        }   return o;
    }


    ///------------------------------------------------------------------------|
    /// Персонаж.
    ///----------------------------------------------------------------- Person:
    struct  Person
    {       Person(const Config& cfg, std::string_view name)
                :   cfg (cfg ),
                    name(name)

            {   init();
            }

        const Config& cfg;

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

        void doEvent()
        {   if(cfg.managerEvents.empty()) return;

            std::cout << "Событие для " << name << ":\n";
            const_cast<model::Config*>(&cfg)->managerEvents.make();
        }

        void info() const
        {
            unsigned chance = (*(cfg.pfield))[position].chance;
        /// Card*    card   = (*(cfg.pfield))[position].card;

        /// auto n = 15 - nn + name.size();
            std::cout << ">> Имя     : \""               << name     << "\"\n"
                      << "   Деньги  = " << std::setw(4) << money    << "\n"
                      << "   Позиция = " << std::setw(4) << position << "\n"
                      << "   Статус  = " << std::setw(4)
                      << cfg.decodeStatus(status)                    << "\n"
                      << "   Круг    = " << std::setw(4) << circle   << "\n"
                      << "   Шанс    = " << std::setw(4) << chance   << "\n\n";
        }

        ///------------------------------|
        /// Для utf8 в std::setw.        |
        ///------------------------------:
        unsigned nn;
        void   init()
        {
        /// const auto     s = utf8ToWstr(name);
        /// nn = (unsigned)s.size();

            money = cfg.startMoney;
        }

    private:
        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Field   field(Config::getDefault());

            Config* cfg = const_cast<model::Config*>(&Config::getDefault());
                    cfg->pfield = &field;

            Person  person(Config::getDefault(), "Patison");
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
                ///------------------------|
                /// Профили игроков.       |
                ///------------------------:
                for(const auto& pl : Cfg.players)
                {   perses.emplace_back(Person(Cfg, pl.name));
                }

                ///------------------------|
                /// Инициализация ГПСЧ.    |
                ///------------------------:
                srand(unsigned(time(NULL)));

                ln(field)

                cfg = const_cast<model::Config*>(&Cfg);

                cfg->pfield = &field;
                cfg->managerEvents.initEvents<Referee>(this);
            //  cfg->managerEvents.push(0);
            //  cfg->managerEvents.make( );

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

        std::vector<model::Person> perses;

        model::Field  field;

        model::Config* cfg;

        std::vector<model::Card >  cards;
        std::vector<model::Card*> pcards;

        myl::WhoFirstPlayer whoFirstPlayer;
        std::vector<unsigned>        order;

        Bank bank;

        ///------------------------------|
        /// Все игроки делают по 1 ходу. |
        ///------------------------------:
        bool  step()
        {
            for(unsigned i = 0; i < perses.size(); ++i)
            {
                auto& pers = perses[order[i]];

                const unsigned randNumber = rand() % 6 + 1;
                ///          l(randNumber)

                const auto& [pos, isStart]
                    = field.add(pers.position, randNumber);

                pers.position = pos;

                ///------------------------------|
                /// Ячейка с шансом?             |
                ///------------------------------:
                if(field[pos].chance > 0) genChanse();

                if (isStart)
                {
                    if (++pers.status == 3) pers.status = 0;

                    ++pers.circle;
                }

                pers.doEvent();
                pers.info();

                //std::cout << std::endl;
            }
            return true;
        }

        ///------------------------------|
        /// Генератор события.           |
        ///------------------------------:
        void genChanse()
        {
            if(unsigned randN = (rand() % 10); randN < 3)
            {
                unsigned randNChance = rand() % cfg->managerEvents.getSize();

                cfg->managerEvents.push(randNChance);
            }
        }

        friend struct TestGame;
        friend struct ManagerEvents;
    };

}   /// namespace model


#endif // M_MODEL_H
