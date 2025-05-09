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
            {
                rs.setTexture(&HolderTexture::get(nameTx));
                rs.setSize({1344, 768});

                tmess1.setString(L"Игра...");

                dice.init();
            }

        PLUG_IOBJECT

        ///-----------------------------------|
        /// Имя загруженной текстуры.         |
        ///-----------------------------------:
        std::string    nameTx;
        sf::RectangleShape rs;
        TextStyleA     tmess1;

        ShaderDice  dice;

        ///-----------------------------------|
        /// Дебаг.                            |
        ///-----------------------------------:
        void debug() const
        {   l(nameTx)
            l(rs.getTexture()->getSize().x)
            l(rs.getTexture()->getSize().y)
        }

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {   target.draw(rs    , states);
            target.draw(tmess1, states);
            target.draw(dice  , states);
        }
    };
}

#endif // SCENE_GAME_H
