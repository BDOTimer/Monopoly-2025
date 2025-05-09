#ifndef SCENE_TUNE_H
#define SCENE_TUNE_H
///----------------------------------------------------------------------------|
/// "scene-tune.h"
///----------------------------------------------------------------------------:
#include "common.h"

namespace vsl
{
    ///------------------------------------------------------------------------|
    /// SceneTune.
    ///-------------------------------------------------------------- SceneTune:
    struct  SceneTune   : vsl::IObject
    {       SceneTune() : fon (vsl::cfg.szfWin)
            {
                fon.setTexture(&HolderTexture::get("res/tune.jpg"));
                vsl::Config::setOrigin(fon);

                tmess1.setString(L"Настройки...");
            }

        PLUG_IOBJECT

        ///-----------------------------------|
        /// ...                               |
        ///-----------------------------------:
        sf::RectangleShape fon;
        TextStyleA      tmess1;

        ///-----------------------------------|
        /// Дебаг.                            |
        ///-----------------------------------:
        void debug() const
        {   l(fon.getTexture()->getSize().x)
            l(fon.getTexture()->getSize().y)
        }

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
            target.setView(*vsl::cfg.camFon);
            target.draw   (fon,      states);

            target.setView(*vsl::cfg.camGui);
            target.draw   (tmess1,   states);
        }
    };
}

#endif // SCENE_TUNE_H
