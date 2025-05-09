#ifndef SCENE_GAME_H
#define SCENE_GAME_H
///----------------------------------------------------------------------------|
/// "scene-game.h"
///----------------------------------------------------------------------------:
#include "common.h"
#include "shader-dice.h"

namespace vsl
{

    ///------------------------------------------------------------------------|
    /// Отображаемый одиночный объект.
    ///-------------------------------------------------------------- SceneGame:
    struct  SceneGame   : vsl::IObject
    {       SceneGame() : nameTx("res/game.jpg")
                        , fon (vsl::cfg.szfWin)
                        , dice(vsl::cfg.szfWin)
            {
                fon.setTexture(&HolderTexture::get(nameTx));

                vsl::Config::setOrigin(fon);

                tmess1.setString(L"Игра...");

                dice.init();
            }

        PLUG_IOBJECT

        ///-----------------------------------|
        /// Имя загруженной текстуры.         |
        ///-----------------------------------:
        std::string     nameTx;
        sf::RectangleShape fon;
        TextStyleA      tmess1;

        ShaderDice        dice;

        ///-----------------------------------|
        /// Дебаг.                            |
        ///-----------------------------------:
        void debug() const
        {   l(nameTx)
            l(fon.getTexture()->getSize().x)
            l(fon.getTexture()->getSize().y)
        }

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {   target.setView(*vsl::cfg.camFon);
            target.draw   (fon,      states);

            target.setView(*vsl::cfg.camGui);
            target.draw   (tmess1,   states);

            target.setView(*vsl::cfg.camFon);
            target.draw   (dice,     states);
        }
    };
}

#endif // SCENE_GAME_H
