#ifndef BOT_IQ_H
#define BOT_IQ_H
///----------------------------------------------------------------------------|
/// "implants/bot-iq.h"
///----------------------------------------------------------------------------:
#include "../../debug.h"

namespace model
{   struct IPerson;
}

namespace implants
{
    struct TuneIQ;

    using TuneIQs = std::array<implants::TuneIQ, 3>;

    ///------------------------------------------------------------------------|
    /// Умственные способности бота описываются здесь.
    ///----------------------------------------------------------------- TuneIQ:
    struct TuneIQ
    {
        std::string name;

        ///----------------------------------|
        /// Запрет на продажу                |
        /// от статуса(0,1,2) товара.        |
        ///----------------------------------:
        std::string botSell;

        ///----------------------------------|
        /// Запрет на покупку от ранга(0,1,2)|
        /// товара. Чем выше ранг, тем       |
        /// выгоднее сделка.                 |
        ///----------------------------------:
        std::string botBuy;

        bool canSellBot(unsigned priceRang) const
        {   return botSell.find('0' + priceRang) == NPOS;
        }

        bool canBuyBot(unsigned priceRang) const
        {   return botBuy.find('0' + priceRang) == NPOS;
        }
    };

    enum eWHATDO
    {    E_BUY ,
         E_SELL,
         E_NONE
    };

    ///-------------------------|
    /// Интерфейс объекта.      |--------------------------------------------!!!
    ///-------------------------:
    struct      IBotIQ
    {           IBotIQ(const TuneIQ* tuneIQ) : tuneIQ(tuneIQ){}
        virtual~IBotIQ(){}
        virtual eWHATDO whatDo(model::IPerson*) = 0;
        virtual const TuneIQ* getTuneIQ() const = 0;

        std::string name{"<IBotIQ>"};

        const TuneIQ*     tuneIQ{nullptr};

        void debug() const
        {   ASSERT(nullptr != tuneIQ)
            l(tuneIQ->name)
        }

        std::stringstream message;

    private:
    };


    ///-------------------------|
    /// Интерфейс объекта.      |--------------------------------------------!!!
    ///-------------------------:
    struct  HolderTuneIQ
    {       HolderTuneIQ(const TuneIQs& tunes)
            {   init(tunes);
            }
           ~HolderTuneIQ(){ for(auto p : m) delete p; }

        const implants::TuneIQ* getTuneIQ(unsigned id) const;
        const implants::IBotIQ* get      (unsigned id) const;

        void debug() const;

    private:
        std::vector<IBotIQ*> m;

        void init(const TuneIQs& tunes);
    };
}

#endif // BOT_IQ_H
