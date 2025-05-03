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

        unsigned money;

        void info() const
        {   std::cout << "///----------------------------|\n"
                      << "/// БАНК-ЛАБЕАН: " << money << "\n"
                      << "///----------------------------|\n\n";
        }
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
        std::array<unsigned,3>   buy; /// Банк покупает.
        std::array<unsigned,3>  sell; /// Банк продаёт.
        unsigned        difference{};
        float              persent{};

        ///-------------------------|
        /// Кол-во вещей.           |
        ///-------------------------:
        unsigned     amountThings{1};

        unsigned getBestBuy() const
        {   return std::max(buy[0], std::max(buy[1], buy[2]));
        }

        unsigned getBestSell() const
        {   return std::min(sell[0], std::max(sell[1], sell[2]));
        }

        friend std::ostream& operator<<(std::ostream& o, const Cell& cell);
    };


    ///----------------------------------|
    /// Вывод Cell в консоль.            |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const Cell& e)
    {   return o    << std::setw(4) << e.id
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
        virtual~IPerson() {};

        virtual void input () = 0;
        virtual void update() = 0;

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
        unsigned money   { 0 };

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
            const_cast<model::Config*>(&cfg)->managerEvents.make();
        }

        void infoCargo() const
        {   const Field& field = *(cfg.pfield);

            std::cout << "Инвентарь:\n";
            for(const auto&[id, n] : cargo)
            {   std::cout << "    " << std::setw(4) << id
                          << ",   " << n << " : "   << field[id].name << '\n';
            }   std::cout << "... ";

            if(cargo.empty())
            {   std::cout << "пусто ..." ;
            }   std::cout << "\n";
        }

        void infoName() const
        {   std::cout << ">> Имя персонажа: \""  << name << "\"\n\n";
        }

        void info() const
        {
            const auto& cell = (*(cfg.pfield))[position];
            unsigned  chance = cell.chance;
        /// Card*     card   = cell.card  ;

        /// auto n = 15 - nn + name.size();
            std::cout
                << "   Кошелёк = " << std::setw(4) << money    << "\n"
                << "   Позиция = " << std::setw(4) << position << "\n"
                << "   Статус  = " << std::setw(4)
                << cfg.decodeStatus(status)                    << "\n"
                << "   Круг    = " << std::setw(4) << circle   << "\n"
                << "   Шанс    = " << std::setw(4) << chance   << "\n"
                << "   Товар   = " <<                 cell.name<< "\n"
                << "   Кол-во  = " << std::setw(4)
                << (cell.amountThings != 0 ?
                          std::to_string(cell.amountThings) : " пусто") << "\n"
                << "   Продажа = " << std::setw(4) << cell.buy [status] << "\n"
                << "   Покупка = " << std::setw(4) << cell.sell[status] << "\n"
                << "\n";
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

        void input () override
        {
            Cell& cell = (*cfg.pfield)[position];

            bool goodSky = cell.status == IPerson::status;
            if(  goodSky)
            {   std::cout << "\"ЗВЁЗДЫ СВЕТЯТ МНЕ КРАСИВО!\"\n";
            }

            unsigned r = rand() % 3;
            std::cout << "Принято решение " << decodeDone[r] << '\n';

            Bank& bank = cfg.pfield->bank;

            switch(r)
            {
                ///----------------------------------------|
                /// Купить.                                |
                ///----------------------------------------:
                case 0:
                {
                    const unsigned price   = goodSky ?
                        cell.getBestSell() : cell.sell[status];

                    bool isMoney = money >= price;
                    bool isEmpty = cell.amountThings == 0;

                    if( isMoney && !isEmpty)
                    {        money -= price;
                        bank.money += price;
                      --cell.amountThings  ;

                        cargo[position] = 1;

                        std::cout << "Товар \"" << cell.name
                                  << "\" был куплен по цене: " << price << "\n";

                        isActBuy = true;
                    }
                    else if( isEmpty) std::cout << "... нет товара ...\n";
                    else if(!isMoney) std::cout << "... мало денег ...\n";

                    break;
                }

                ///----------------------------------------|
                /// Продать.                               |
                ///----------------------------------------:
                case 1:
                {
                    if(isActBuy)
                    {   std::cout << "В этом круге продажа заблокированы...\n";
                        break;
                    }

                    IPerson::infoCargo();

                    if(cargo.empty())
                    {   break;
                    }

                    const auto&[id, n] = *cargo.begin();

                    Cell& cellSell = (*cfg.pfield)[id];

                    const unsigned price  = goodSky ?
                        cell.getBestBuy() : cell.buy[status];

                    {
                             money += price;
                        bank.money -= price;
                      ++cellSell.amountThings  ;

                        cargo.erase(cargo.begin());

                        std::cout << "Товар \"" << cellSell.name
                                  << "\" был продан по цене: " << price << "\n";
                    }

                    break;
                }

                ///----------------------------------------|
                /// Ничего не делать.                      |
                ///----------------------------------------:
                default: ;
            }
        }

        void update() override
        {

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
                     person->info();

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

        void input () override
        {

        }

        void update() override
        {

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
                     person->info();

            delete   person;
        }
    };


    ///------------------------------------------------------------------------|
    /// Арбитер.
    ///---------------------------------------------------------------- Referee:
    struct  Referee
    {       Referee(const Config& Cfg) :
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
        void info() const
        {   for(unsigned i = 0; i < perses.size(); ++i)
            {   perses[order[i]]->infoName();
                perses[order[i]]->info    ();
            }
        }

        std::vector<model::IPerson*> perses;

        model::Field  field;

        model::Config* cfg;

        std::vector<model::Card >  cards;
        std::vector<model::Card*> pcards;

        myl::WhoFirstPlayer whoFirstPlayer;
        std::vector<unsigned>        order;

        ///------------------------------|
        /// Все игроки делают по 1 ходу. |
        ///------------------------------:
        bool  step()
        {
            for(unsigned i = 0; i < perses.size(); ++i)
            {
                field.bank.info();

                auto& pers = *perses[order[i]];
                      pers.infoName();

                const unsigned cubicDice = rand() % 6 + 1;

                std::cout << "cubicDice  = "
                          << std::setw(4) << cubicDice << '\n';

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
                pers.info   ();
                pers.input  ();

                std::cout << std::endl;
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
