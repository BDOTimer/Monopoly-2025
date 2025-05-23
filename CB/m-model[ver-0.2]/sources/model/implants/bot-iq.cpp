///----------------------------------------------------------------------------|
/// "bot-iq.h"
///----------------------------------------------------------------------------:
#include "bot-iq.h"
#include "../!m-model.h"

namespace implants
{

    ///------------------------------------------------------------------------|
    /// Стратегия типа самого умного...
    ///------------------------------------------------------------- BotIQSmart:
    struct  BotIQSmart : IBotIQ
    {       BotIQSmart(const TuneIQ* tuneIQ) : IBotIQ(tuneIQ)
            {   IBotIQ::name = "IQ::Smart";
            }

        AnswerIQ whatDo(model::IPerson* pers) override
        {
            AnswerIQ answerIQ;

            model::Field& field = *(pers->cfg.pfield);
            model::Cell&  cell  = field[pers->position];

            ///------------------------------------------------|
            /// "ЗВЁЗДЫ СВЕТЯТ МНЕ КРАСИВО!"                   |
            ///------------------------------------------------:
            bool goodSky = cell.status == pers->status;

            ///------------------------------------------------|
            /// Получить инфу о возможности сделки.            |
            ///------------------------------------------------:
            bool mayBuy  = !cell.isBusy();
            bool maySell = !pers->cargo.empty() && !pers->isActBuy;

            ///------------------------------------------------|
            /// Узнаём цену покупки ячеки.                     |
            ///------------------------------------------------:
            int priseBuy  = cell.getPriseBuy (pers->status);

            ///------------------------------------------------|
            /// Есть ли в кошельке деньги на покупку?          |
            ///------------------------------------------------:
            mayBuy = mayBuy && (pers->money >= priseBuy);

            ///------------------------------------------------|
            /// Если денег много не фик продоавтать!           |
            ///------------------------------------------------:
            maySell = maySell && (pers->money < 200);

            ///------------------------------------------------|
            /// Получить разрешение на покупку и продажу.      |
            ///------------------------------------------------:
            bool canBuy  = getTuneIQ()->canBuyBot (cell.status) || goodSky;
            bool canSell = getTuneIQ()->canSellBot(cell.status) || goodSky;

            mayBuy  = mayBuy  && canBuy;
            maySell = maySell && canSell;

            auto& mE = pers->messEvents;

            if(mayBuy)
            {   mE << "   botIQ::Есть желание КУПИТЬ!\n";

                       answerIQ.E = E_BUY;
                return answerIQ;
            }
            if(maySell)
            {   mE << "   botIQ::Есть желание ПРОДАТЬ!\n";
                answerIQ.titer = getIterGoods4Sell(pers);

                if(answerIQ.titer != pers->cargo.cend())
                {   mE << "   IQ::Рекомендованно продать: "
                       << answerIQ.titer->second << ", "
                       << field[answerIQ.titer->second/*id*/].name << "\n";
                }
                else mE << "   IQ::Рекомендаций на продажу нет!\n";

                       answerIQ.E = E_SELL;
                return answerIQ;
            }

            mE << "   botIQ::Пойду лучше на диване полежу ...\n";

                   answerIQ.E = E_NONE;
            return answerIQ;
        }

        const TuneIQ* getTuneIQ() const override
        {   return IBotIQ::tuneIQ;
        }

    protected:
        static Titer getIterGoods4Sell(model::IPerson* pers)
        {
            if(pers->cargo.empty()) return pers->cargo.cend();

            std::array<std::list<Titer>, 3> stat
            {   std::list<Titer>(),
                std::list<Titer>(),
                std::list<Titer>()
            };

            for(auto e  = pers->cargo.cbegin();
                     e != pers->cargo.cend  (); ++e)
            {
                const auto&[status, id]    = *e ;
                stat       [status].push_back(e);
            }

            std::map<size_t, unsigned> a;

            for(size_t   i = 0;    i < stat.size();             ++i)
            {   if(!stat[i].empty()) a[stat[i].size()] = unsigned(i);
            }

            if(!a.empty())
            {   return stat[a.begin()->second].front();
            }

            return pers->cargo.cend();
        }

        TEST
        {   TuneIQ            tuneIQ{ "...xxx8=8xxx...", "02", "02"};
            BotIQSmart botIQ(&tuneIQ);

            model::IPerson* p{nullptr};

            l(botIQ.whatDo(p).E)
            l(botIQ.getTuneIQ()->name)
        }

        friend void tests();
    };


