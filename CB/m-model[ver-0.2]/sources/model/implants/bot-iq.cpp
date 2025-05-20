///----------------------------------------------------------------------------|
/// "bot-iq.h"
///----------------------------------------------------------------------------:
#include "bot-iq.h"
#include "../!m-model.h"

namespace implants
{

    struct  BotIQ : IBotIQ
    {       BotIQ(const TuneIQ& tuneIQ) : tuneIQ(tuneIQ)
            {
            }

        unsigned whatDo(model::IPerson* pers) override
        {


            return 2;
        }

        const TuneIQ* get() const
        {   return &tuneIQ;
        }

    private:
        const TuneIQ& tuneIQ;

        TEST
        {   TuneIQ       tuneIQ ;
            BotIQ  botIQ(tuneIQ);

            model::IPerson* p{nullptr};

            l(botIQ.whatDo(p))
        }

        friend void tests();
    };


    void HolderTuneIQ::init(const TuneIQs& tunes)
    {   m.reserve(tunes.size());
        for(const auto& t : tunes)
        {   m.push_back(new BotIQ(t));
        }
    }

    const implants::TuneIQ* HolderTuneIQ::get(unsigned id) const
    {   ASSERT(id < m.size())
        return m[id]->get();
    }


    void tests()
    {   BotIQ::test();

        std::cin.get();
    }
}
