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
    {   ///----------------------------------|
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


    ///-------------------------|
    /// Интерфейс объекта.      |--------------------------------------------!!!
    ///-------------------------:
    struct      IBotIQ
    {   virtual~IBotIQ(){}
        virtual unsigned whatDo(model::IPerson*) = 0;
        virtual const TuneIQ* get() const = 0;

        std::string_view name;

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

        virtual const implants::TuneIQ* get(unsigned id) const;

    private:
        std::vector<IBotIQ*> m;

        void init(const TuneIQs& tunes);
    };
}

#endif // BOT_IQ_H