    ///------------------------------------------------------------------------|
    /// Стратегия охотника за монобонусом.
    ///--------------------------------------------------------- BotIQMonoBonus:
    struct  BotIQMonoBonus : BotIQSmart
    {       BotIQMonoBonus(const TuneIQ* tuneIQ) : BotIQSmart(tuneIQ)
            {
                IBotIQ::name = "IQ::MonoBonus";
            }

        ///------------------------------------------------|
        /// Запрет продажи товара с данным статусом.       |
        ///------------------------------------------------:
        int vetoStatus{-1};

        AnswerIQ whatDo(model::IPerson* pers) override
        {
            AnswerIQ answerIQ;

            if(vetoStatus == -1)
            {   vetoStatus = getVetoStatus(pers);
            }

            model::Field& field = *(pers->cfg.pfield);
            model::Cell&  cell  = field[pers->position];

            ///------------------------------------------------|
            /// "ЗВЁЗДЫ СВЕТЯТ МНЕ КРАСИВО!"                   |
            ///------------------------------------------------:
            bool goodSky = cell.status == pers->status;

            ///------------------------------------------------|
            /// Получить инфу о возможности сделки.            |
            ///------------------------------------------------:
            bool mayBuy  = !cell.isBusy();
            bool maySell = !pers->cargo.empty() && !pers->isActBuy;

            ///------------------------------------------------|
            /// Узнаём цену покупки ячеки.                     |
            ///------------------------------------------------:
            int priseBuy  = cell.getPriseBuy (pers->status);

            ///------------------------------------------------|
            /// Есть ли в кошельке деньги на покупку?          |
            ///------------------------------------------------:
            mayBuy = mayBuy && (pers->money >= priseBuy);

            ///------------------------------------------------|
            /// Если денег много не фик продоавтать!           |
            ///------------------------------------------------:
            maySell = maySell && (pers->money < 200);

            ///------------------------------------------------|
            /// Получить разрешение на покупку и продажу.      |
            ///------------------------------------------------:
            bool canBuy  = getTuneIQ()->canBuyBot (cell.status) || goodSky;
            bool canSell = getTuneIQ()->canSellBot(cell.status) || goodSky;

            mayBuy  = mayBuy  && canBuy;
            maySell = maySell && canSell;

            auto& mE = pers->messEvents;

            if(mayBuy)
            {   mE << "   botIQ::Есть желание КУПИТЬ!\n";

                       answerIQ.E = E_BUY;
                return answerIQ;
            }
            if(maySell)
            {   mE << "   botIQ::Есть желание ПРОДАТЬ!\n";
                answerIQ.titer = BotIQSmart::getIterGoods4Sell(pers);

                if(answerIQ.titer        != pers->cargo.cend() &&
                   answerIQ.titer->first != (unsigned)vetoStatus)
                {
                    mE << "   IQ::Рекомендованно продать: "
                            << answerIQ.titer->second << ", "
                            << field[answerIQ.titer->second/*id*/].name << "\n";
                }
                else mE << "   IQ::Рекомендаций на продажу нет!\n";

                       answerIQ.E = E_SELL;
                return answerIQ;
            }

            mE << "   botIQ::Пойду лучше на диване полежу ...\n";

                   answerIQ.E = E_NONE;
            return answerIQ;
        }

        const TuneIQ* getTuneIQ() const override
        {   return IBotIQ::tuneIQ;
        }

    private:

        int getVetoStatus(model::IPerson* pers) const
        {   const auto& arr = pers->monoBonus.getStatistic();
            /// TODO ...
            std::map<unsigned, unsigned> m
            {   {arr[0], 0},
                {arr[1], 1},
                {arr[2], 2}
            };
            const auto it = --m.end();
            return     it-> first   > 6 ? it->second : -1;
        }

        TEST
        {   TuneIQ                 tuneIQ{ "...xxx8=8xxx...", "02", "02"};
            BotIQMonoBonus  botIQ(&tuneIQ);

            model::IPerson* p{nullptr};

            l(botIQ.whatDo(p).E)
            l(botIQ.getTuneIQ()->name)
        }

        friend void tests();
    };


    ///------------------------------------------------------------------------|
    /// Стратегия дурака.
    ///-------------------------------------------------------------- BotIQFool:
    struct  BotIQFool : BotIQSmart
    {       BotIQFool(const TuneIQ* tuneIQ) : BotIQSmart(tuneIQ)
            {
                IBotIQ::name = "IQ::Fool";
            }

