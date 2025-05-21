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

        eWHATDO whatDo(model::IPerson* pers) override
        {
            model::Cell& cell = (*(pers->cfg.pfield))[pers->position];

            bool canBuy  = getTuneIQ()->canBuyBot (cell.status);
            bool canSell = getTuneIQ()->canSellBot(cell.status);

            if(!cell.isBusy() && canBuy )
            {   message << "   botIQ::Есть желание КУПИТЬ!\n";
                return E_BUY;
            }
            if(canSell)
            {   message << "   botIQ::Есть желание ПРОДАТЬ!\n";
                return E_SELL;
            }

            message << "   botIQ::Пойду лучше на диване полежу ...\n";
            return E_NONE;
        }

        const TuneIQ* getTuneIQ() const override
        {   return IBotIQ::tuneIQ;
        }

    //private:


        TEST
        {   TuneIQ        tuneIQ{ "...xxx8=8xxx...", "02", "02"};
            BotIQ  botIQ(&tuneIQ);

            model::IPerson* p{nullptr};

            l(botIQ.whatDo(p))
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
