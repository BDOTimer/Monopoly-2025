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
    struct  IPerson;
    struct  Card
    {
        ///------------------------------|
        /// Из "cards.inc"               |
        ///------------------------------:
        unsigned              id; /// Идентификатор.
        unsigned            type; /// Тип.
        int             money[3]; /// Монеты от статуса.
        int             steps[2]; /// Ходы.
        unsigned           count; /// Сила в кол-ве раз действии.

        ///------------------------------|
        /// Служебная инфа.              |
        ///------------------------------:
        int buf[3]{};

        bool empty() const { return 0 == count; }

        ///------------------------------|
        /// Получаем имя из type.        |
        ///------------------------------:
        std::string_view decodeName() const
        {   if(type >= N) return name[N - 1];
            return name[type];
        }

        std::string doAct(IPerson* pers);

    private:
        ///------------------------------|
        /// Расшифровки типов.           |
        ///------------------------------:
        inline static const char*   name[]
        {   "Сюрприз",
            "Золотая",
            "Джокер" ,
            "Просроченная"
        };
        inline static unsigned N{sizeof name / sizeof *name};

        std::string infoWhat(std::string s)
        {   s.resize(11, ' ');
            std::stringstream ss;
            ss << "///--------------------------|\n"
               << "/// Событие Шанс: " << s << "| <--- " << decodeName() << '\n'
               << "///--------------------------|\n\n";
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& o, const Card& card);
        friend struct Cards;
    };


    ///----------------------------------|
    /// Вывод Card в консоль.            |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const Card& card)
    {   return o    << std::setw(3) << card.type     << ", "
                    << std::setw(4) << card.money[0] << ", "
                    << std::setw(4) << card.money[1] << ", "
                    << std::setw(3) << card.money[2] << ", "
                    << std::setw(3) << card.steps[0] << ", "
                    << std::setw(3) << card.steps[1] << ", "
                    << card.decodeName();
    }


    ///------------------------------------------------------------------------|
    /// Коллекция карточек.
    ///------------------------------------------------------------------- Card:
    struct  Cards   : std::vector<Card>
    {       Cards() : std::vector<Card>
                    {
                        #include "cards.inc"
                    }
            {   init();
            }

    friend std::ostream& operator<<(std::ostream& o, const Cards& card);

    Card* getCard()
    {   if(pcards.empty()) return nullptr;
        Card*  pc = pcards.back();
        pcards.pop_back();
        return pc;
    }

    private:
        std::vector<Card*> pcards;

        void init()
        {   pcards.clear();
            pcards.reserve(size());
            for(Card& e : *this) pcards.push_back(&e);
            myl::mixer(pcards);
        }

        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Cards   cards;
                 ln(cards)
                 ln(cards[0 ].infoWhat("-30 "))
                 ln(cards[25].infoWhat("4, 6"))
        }
    };


    ///----------------------------------|
    /// Вывод Cards в консоль.           |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const Cards& cards)
    {   for(const auto& card : cards)
        {          o << card << '\n';
        }   return o;
    }


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
    struct  IPerson;
    struct  Cell
    {
        unsigned                     id;
        std::string                type;
        std::string                name;
        unsigned                 chance;
        unsigned                 status;
        int                   priseBase;
        std::array<int     ,3> bankSell; /// Банк продаёт.
        std::array<int     ,3> bankBuy ; /// Банк покупает.
        std::array<unsigned,3> rangSell; /// Выгода покупки для персонажа.
        std::array<unsigned,3> rangBuy ; /// Выгода продажи для персонажа.
        int                difference{};
        float                 persent{};

        ///----------------------------|
        /// кол-во вещей.              |
        ///----------------------------:
        unsigned        amountThings{1};

        ///----------------------------|
        /// Чья ячейка?                |
        ///----------------------------:
        IPerson*          pers{nullptr};

        ///----------------------------|
        /// Ячейка занята?             |
        ///----------------------------:
        bool isBusy() const { return nullptr != pers; }

        unsigned getBestBuy() const
        {   return std::max(bankBuy[0], std::max(bankBuy[1], bankBuy[2]));
        }

        unsigned getBestSell() const
        {   return std::min(bankSell[0], std::min(bankSell[1], bankSell[2]));
        }

        friend std::ostream& operator<<(std::ostream& o, const Cell& cell);

        void init()
        {
            {   std::array<std::pair<int, int>, 3> m
                {   std::pair{bankSell[0], 0},
                    std::pair{bankSell[1], 1},
                    std::pair{bankSell[2], 2}
                };
                std::sort(m.begin(), m.end());

                rangSell[0] = m[0].second;
                rangSell[1] = m[1].second;
                rangSell[2] = m[2].second;
            }
            {
                std::array<std::pair<int, int>, 3> m
                {   std::pair{bankBuy[0], 0},
                    std::pair{bankBuy[1], 1},
                    std::pair{bankBuy[2], 2}
                };
                std::sort(m.begin(), m.end());

                rangBuy[0] = m[2].second;
                rangBuy[1] = m[1].second;
                rangBuy[2] = m[0].second;
            }
        }


        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Cell cell
            {     0, "Дети", "Завод воздушных шариков.",
                  0,      1, 120,
                100,    140, 120,
                140,    100, 120
            };

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


    struct CellInfoTester : Cell
    {
        friend std::ostream& operator<<(std::ostream&, const CellInfoTester&);

        TEST
        {   
            Cell cell
            {     0, "Дети", "Завод воздушных шариков.",
                  0,      1, 120,
                100,    140, 120,
                140,    100, 120
            };
            std::cout << (CellInfoTester)cell << '\n';
        }
    };

    std::ostream& operator<<(std::ostream& o, const CellInfoTester& e)
    {   return o
            << "ЯЧЕЙКА: ---------------------------------: " << e.id << '\n'
            << "   Позиция      :  " << std::setw(4)  << e.id        << '\n'
            << "   Товар        :  " << e.name                       << '\n'
            << "   Шанс         :  " << std::setw(4)  << e.chance    << '\n'
            << "   Статус       :  " << std::setw(4)  << e.status    << '\n'
            << "   Цена базовая :  " << std::setw(4)  << e.priseBase << '\n'
            << "   Банк покупает: [" << e.bankBuy [0] << ", "
                                     << e.bankBuy [1] << ", "
                                     << e.bankBuy [2] << "]\n"
            << "   Банк продает : [" << e.bankSell[0] << ", "
                                     << e.bankSell[1] << ", "
                                     << e.bankSell[2] << "]\n"
        ;
    }



    ///------------------------------------------------------------------------|
    /// Поле Модели.
    ///------------------------------------------------------------------ Field:
    struct  Field                    :  std::vector<Cell>
    {       Field(const Config& cfg) :  std::vector<Cell>
                                     {
                                        #include "field.inc"
                                     }
                                     , bank(cfg)
                                     , cfg (cfg)
            {
                for(auto& e : *this) e.init();
            }

        Bank   bank;
        Cards cards;

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

    /// ERROR ...(не рабочий, почему?)
    ///----------------------------------|
    /// Вывод Cell в консоль.            |
    ///----------------------------------:
    std::function<bool(const unsigned, const unsigned)> //, decltype(comparator)
    comparator{[](const unsigned lhs,
                  const unsigned rhs)
    {   return lhs < rhs;
    }};

    ///------------------------------------------------------------------------|
    /// Интерфейс персонажа.
    ///---------------------------------------------------------------- IPerson:
    struct      IPerson
    {           IPerson(const Config& cfg, unsigned id)
                    :   cfg  (cfg )
                    ,    id  ( id )
                    ,   name (cfg.getNamePlayer(id))
                {
                }
        virtual~IPerson()  {};

        [[nodiscard]]
        virtual const std::string input() = 0;
        virtual void              doAct() = 0;

        const Config& cfg;

        ///------------------------------|
        /// Идентификатор.               |
        ///------------------------------:
        unsigned id;

        ///------------------------------|
        /// Имя персонажа.               |
        ///------------------------------:
        std::string  name;

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

        ///------------------------------|
        /// Вся собственность в монетах. |
        ///------------------------------:
        int     capital  { 0 };

        //-------------------------------|
        /// Была покупка в этом круге?   |
        ///------------------------------:
        bool isActBuy  {false};

        ///------------------------------|
        /// Список собственности.        |
        ///------------------------------:
        std::multimap<unsigned,/* status */
                      unsigned /*     id */>  cargo;

        ///------------------------------|
        /// Награды.                     |
        ///------------------------------:
        std::map<std::string, unsigned>  medals;

        ///------------------------------|
        /// Особые условия.              |
        ///------------------------------:
        std::map<std::string, unsigned>  specs;

        ///------------------------------|
        /// Карточка.                    |
        ///------------------------------:
        Card* pcard{nullptr};

        std::string_view decodeDone[3]
        {   "купить." ,
            "продать.",
            "ничего не делать."
        };

        ///------------------------------|
        /// Добавить Вещь в инвентарь.   |
        ///------------------------------:
        void addThing(const Cell& cell)
        {   cargo.insert(std::pair{cell.status, cell.id});
            capital += cell.priseBase;
        }

        ///------------------------------|
        /// Удалить Вещь из инвентаря.   |
        ///------------------------------:
        void deleteThing(std::multimap<unsigned, unsigned>::iterator it)
        {   const auto&[sts, id] = *it;
            Cell&       cellSell = (*cfg.pfield)[id];
            capital -= cellSell.priseBase;
            cargo.erase(it);
        }

        void nextCircle()
        { ++circle;
            isActBuy = false;
        }

        [[nodiscard]]
        const std::string infoCargo() const
        {   const Field& field = *(cfg.pfield);

            std::stringstream ss;

            ss << "Инвентарь:\n";
            for(const auto&[sts, id] : cargo)
            {   ss  << "    " << std::setw(4) << id
                    << ",   " << sts << " : "   << field[id].name << '\n';
            }   ss  << "... ";

            if(cargo.empty())
            {   ss << "пусто ..." ;
            }   ss << "\n";

            ss << "Вся стоимость всей собственности: " << capital << '\n';

            return ss.str();
        }

        ///------------------------------|
        /// Расчет аренды.               |
        ///------------------------------:
        int calcRent(const IPerson* guest)
        {   
            /// TODO: ...

            return 0;
        }

        ///------------------------------|
        /// Инфа об имени игроке.        |
        ///------------------------------:
        [[nodiscard]]
        const std::string infoName() const
        {   std::stringstream ss;
                              ss  << ">> Имя персонажа: \"" << name << "\"\n";
            return            ss.str();
        }

        ///------------------------------|
        /// Инфа об игроке.              |
        ///------------------------------:
        [[nodiscard]]
        const std::string info() const
        {
            std::stringstream ss;

            const auto& cell = (*(cfg.pfield))[position];
            unsigned  chance = cell.chance;
        /// Card*     card   = cell.card  ;

        ///------------------------------|
        /// Чья ячейка?                  |
        ///------------------------------:
        cell.pers == nullptr
            ?   ss  << "Эта ячейка свободна для продажи!\n" 
            :   ss  << "Эта ячейка принадлежит игроку "
                    << cell.pers->name << '\n';
        

        /// auto n = 15 - nn + name.size();
            ss  << "   Кошелёк = " << std::setw(4) << money      << "\n"
                << "   Позиция = " << std::setw(4) << position   << "\n"
                << "   Статус  = " << std::setw(4)
                << cfg.decodeStatus(status)<< ": ["<< status+1   << "]\n"
                << "   Круг    = " << std::setw(4) << circle     << "\n"
                << "   Шанс    = " << std::setw(4) << chance     << "\n"
                << "   Товар   = " <<                 cell.name
                << ": Cтатус: ["   <<                 cell.status<< "]\n"
                << "   Кол-во  = " << std::setw(4)
                << (cell.amountThings != 0 ?
                    std::to_string(cell.amountThings) : " пусто")<< "\n"
                << "   Банк покупает: " << std::setw(4)
                                        << cell.bankBuy [status] << "\n"
                << "   Базовая цена : " << std::setw(4)
                                        << cell.priseBase        << "\n"
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

        friend std::ostream& operator<<(std::ostream&, const IPerson*);
    };


    ///----------------------------------|
    /// Вывод IPerson в поток.           |
    ///----------------------------------:
    std::ostream& operator<<(std::ostream& o, const IPerson* p)
    {   const Config& cfg{p->cfg};
        const auto&   cell = (*(cfg.pfield))[p->position];

        o   << "Игрок: ----------------------------------: "  << p->id << '\n'
            << "   Статус: " << std::setw(4) << p->status + 1 << " ---> "    
                             << cfg.decodeStatus(p->status)   << "\n"
            << "   Круг  : " << std::setw(4) << p->circle     << "\n";
            
            ///------------------------------|
            /// Чья ячейка?                  |
            ///------------------------------:
            cell.pers == nullptr
                ?   o << "   Эта ячейка свободна для продажи!\n" 
                :   o << "   Эта ячейка принадлежит игроку "
                      << cell.pers->name << '\n';
        ;
        return o;
    }


    ///------------------------------------------------------------------------|
    /// ВСЕ ПРАВИЛА для карточек ШАНС.
    ///------------------------------------------------------------ Card::doAct:
    std::string Card::doAct(IPerson* pers)
    {
        if(0 == count)
        {   return "";
        }

        count--;

        std::stringstream ss;

        switch(type)
        {
            case 0:
            {
            }
            case 1:
            {
                const int mn{money[pers->status]};
                pers->money += mn;
                return infoWhat(std::to_string(mn) + std::string("$"));
            }
            case 2:
            {
                ///ss << steps[0] << ", " << steps[1];
                ss << "TODO:?";
                break;
            }
        }

        return infoWhat(ss.str());
    }


    ///------------------------------------------------------------------------|
    /// Персонаж Бот.
    ///-------------------------------------------------------------- PersonBot:
    struct  PersonBot : IPerson
    {       PersonBot(const Config& cfg, unsigned id)
                      : IPerson    (cfg, id)
            {
                init();
                botIQ = cfg.getBotIQ(id);
            }

        ///------------------------------|
        /// Интеллект бота               |
        ///------------------------------:
        const BotProfileIQ* botIQ{nullptr};


        const std::string input () override
        {
            std::stringstream ss;

            Cell& cell = (*cfg.pfield)[position];

            if(cell.status - 1 > 2) std::cout << "ERROR::PersonBot::input\n";

            bool goodSky = cell.status - 1 == IPerson::status;
            if(  goodSky)
            {   ss << "\"ЗВЁЗДЫ СВЕТЯТ МНЕ КРАСИВО!\"\n";
            }

            unsigned r = rand() % 3;

            ss << "Принято решение " << decodeDone[r] << '\n';

            if(cell.isBusy())
            {   ss  << "... нет товара ...\n"
                    << "Стоимость[TODO] аренды ячейки: " 
                    << cell.pers->calcRent(this) << '\n';

                if( r == 0)
                {   r =  2;
                }
            }

            {   ///----------------------------------------|
                /// IQ.                                    |
                ///----------------------------------------:
                bool canBuy  = botIQ->canBuyBot (cell.status);
                bool canSell = botIQ->canSellBot(cell.status);

                if(0 == r && !canBuy )
                {   r = 2; ss << "botIQ::передумал покупать\n";
                }
                if(1 == r && !canSell)
                {   r = 2; ss << "botIQ::передумал продавать\n";
                }   
            }

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

                        ///------------------------------|
                        /// Добавить Вещь в инвентарь.   |
                        ///------------------------------:
                    /// cargo[position] = 1;
                    /// cargo.insert(std::pair{cell.status, position});
                        addThing(cell);

                        ss  << "Товар \"" << cell.name
                            << "\" был куплен по цене: " << price << "\n";

                        ss << IPerson::infoCargo();

                        isActBuy = true;

                        cell.pers = this;
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

                    auto it = cargo.begin();

                    const auto&[sts, id] = *it;

                    Cell& cellSell = (*cfg.pfield)[id];

                    const int price  = goodSky ?
                        cellSell.getBestBuy() : cellSell.bankBuy[status];

                    {
                             money += price;
                        bank.money -= price;
                      ++cellSell.amountThings;

                        ///------------------------------|
                        /// Удалить Вещь из инвентаря.   |
                        ///------------------------------:
                        deleteThing(it);

                        ss  << "Товар \"" << cellSell.name
                            << "\" был продан по цене: " << price << "\n";

                        ss << IPerson::infoCargo();

                        cell.pers = nullptr;
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

        void doAct() override
        {
            if(pcard == nullptr) return;
            if(pcard -> empty()) { pcard = nullptr; return; }

            std::cout << "Событие для " << name << ":\n";
            std::cout << pcard->doAct(this).c_str();
        }

    private:
        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Field   field(Config::getDefault());

            Config* cfg = const_cast<model::Config*>(&Config::getDefault());
                    cfg->pfield = &field;

            IPerson* person = new PersonBot(*cfg, 0);
                  ln(person->infoName());
                  ln(person->info    ());

            delete   person;
        }
    };


    ///------------------------------------------------------------------------|
    /// Персонаж Человек.
    ///------------------------------------------------------------ PersonHuman:
    struct  PersonHuman : IPerson
    {       PersonHuman(const Config& cfg, unsigned id)
                        : IPerson    (cfg, id)
            {
                init();
            }

        const std::string input () override
        {
            std::stringstream ss;

            /// ...

            return ss.str();
        }

        void doAct() override
        {   if(nullptr == pcard) return;
            std::cout << "Событие для " << name << ":\n";
            std::cout << pcard->doAct(this);
        }


    private:
        ///------------------------------|
        /// Тест класса.                 |
        ///------------------------------:
        TEST
        {   Field   field(Config::getDefault());

            Config* cfg = const_cast<model::Config*>(&Config::getDefault());
                    cfg->pfield = &field;

            IPerson* person = new PersonHuman(*cfg, 3); /// "Slava-rusi11"
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
                unsigned seed = Cfg.isSeed ? Cfg.isSeed : unsigned(time(NULL));

                ///------------------------|
                /// Инициализация ГПСЧ.    |
                ///------------------------:
                srand(seed);

                ///------------------------|
                /// Профили игроков.       |
                ///------------------------:
                unsigned idBot{0};
                for(const auto& pl : Cfg.players)
                {   if(pl.isBot)
                    {   perses.emplace_back(new PersonBot(Cfg, idBot));
                    }
                    else
                    {   perses.emplace_back(new PersonHuman(Cfg, idBot));
                    }
                    idBot++;
                }

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
                std::cout << field.bank.info();

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

                pers.doAct();
            /// std::cout << pers.info ();

                std::cout << (CellInfoTester)field[pos] << '\n';
                std::cout << &pers                      << '\n';

                std::cout << "ОПЕРАЦИИ: -------------------------------: \n";
                std::cout <<  pers.input();

                std::cout << std::endl;
            }
            return true;
        }

        ///------------------------------|
        /// Генератор события.           |
        ///------------------------------:
        void genChanse()
        {   if(unsigned randN = (rand() % 100); randN < cfg->percentDoChanse)
            {   if(nullptr == persNow->pcard)
                {   persNow->pcard = field.cards.getCard();
                }
            }
        }

        friend struct TestGame;
        friend struct ManagerEvents;
    };

}   /// namespace model


#endif // M_MODEL_H