        AnswerIQ whatDo(model::IPerson* pers) override
        {
            AnswerIQ answerIQ;

            model::Field& field = *(pers->cfg.pfield);
            model::Cell&  cell  = field[pers->position];

            ///------------------------------------------------|
            /// "ЗВЁЗДЫ СВЕТЯТ МНЕ КРАСИВО!"                   |
            ///------------------------------------------------:
            bool goodSky = cell.status == pers->status;

            ///------------------------------------------------|
            /// Получить инфу о возможности сделки.            |
            ///------------------------------------------------:
            bool mayBuy  = !cell.isBusy();
            bool maySell = !pers->cargo.empty() && !pers->isActBuy;

            ///------------------------------------------------|
            /// Узнаём цену покупки ячеки.                     |
            ///------------------------------------------------:
            int priseBuy  = cell.getPriseBuy (pers->status);

            ///------------------------------------------------|
            /// Есть ли в кошельке деньги на покупку?          |
            ///------------------------------------------------:
            mayBuy = mayBuy && (pers->money >= priseBuy);

            ///------------------------------------------------|
            /// Если денег много не фик продоавтать!           |
            ///------------------------------------------------:
            maySell = maySell && (pers->money < 200);

            ///------------------------------------------------|
            /// Получить разрешение на покупку и продажу.      |
            ///------------------------------------------------:
            bool canBuy  = getTuneIQ()->canBuyBot (cell.status) || !goodSky;
            bool canSell = getTuneIQ()->canSellBot(cell.status) || !goodSky;

            mayBuy  = mayBuy  && canBuy;
            maySell = maySell && canSell;

            auto& mE = pers->messEvents;

            if(mayBuy)
            {   mE << "   botIQ::Есть желание КУПИТЬ!\n";

                       answerIQ.E = E_BUY;
                return answerIQ;
            }
            if(maySell)
            {   mE << "   botIQ::Есть желание ПРОДАТЬ!\n";
                answerIQ.titer = BotIQSmart::getIterGoods4Sell(pers);

                if(answerIQ.titer != pers->cargo.cend())
                {   mE  << "   IQ::Рекомендованно продать: "
                        << answerIQ.titer->second << ", "
                        << field[answerIQ.titer->second/*id*/].name << "\n";
                }
                else mE << "   IQ::Рекомендаций на продажу нет!\n";

                       answerIQ.E = E_SELL;
                return answerIQ;
            }

            mE << "   botIQ::Пойду лучше на диване полежу ...\n";

                   answerIQ.E = E_NONE;
            return answerIQ;
        }

        const TuneIQ* getTuneIQ() const override
        {   return IBotIQ::tuneIQ;
        }

    private:

        TEST
        {   TuneIQ                 tuneIQ{ "...xxx8=8xxx...", "02", "02"};
            BotIQMonoBonus  botIQ(&tuneIQ);

            model::IPerson* p{nullptr};

            l(botIQ.whatDo(p).E)
            l(botIQ.getTuneIQ()->name)
        }

        friend void tests();
    };


    void HolderTuneIQ::init(const TuneIQs& tunes)
    {   m.reserve(tunes.size());
        ASSERT(tunes.size() == 3)
        m.push_back(IBotIQ::fabric(eSMARTNESS_BOT::SMART   , &tunes[0]));
        m.push_back(IBotIQ::fabric(eSMARTNESS_BOT::ORDINARY, &tunes[1]));
        m.push_back(IBotIQ::fabric(eSMARTNESS_BOT::FOOL    , &tunes[2]));
    }

    const implants::TuneIQ* HolderTuneIQ::getTuneIQ(unsigned id) const
    {   ASSERT(id < m.size())
        return m[id]->getTuneIQ();
    }

    const implants::IBotIQ* HolderTuneIQ::get(unsigned id) const
    {   ASSERT(id < m.size())
        return m[id];
    }

    void HolderTuneIQ::debug() const
    {   l(m.size())
        ASSERT(!m.empty())
        m.front()->debug();
        l(m.front()->tuneIQ->name)
        l(m.front()->getTuneIQ()->name)
        SIGNAL("End HolderTuneIQ::debug() ...")
    }

    IBotIQ* IBotIQ::fabric(const eSMARTNESS_BOT type,
                           const TuneIQ*      tuneIQ)
    {   switch(type)
        {   case eSMARTNESS_BOT::SMART   : return new BotIQSmart    (tuneIQ);
            case eSMARTNESS_BOT::ORDINARY: return new BotIQMonoBonus(tuneIQ);
            case eSMARTNESS_BOT::FOOL    : return new BotIQFool     (tuneIQ);
        }   return nullptr;
    }


    void tests()
    {   BotIQSmart::test();

        std::cin.get();
    }
}
