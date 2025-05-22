///----------------------------------------------------------------------------|
/// "bot-iq.h"
///----------------------------------------------------------------------------:
#include "bot-iq.h"
#include "../!m-model.h"

namespace implants
{

    struct  BotIQ : IBotIQ
    {       BotIQ(const TuneIQ* tuneIQ) : IBotIQ(tuneIQ)
            {
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
            /// Получить разрешение на покупку и продажу.      |
            ///------------------------------------------------:
            bool canBuy  = getTuneIQ()->canBuyBot (cell.status) || goodSky;
            bool canSell = getTuneIQ()->canSellBot(cell.status) || goodSky;

            mayBuy  = mayBuy  && canBuy;
            maySell = maySell && canSell;

            if(mayBuy)
            {   message << "   botIQ::Есть желание КУПИТЬ!\n";

                       answerIQ.E = E_BUY;
                return answerIQ;
            }
            if(maySell)
            {   message << "   botIQ::Есть желание ПРОДАТЬ!\n";
                answerIQ.titer = getIterGoods4Sell(pers);

                if(answerIQ.titer != pers->cargo.cend())
                {   message << "   IQ::Рекомендованно продать: "
                            << answerIQ.titer->second << ", "
                            << field[answerIQ.titer->second/*id*/].name << "\n";
                }
                else message << "   IQ::Рекомендаций на продажу нет!\n";

                       answerIQ.E = E_SELL;
                return answerIQ;
            }

            message << "   botIQ::Пойду лучше на диване полежу ...\n";

                   answerIQ.E = E_NONE;
            return answerIQ;
        }

        const TuneIQ* getTuneIQ() const override
        {   return IBotIQ::tuneIQ;
        }

    private:
        Titer getIterGoods4Sell(model::IPerson* pers) const
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

            for(size_t   i = 0;    i < stat.size();    ++i)
            {   if(!stat[i].empty()) a[stat[i].size()] = i;
            }

            if(!a.empty())
            {   return stat[a.begin()->second].front();
            }

            return pers->cargo.cend();
        }

        TEST
        {   TuneIQ        tuneIQ{ "...xxx8=8xxx...", "02", "02"};
            BotIQ  botIQ(&tuneIQ);

            model::IPerson* p{nullptr};

            l(botIQ.whatDo(p).E)
            l(botIQ.getTuneIQ()->name)
        }

        friend void tests();
    };


    void HolderTuneIQ::init(const TuneIQs& tunes)
    {   m.reserve(tunes.size());
        for(const auto& t : tunes)
        {   m.push_back(new BotIQ(&t));
        }
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


    void tests()
    {   BotIQ::test();

        std::cin.get();
    }
}
