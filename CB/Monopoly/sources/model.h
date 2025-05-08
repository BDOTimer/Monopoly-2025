#ifndef MODEL_H
#define MODEL_H
///----------------------------------------------------------------------------|
/// "model.h"
///----------------------------------------------------------------------------:
#include "player.h"

struct  xModel   : vsl::IObject
{       xModel() : table (Data4Sprites::get()[0])
                , tmess1(vsl::Config::getFont())
        {
            for(unsigned i = 1; i < Data4Sprites::get().size(); ++i)
            {   m.emplace_back(Player(Data4Sprites::get()[i]));
            }

            tmess1.setString           (mess1);
            tmess1.setCharacterSize       (24);
            tmess1.setFillColor({128, 196, 255});
        }

    PLUG_IOBJECT

private:
    Object          table;
    std::vector<Player> m;

    std::wstring mess1{L"Жмакай ENTER ..."};
    sf::Text   tmess1;

    ///------------------------------------|
    /// На рендер.                         |
    ///------------------------------------:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates  states) const
    {
                                target.draw(table, states);
        for(const auto& pl : m) target.draw(pl   , states);
        target.draw(tmess1, states);
    }
};

#endif // MODEL_H
