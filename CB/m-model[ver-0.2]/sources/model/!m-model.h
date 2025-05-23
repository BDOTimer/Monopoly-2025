﻿#ifndef M_MODEL_H
#define M_MODEL_H

///----------------------------------------------------------------------------|
/// "!m-model.h"
/// Модель игры Монополия-2025.
///----------------------------------------------------------------------------:

#include "myl.h"


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
        {   myl::setwUtf8(11, s);

            std::stringstream ss;
            ss << "   ///--------------------------|\n"
               << "   /// Событие Шанс: " << s << "| <--- "<<decodeName()<<'\n'
               << "   ///--------------------------|\n";
            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& o, const Card& card);
        friend struct Cards;
    };


    ///----------------------------------|
    /// Вывод Card в консоль.            |
    ///----------------------------------:
    inline std::ostream& operator<<(std::ostream& o, const Card& card)
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
                 ln(cards[0 ].infoWhat("-1"))
                 ln(cards[25].infoWhat("4, 6"))
                 ln(cards[25].infoWhat("TODO::?"))
                 ln(cards[25].infoWhat(std::to_string(1000) + $s.data()))
        }
    };


    ///----------------------------------|
    /// Вывод Cards в консоль.           |
    ///----------------------------------:
    inline std::ostream& operator<<(std::ostream& o, const Cards& cards)
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
        {
            #define A std::setw(6) << money << $s << std::setw(17) << "|\n"
            std::stringstream ss;
            ss  << "///-----------------------------------------|\n"
                << "///     БАНК-ЛАБЕАН: " << A
                << "///-----------------------------------------|\n";
            return ss.str();
            #undef A
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

        int getPriseSell(unsigned persStatus) const
        {   return bankSell[persStatus];
        }

        int getPriseBuy(unsigned persStatus) const
        {   return bankBuy[persStatus];
        }

        unsigned getRangSell(unsigned persStatus) const
        {      return  rangSell[persStatus];
        }

        unsigned getRangBuy(unsigned persStatus) const
        {       return  bankBuy[persStatus];
        }

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
            status       -= 1;
            ASSERT(status < 3)

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

        std::string getOwner() const;

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
    inline std::ostream& operator<<(std::ostream& o, const Cell& e)
    {   return o    << std::setw(4) << e.id
                    << std::setw(4) << e.chance
                    << std::setw(4) << e.status + 1
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

    inline std::ostream& operator<<(std::ostream& o, const std::array<int,3>& e)
    {   return o << '['
                 << std::setw(3) << e[0] << ", "
                 << std::setw(3) << e[1] << ", "
                 << std::setw(3) << e[2]
                 << ']' << $s;
    }

    inline std::ostream& operator<<(std::ostream& o, const CellInfoTester& e)
    {   return o
            << "ЯЧЕЙКА: ---------------------------------: " << e.id << '\n'
            << "   Позиция      : " << e.id          << '\n'
            << "   Товар        : " << e.name        << '\n'
            << "   Шанс         : "
            <<     (e.chance != 0 ? "Да" : "Нет")    << '\n'
            << "   Статус       : " << e.status + 1  << '\n'
            << "   Цена базовая : " << e.priseBase   << '\n'
            << "   Банк покупает: " << e.bankBuy     << '\n'
            << "   Банк продает : " << e.bankSell    << '\n'
            << "   "                << e.getOwner()  << '\n'
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
                for(auto& e : *this)
                {   e.init();
                    statist.capitalAll += e.priseBase;
                }
                statist.priceAvarege = statist.capitalAll / (int)size();
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

        struct 
        {   int capitalAll{};
            int priceAvarege;
        }statist;

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
    inline std::ostream& operator<<(std::ostream& o, const Field& field)
    {   for(const auto& cell : field)
        {          o << cell << '\n';
        }   return o;
    }


    ///------------------------------------------------------------------------|
    /// Фича: Монопольный Бонус.
    ///-------------------------------------------------------------- MonoBonus:
    struct  MonoBonus
    {       MonoBonus()
            {
            }

        void add(unsigned statusCell)
        {   ASSERT (statusCell < counted.size())
          ++counted[statusCell];
            doCalc();
        }

        void sub(unsigned statusCell)
        {   ASSERT (statusCell < counted.size())
          --counted[statusCell];
            ASSERT (counted[statusCell] >= 0)
            doCalc();
        }

        int getBonus() const { return moneyBonus; }

        std::string info(const unsigned w) const
        {   auto p = const_cast<MonoBonus*>(this);
            std::stringstream
            ss;    ss << std::setw(w) << getBonus() << $s << messEvent;
            p->messEvent.clear();
            return ss.str();
        }

        std::string getMessStatistic() const
        {   std::stringstream ss;
            ss << "   Инвентарь: Статус, Кол-во\n"
               << "                   1, " << std::setw(6) << counted[0] << '\n'
               << "                   2, " << std::setw(6) << counted[1] << '\n'
               << "                   3, " << std::setw(6) << counted[2] << '\n';
            return ss.str();
        }

        void debug() const
        {   l(messEvent)
        }

        const std::array<int, 3>& getStatistic() const
        {   return counted;
        }

    private:
        std::array<int, 3>   counted{};
        int               moneyBonus{};

        std::string        messEvent;

        int getMaxCnt() const
        {   return  std::max(counted[0],
                    std::max(counted[1],
                             counted[2]));
        }

        void doCalc()
        {   switch(int maxCnt = getMaxCnt(); maxCnt)
            {
                case  4:
                case  5: doMessEvent( 5); break;
                case  6:
                case  7: doMessEvent(10); break;
                case  8: doMessEvent(15); break;
                case  9: doMessEvent(20); break;
                case 10: doMessEvent(25); break;
                default: if(maxCnt  < 4)  doMessEvent(0);
            }
        }

        void doMessEvent(const int moneyBonusNow)
        {   if(moneyBonusNow  > moneyBonus)
            {    messEvent = " (повысился)";
            }
            else if(moneyBonusNow  < moneyBonus)
            {    messEvent = " (понизился)";
            }
            else messEvent = "";
            moneyBonus = moneyBonusNow;
        }
    };


    /// ERROR ...(не рабочий, почему?)
    ///----------------------------------|
    /// Вывод Cell в консоль.            |
    ///----------------------------------:
    inline std::function<bool(const unsigned, const unsigned)>
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
                {
                    name += cfg.getNamePlayer(id);
                    name.push_back('\"');

                    getLetters ();
                }
        virtual~IPerson()  {};

        virtual void input() = 0;
        virtual void doAct() = 0;

        const Config& cfg;

        ///------------------------------|
        /// Интеллект бота               |
        ///------------------------------:
        implants::IBotIQ* botIQ{nullptr};

        ///------------------------------|
        /// Идентификатор.               |
        ///------------------------------:
        unsigned id;

        ///------------------------------|
        /// Имя персонажа.               |
        ///------------------------------:
        std::string  name{'\"'};

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
        /// Премия, если встал на своё.  |
        ///------------------------------:
        int prizeMyCell{5};

        ///------------------------------|
        /// Карточка.                    |
        ///------------------------------:
        Card* pcard{nullptr};

        ///------------------------------|
        /// - Письма.                    |
        /// - События.                   |
        /// - Операции.                  |
        ///------------------------------:
        std::stringstream     messLetters;
        std::stringstream     messEvents ;
        std::stringstream     messOper   ;

        void clearMess    (  )
        {   messEvents.str("");
            messOper  .str("");
        }

        std::string_view decodeDone[3]
        {   "купить." ,
            "продать.",
            "ничего не делать."
        };

        void toPayBonus(IPerson* persGuest)
        {   if(const auto& m = monoBonus.getBonus(); m != 0)
            {
                            capital += monoBonus.getBonus();
                persGuest-> capital -= monoBonus.getBonus();

                messLetters << " --> Получен Бонус  : +" << std::setw(4) << m
                            << $s << " от " << persGuest->name << '\n';

                persGuest->messOper << " --> Оплата Бонуса: -" << m
                                    << $s << " для " << name   << '\n';
            }
        }

        std::string getLetters()
        {   std::string_view a{"\nПИСЬМА:\n"};
            if(messLetters.str().size() == a.size()) return "";
            messLetters << '\n';
            std::string  s{messLetters.str()};
            messLetters.str("");
            messLetters <<   a;
            return       s;
        }

        ///------------------------------|
        /// Монопольный Бонус.           |
        ///------------------------------:
        MonoBonus               monoBonus;

        ///------------------------------|
        /// Добавить Вещь в инвентарь.   |
        ///------------------------------:
        void addThing(const Cell& cell)
        {   cargo.insert(std::pair{cell.status, cell.id});
            capital += cell.priseBase;

            monoBonus.add(cell.status);
        }

        ///------------------------------|
        /// Удалить Вещь из инвентаря.   |
        ///------------------------------:
        void deleteThing(std::multimap<unsigned, unsigned>::iterator it)
        {   const   auto&[sts, id] = *it;
            monoBonus.sub(sts);

            Cell&      cellSell = (*cfg.pfield)[id];
            capital -= cellSell.priseBase;
            cargo.erase(it);
        }


        int getCapitalMoney() const
        {   return capital + money;
        }

        void nextCircle()
        { ++circle;
            isActBuy = false;
        }

        [[nodiscard]]
        const std::string infoCargo() const
        {   const Field& field = *(cfg.pfield);

            std::stringstream ss;

            int n{0};

            ss << "   Инвентарь:\n";
            for(const auto&[sts, id] : cargo)
            {   ss  << "    " << std::setw(3) << ++n <<':'
                              << std::setw(3) << id
                    << ", " << sts << ", "   << field[id].name << '\n';
            }   ss  << "    ... ";

            if(cargo.empty())
            {   ss << "пусто ..." ;
            }   ss << "\n";

            ss << "   Вся собственность: " << capital << $s << '\n';

            return ss.str();
        }

        ///------------------------------|
        /// Транзакция оплаты ренты.     |
        ///------------------------------:
        int payRent(IPerson* host)
        {
            Cell& cell = (*(cfg.pfield))[host->position];

            int price = cell.bankSell[host->status];

            host->money += price;
                  money -= price;

            host->messLetters << " --> Плата за аренду: +"
                              << std::setw(4) << price << $s
                          << " от " << name << '\n';
            return price;
        }

        ///------------------------------|
        /// Инфа об имени игроке.        |
        ///------------------------------:
        [[nodiscard]]
        const std::string infoName() const
        {   std::stringstream ss;
                              ss  << ">> ИГРОК: "
                                  << name << " --> " << botIQ->name << '\n';
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

        const char E{'\n'};

        ///------------------------------|
        /// Чья ячейка?                  |
        ///------------------------------:
        cell.pers == nullptr
            ?   ss  << "Эта ячейка свободна для продажи!\n"
            :   ss  << "Эта ячейка принадлежит игроку "
                    << cell.pers->name << E;

        /// auto n = 15 - nn + name.size();
            ss  << "   Кошелёк = " << std::setw(4) << money      << E
                << "   Позиция = " << std::setw(4) << position   << E
                << "   Статус  = " << std::setw(4)
                << cfg.decodeStatus(status)<< ": ["<< status + 1 << "]\n"
                << "   Круг    = " << std::setw(4) << circle     << E
                << "   Шанс    = " << std::setw(4) << chance     << E
                << "   Товар   = " <<                 cell.name
                << ": Cтатус: ["   <<             1 + cell.status<< "]\n"
                << "   Кол-во  = " << std::setw(4)
                << (cell.amountThings != 0 ?
                    std::to_string(cell.amountThings) : " пусто")<< E
                << "   Банк покупает: " << std::setw(4)
                                        << cell.bankBuy [status] << E
                << "   Базовая цена : " << std::setw(4)
                                        << cell.priseBase        << E
                << "   Банк продаёт : " << std::setw(4)
                                        << cell.bankSell[status] << E
                << E;

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

        void debug() const
        {   monoBonus.debug();
        }

        friend std::ostream& operator<<(std::ostream&, const IPerson*);
    };


    ///----------------------------------|
    /// Вывод IPerson в поток.           |
    ///----------------------------------:
    inline std::ostream& operator<<(std::ostream& o, const IPerson* p)
    {   const Config& cfg{p->cfg};
    /// const auto&   cell = (*(cfg.pfield))[p->position];

        o   << "Игрок: ----------------------------------: " << p->id << '\n'
            << "   Кошелёк  : " << std::setw(4) <<  p->money << $s    << '\n'
            << "   Статус   : " << std::setw(4) <<  p->status + 1 << " ---> "
                                 << cfg.decodeStatus(p->status)       << '\n'
            << "   Круг     : " << std::setw(4) <<  p->circle         << '\n'
            << "   МоноБонус: " << p->monoBonus.info(4)               << '\n'
            << p->monoBonus.getMessStatistic()
        ;
        return o;
    }


    ///------------------------------------------------------------------------|
    /// ВСЕ ПРАВИЛА для карточек ШАНС.
    ///------------------------------------------------------------ Card::doAct:
    inline std::string Card::doAct(IPerson* pers)
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
                return infoWhat(std::to_string(mn) + $s.data());
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
                botIQ = const_cast<implants::IBotIQ*>(cfg.getIBotIQ(id));

            /// cfg._holderTuneIQ.debug();
            }

        void input () override
        {
            Cell& cell = (*cfg.pfield)[position];

            ASSERT(cell.status < 3)
            ASSERT(     status < 3)

            bool goodSky = cell.status == IPerson::status;
            if(  goodSky)
            {   messEvents << "   \"ЗВЁЗДЫ СВЕТЯТ МНЕ КРАСИВО!\"\n";
            }

            ///----------------------------------------|
            /// IQ.                                    |
            ///----------------------------------------:
            const auto&[OPER, TITER] = botIQ->whatDo(this);

            ///----------------------------------------|
            /// Ячейка уже занята!                     |
            ///----------------------------------------:
            if(cell.isBusy())
            {
                if(this == cell.pers)
                {   messOper << "   ... это моя ячейка: +"
                             << prizeMyCell << $sn;
                }
                else
                {   messEvents << "   ... ячейка занята ...\n";
                    messOper   << "   Кошелёк до   : " << money << $sn
                               << "   Стоимость аренды ячейки: -"
                               << this->payRent(cell.pers)      << $sn
                               << "   Кошелёк после: " << money << $sn;
                }
            }

            Bank& bank = cfg.pfield->bank;

            switch(OPER)
            {
                ///----------------------------------------|
                /// Купить.                                |
                ///----------------------------------------:
                case implants::E_BUY:
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

                        messOper << "   \"" << cell.name
                                 << "\" куплен за " << price << $sn;

                        messOper << IPerson::infoCargo();

                        isActBuy  = true;
                        cell.pers = this;
                    }
                    else if( isEmpty) messEvents << "   ... нет товара ...\n";
                    else if(!isMoney) messEvents << "   ... мало денег ...\n";

                    break;
                }

                ///----------------------------------------|
                /// Продать.                               |
                ///----------------------------------------:
                case implants::E_SELL:
                {

                    messOper << IPerson::infoCargo();

                    if(cargo.empty())
                    {   break;
                    }

                    auto it = cargo.begin();

                    const auto&[sts, id] = *it;

                /// if(TITER == nullptr) break;

                    Cell& cellSell = (*cfg.pfield)[id];
                /// Cell& cellSell = (*cfg.pfield)[TITER->second];

                    const int price = goodSky ?
                        cellSell.getBestBuy() : cellSell.bankBuy[status];

                    {
                             money += price;
                        bank.money -= price;
                      ++cellSell.amountThings;

                        ///------------------------------|
                        /// Удалить Вещь из инвентаря.   |
                        ///------------------------------:
                        deleteThing(it);

                        messOper << "Товар \"" << cellSell.name
                                 << "\" продан, цена: " << price << $s << '\n';

                        messOper << IPerson::infoCargo();

                        cell.pers = nullptr;
                    }

                    break;
                }
                ///----------------------------------------|
                /// Ничего не делать.                      |
                ///----------------------------------------:
                default: ;
            }

            if( isActBuy )
            {   messEvents << "   В этом круге блок на продажу!\n";
            }
        }

        void doAct() override
        {
            if(pcard == nullptr) return;
            if(pcard -> empty()) { pcard = nullptr; return; }

            messEvents << pcard->doAct(this);
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

        void input () override
        {
            /// ...
        }

        void doAct() override
        {   if(nullptr == pcard) return;
            messEvents << pcard->doAct(this);
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
    {       Referee  (const Config& Cfg)
                :   field          (Cfg)
                ,   whoFirstPlayer (Cfg.amountPlayers)
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

                cfg = const_cast<model::Config*>(&Cfg);

                cfg->pfield = &field;
                cfg->managerEvents.initEvents<Referee>(this);
            //  cfg->managerEvents.push(0);
            //  cfg->managerEvents.make( );

                order = myl::WhoFirstPlayer::getFastOrder(Cfg.amountPlayers);
            }
           ~Referee()
            {   for(auto p : perses) delete p;
            }

        std::string infoField() const
        {   std::stringstream  ss;
                               ss << "Field:\n"   << field << "\n"
                                  << "Жеребьевка: ";

            for(const auto n : order)
            {   ss << (n+1) << " ";
            }   ss << '\n';

            for(unsigned i = 0; i < perses.size(); ++i)
            {   ss << perses[order[i]]->name << '\n';
            }

            ss << '\n' << info();

            return ss.str();
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

        model::Config*  cfg;

        std::vector<model::Card >  cards;
        std::vector<model::Card*> pcards;

        myl::WhoFirstPlayer whoFirstPlayer;
        std::vector<unsigned>        order;

        IPerson* persNow{nullptr};

        ///------------------------------|
        /// Проверка конца игры.         |
        ///------------------------------:
        bool isGameOver() const
        {
            /// TODO ...

            return false;
        }

        ///------------------------------|
        /// Все игроки делают по 1 ходу. |
        ///------------------------------:
        [[nodiscard]]
        std::string  doStep()
        {   std::stringstream ss;
            for(unsigned i  = 0;      i < perses.size(); ++i)
            {            ss << doStep(i);
            }   return   ss.str();
        }

        unsigned steps{};

        ///------------------------------|
        /// Все игроки делают по 1 ходу. |
        ///------------------------------:
        [[nodiscard]]
        std::string doStep(unsigned i)
        {
            ++steps;

            std::stringstream ss;

            ///------------------------------|
            /// Банк.                        |
            ///------------------------------:
            ss << field.bank.info() << '\n';

            persNow = perses[order[i]];

            auto& pers = *persNow;
            ss <<  pers.infoName();

            pers.clearMess();

            ///------------------------------|
            /// Получить письма.             |
            ///------------------------------:
            ss << pers.getLetters();

            const unsigned cubicDice = rand() % 6 + 1;

            ss <<   "           |----\n"
                    "cubicDice:-| " << cubicDice << " |\n"
                    "           ----|\n";

            const auto& [pos, isStart]
                = field.add(pers.position, cubicDice);

            pers.position = pos;

            Cell& cell = field[pos];

            ///------------------------------|
            /// Ячейка с шансом?             |
            ///------------------------------:
            if(field[pos].chance > 0) genChanse();

            if (isStart)
            {
                if (++pers.status == 3) pers.status = 0;

                pers.nextCircle();
            }

            ///------------------------------|
            /// Оплата МоноБонуса?           |
            ///------------------------------:
            if(auto owner = cell.pers; owner   != nullptr &&
                                       persNow != cell.pers)
            {
                owner->toPayBonus(persNow);
            }

            pers.doAct();

            pers.input();

            ///------------------------------|
            /// Поле + Игрок.                |
            ///------------------------------:
            ss << (CellInfoTester)field[pos] << '\n';
            ss << &pers; ///                 << '\n';

            if(!pers.messEvents.str().empty())
            {   ss << "CОБЫТИЯ: --------------------------------: \n";
                ss << pers.messEvents.str() << std::endl;
            }

            if(!pers.messOper.str().empty())
            {   ss << "ОПЕРАЦИИ: -------------------------------: \n";
                ss << pers.messOper.str() << std::endl;
            }

            ///------------------------------|
            /// В конце раунда ...           |
            ///------------------------------:
            if(0 == steps % perses.size())
            {   whoVictor();
                ss << conditionVictorStr.str();
            }

            return ss.str();
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

        std::stringstream conditionVictorStr;

        size_t whoVictor() const
        {   
            int capitalAll = field.statist.capitalAll;

            std::array<int, 3> cap;

            unsigned i{};

            for(const auto&   p : perses)
            {   capitalAll += p-> money ;
                cap[i++]    = p-> getCapitalMoney();
            }

            int capitalAll$cv = capitalAll 
                              * cfg->victoryCondition.percentCapital / 100;

            for(unsigned i = 0; i < cap.size(); ++i)
            {   
                if(cap[i] >= capitalAll$cv)
                {
                    const_cast<Referee*>(this)->conditionVictorStr
                        << "   |-------------------------------------|\n"
                        << "   |                                     |\n"
                        << "   | Победитель игры: " << perses[i]->name << '\n'
                        << "   |                                     |\n"
                        << "   |-------------------------------------|\n\n";
                    return i;
                }
            }

            return NPOS;
        }

        friend struct TestGame;
        friend struct ManagerEvents;
        friend struct ModelGate;
    };

}   /// namespace model


#endif // M_MODEL_H
