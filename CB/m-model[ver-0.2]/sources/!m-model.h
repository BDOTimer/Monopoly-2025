#ifndef M_MODEL_H
#define M_MODEL_H

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

        int money;

        [[nodiscard]]
        const std::string info() const
        {   std::stringstream ss;
            ss  << "///----------------------------|\n"
                << "/// БАНК-ЛАБЕАН: " << money << "\n"
                << "///----------------------------|\n\n";
            return ss.str();
        }
    };


    ///------------------------------------------------------------------------|
    /// Описание(класс) ячейки на карте.
    ///------------------------------------------------------------------ Cell2:
    struct  Cell
    {
        unsigned                id;
        std::string           type;
        std::string           name;
        unsigned            chance;
        unsigned            status;
        int              priseBase;
        std::array<int,3> bankBuy ; /// Банк покупает.
        std::array<int,3> bankSell; /// Банк продаёт.
        int           difference{};
        float            persent{};

        ///-------------------------|
        /// Кол-во вещей.           |
        ///-------------------------:
        unsigned     amountThings{1};

        unsigned getBestBuy() const
        {   return std::max(bankBuy[0], std::max(bankBuy[1], bankBuy[2]));
        }

        unsigned getBestSell() const
        {   return std::min(bankSell[0], std::min(bankSell[1], bankSell[2]));
        }

        friend std::ostream& operator<<(std::ostream& o, const Cell& cell);

        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Cell cell{  0, "Дети", "Завод воздушных шариков.",
              0,   1, 120,
            100, 140, 120,
            140, 100, 120};

            ln(cell)
            l( cell.getBestBuy ())
            l( cell.getBestSell())
        }
    };


    ///----------------------------------|
    /// Вывод Cell в консоль.            |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const Cell& e)
    {   return o    << std::setw(4) << e.id
                    << std::setw(4) << e.chance
                    << std::setw(4) << e.status
                    << std::setw(4) << e.priseBase
                    << std::setw(4) << e.bankBuy [0]
                    << std::setw(4) << e.bankBuy [1]
                    << std::setw(4) << e.bankBuy [2]
                    << std::setw(4) << e.bankSell[0]
                    << std::setw(4) << e.bankSell[1]
                    << std::setw(4) << e.bankSell[2]
                    << "  "         << e.name;
    }


    ///------------------------------------------------------------------------|
    /// Поле Модели.
    ///------------------------------------------------------------------ Field:
    struct  Field                    :  std::vector<Cell>
    {       Field(const Config& cfg) :  std::vector<Cell>
                                     {
                                        #include "field.inc"
                                     },
                                     bank(cfg),
                                     cfg (cfg)

            {
            }

        Bank   bank;

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
    /// Интерфейс персонажа.
    ///---------------------------------------------------------------- IPerson:
    struct      IPerson
    {           IPerson(const Config& cfg, std::string_view name)
                    :   cfg  (cfg ),
                        name (name)
                {}
        virtual~IPerson()  {};

        [[nodiscard]]
        virtual const std::string input () = 0;

        const Config& cfg;

        ///------------------------------|
        /// Имя персонажа.               |
        ///------------------------------:
        std::string name;

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
        int      money   { 0 };

        //-------------------------------|
        /// Была покупка в этом круге?   |
        ///------------------------------:
        bool isActBuy  {false};

        ///------------------------------|
        /// Список собственности.        |
        ///------------------------------:
        std::map<unsigned /*     id */,
                 unsigned /* amount */>  cargo;

        ///------------------------------|
        /// Награды.                     |
        ///------------------------------:
        std::map<std::string, unsigned>  medals;

        ///------------------------------|
        /// Особые условия.              |
        ///------------------------------:
        std::map<std::string, unsigned>  specs;

        std::string_view decodeDone[3]
        {   "купить." ,
            "продать.",
            "ничего не делать."
        };

        void nextCircle()
        { ++circle;
            isActBuy = false;
        }

        void doEvent()
        {   if(cfg.managerEvents.empty()) return;

            std::cout << "Событие для " << name << ":\n";
            const auto mess
                = const_cast<model::Config*>(&cfg)->managerEvents.make();

            printf(mess.c_str());
        }

        [[nodiscard]]
        const std::string infoCargo() const
        {   const Field& field = *(cfg.pfield);

            std::stringstream ss;

            ss << "Инвентарь:\n";
            for(const auto&[id, n] : cargo)
            {   ss  << "    " << std::setw(4) << id
                    << ",   " << n << " : "   << field[id].name << '\n';
            }   ss  << "... ";

            if(cargo.empty())
            {   ss << "пусто ..." ;
            }   ss << "\n";

            return ss.str();
        }

        [[nodiscard]]
        const std::string infoName() const
        {   std::stringstream ss;
                              ss  << ">> Имя персонажа: \"" << name << "\"\n";
            return            ss.str();
        }

        [[nodiscard]]
        const std::string info() const
        {
            std::stringstream ss;

            const auto& cell = (*(cfg.pfield))[position];
            unsigned  chance = cell.chance;
        /// Card*     card   = cell.card  ;

        /// auto n = 15 - nn + name.size();
            ss  << "   Кошелёк = " << std::setw(4) << money    << "\n"
                << "   Позиция = " << std::setw(4) << position << "\n"
                << "   Статус  = " << std::setw(4)
                << cfg.decodeStatus(status)                    << "\n"
                << "   Круг    = " << std::setw(4) << circle   << "\n"
                << "   Шанс    = " << std::setw(4) << chance   << "\n"
                << "   Товар   = " <<                 cell.name<< "\n"
                << "   Кол-во  = " << std::setw(4)
                << (cell.amountThings != 0 ?
                          std::to_string(cell.amountThings) : " пусто") << "\n"
                << "   Банк покупает: " << std::setw(4)
                                   << cell.bankBuy [status] << "\n"
                << "   Базовая цена : " << std::setw(4)
                                   << cell.priseBase << "\n"
                << "   Банк продаёт : " << std::setw(4)
                                   << cell.bankSell[status] << "\n"
                << "\n";

            return ss.str();
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
    };


    ///------------------------------------------------------------------------|
    /// Персонаж Бот.
    ///-------------------------------------------------------------- PersonBot:
    struct  PersonBot : IPerson
    {       PersonBot(const Config& cfg, std::string_view name)
                      : IPerson    (cfg, name)
            {
                init();
            }

        const std::string input () override
        {
            std::stringstream ss;

            Cell& cell = (*cfg.pfield)[position];

            bool goodSky = cell.status == IPerson::status;
            if(  goodSky)
            {   ss << "\"ЗВЁЗДЫ СВЕТЯТ МНЕ КРАСИВО!\"\n";
            }

            unsigned r = rand() % 3;
            ss << "Принято решение " << decodeDone[r] << '\n';

            Bank& bank = cfg.pfield->bank;

            switch(r)
            {
                ///----------------------------------------|
                /// Купить.                                |
                ///----------------------------------------:
                case 0:
                {
                    const int price   = goodSky ?
                        cell.getBestSell() : cell.bankSell[status];

                    bool isMoney = money >= price;
                    bool isEmpty = cell.amountThings == 0;

                    if( isMoney && !isEmpty)
                    {        money -= price;
                        bank.money += price;
                      --cell.amountThings  ;

                        cargo[position] = 1;

                        ss  << "Товар \"" << cell.name
                            << "\" был куплен по цене: " << price << "\n";

                        isActBuy = true;
                    }
                    else if( isEmpty) ss << "... нет товара ...\n";
                    else if(!isMoney) ss << "... мало денег ...\n";

                    break;
                }

                ///----------------------------------------|
                /// Продать.                               |
                ///----------------------------------------:
                case 1:
                {
                    if(isActBuy)
                    {   ss << "В этом круге продажа заблокированы...\n";
                        break;
                    }

                    ss << IPerson::infoCargo();

                    if(cargo.empty())
                    {   break;
                    }

                    const auto&[id, n] = *cargo.begin();

                    Cell& cellSell = (*cfg.pfield)[id];

                    const int price  = goodSky ?
                        cell.getBestBuy() : cell.bankBuy[status];

                    {
                             money += price;
                        bank.money -= price;
                      ++cellSell.amountThings  ;

                        cargo.erase(cargo.begin());

                        ss  << "Товар \"" << cellSell.name
                            << "\" был продан по цене: " << price << "\n";
                    }

                    break;
                }

                ///----------------------------------------|
                /// Ничего не делать.                      |
                ///----------------------------------------:
                default: ;
            }
            return ss.str();
        }

    private:
        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Field   field(Config::getDefault());

            Config* cfg = const_cast<model::Config*>(&Config::getDefault());
                    cfg->pfield = &field;

            IPerson* person = new PersonBot(*cfg, "bot:gudleifr");
                  ln(person->infoName());
                  ln(person->info    ());

            delete   person;
        }
    };


    ///------------------------------------------------------------------------|
    /// Персонаж Человек.
    ///------------------------------------------------------------ PersonHuman:
    struct  PersonHuman : IPerson
    {       PersonHuman(const Config& cfg, std::string_view name)
                        : IPerson    (cfg, name)
            {
                init();
            }

        const std::string input () override
        {
            std::stringstream ss;

            /// ...

            return ss.str();
        }


    private:
        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Field   field(Config::getDefault());

            Config* cfg = const_cast<model::Config*>(&Config::getDefault());
                    cfg->pfield = &field;

            IPerson* person = new PersonHuman(*cfg, "Slava-rusi11");
                  ln(person->info())

            delete   person;
        }
    };


    ///------------------------------------------------------------------------|
    /// Арбитер.
    ///---------------------------------------------------------------- Referee:
    struct  Referee
    {       Referee  (const Config& Cfg) :
                field(Cfg),
                whoFirstPlayer(Cfg.amountPlayers)
            {
                ///------------------------|
                /// Профили игроков.       |
                ///------------------------:
                for(const auto& pl : Cfg.players)
                {   if(pl.isBot)
                    {   perses.emplace_back(new PersonBot(Cfg, pl.name));
                    }
                    else
                    {   perses.emplace_back(new PersonHuman(Cfg, pl.name));
                    }
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
                {   std::cout << perses[order[i]]->name << '\n';
                }
            }
           ~Referee()
            {   for(auto p : perses) delete p;
            }

    protected:
        [[nodiscard]]
        const std::string info() const
        {   std::stringstream ss;
            for(unsigned i = 0; i < perses.size(); ++i)
            {   ss << perses[order[i]]->infoName();
                ss << perses[order[i]]->info    ();
            }
            return ss.str();
        }

        std::vector<model::IPerson*> perses;

        model::Field  field;

        model::Config* cfg;

        std::vector<model::Card >  cards;
        std::vector<model::Card*> pcards;

        myl::WhoFirstPlayer whoFirstPlayer;
        std::vector<unsigned>        order;

        IPerson* persNow{nullptr};

        ///------------------------------|
        /// Все игроки делают по 1 ходу. |
        ///------------------------------:
        bool  step()
        {
            for(unsigned i = 0; i < perses.size(); ++i)
            {
                printf(field.bank.info().c_str());

                persNow = perses[order[i]];

                auto& pers = *persNow;
                std::cout <<  pers.infoName();

                const unsigned cubicDice = rand() % 6 + 1;

                std::cout <<
                    "           ╔═══╗\n"
                    "cubicDice:═╣ " << cubicDice << " ║\n"
                    "           ╚═══╝\n";

                const auto& [pos, isStart]
                    = field.add(pers.position, cubicDice);

                pers.position = pos;

                ///------------------------------|
                /// Ячейка с шансом?             |
                ///------------------------------:
                if(field[pos].chance > 0) genChanse();

                if (isStart)
                {
                    if (++pers.status == 3) pers.status = 0;

                    pers.nextCircle();
                }

                pers.doEvent();
                std::cout << pers.info ();
                std::cout << pers.input();

                std::cout << std::endl;
            }
            return true;
        }

        ///------------------------------|
        /// Генератор события.           |
        ///------------------------------:
        void genChanse()
        {
            if(unsigned randN = (rand() % 100); randN < cfg->percentDoChanse)
            {
                const unsigned randNChance = cfg->managerEvents.getNRnd();
                cfg->managerEvents.push(randNChance);
            }
        }

        friend struct TestGame;
        friend struct ManagerEvents;
    };

}   /// namespace model


#endif // M_MODEL_H
