#ifndef SCENE_LOGO_H
#define SCENE_LOGO_H
///----------------------------------------------------------------------------|
/// "scene-logo.h"
///----------------------------------------------------------------------------:
#include "common.h"

///---------|
/// ImGui   |
///---------:
namespace uii
{
    ///------------------------------------------------------------------------|
    /// UILogo
    ///--------------------------------------------------------------------- UI:
    struct  UILogo   : IGui
    {       UILogo() = delete;
            UILogo(sf::RenderWindow& w) : window(w)
            {
            }

        void show()
        {

        }

    private:
        sf::RenderWindow& window;
    };
}

namespace vsl
{
    ///------------------------------------------------------------------------|
    /// SceneLogo.
    ///-------------------------------------------------------------- SceneLogo:
    struct  SceneLogo   : vsl::IObject
    {       SceneLogo() : table (Data4Sprites::get()[0])
            {
                for(unsigned i = 1; i < Data4Sprites::get().size(); ++i)
                {   m.emplace_back(Player(Data4Sprites::get()[i]));
                }

                tmess1.setString(mess1);
            }

        PLUG_IOBJECT

    private:
        Object          table;
        std::vector<Player> m;

        std::wstring mess1{L"Жмакай ENTER ..."};
        TextStyleA  tmess1;

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {                           target.draw(table, states);
            for(const auto& pl : m) target.draw(pl   , states);
            target.draw(tmess1, states);
        }
    };
}

#endif // SCENE_LOGO_H
