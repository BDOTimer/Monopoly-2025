#ifndef MODEL_H
#define MODEL_H
///----------------------------------------------------------------------------|
/// "model.h"
///----------------------------------------------------------------------------:
#include "player.h"

struct  Model : sf::Drawable
{       Model() : table(Data4Sprites::get().front())
        {

            for(unsigned i = 1; i < Data4Sprites::get().size(); ++i)
            {   m.emplace_back(Player(Data4Sprites::get()[i]));
            }
        }

private:
    Object          table;
    std::vector<Player> m;

    ///------------------------------------|
    /// На рендер.                         |
    ///------------------------------------:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates  states) const
    {
                                target.draw(table, states);
        for(const auto& pl : m) target.draw(pl   , states);
    }
};

#endif // MODEL_H
