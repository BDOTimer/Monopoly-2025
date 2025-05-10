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
    struct  SceneGame   :  vsl::IObject
    {       SceneGame     (vsl::Config& cfg)
				:	cfg   (cfg)
				,	nameTx("res/game.jpg")
                ,	fon   (cfg.szfWin)
                ,	dice  (cfg.szfWin)
            {
                fon.setTexture(&HolderTexture::get(nameTx));

                vsl::Config::setOrigin(fon);

                tmess1.setString(L"Игра...\nВыход: ESCAPE");

                dice.init();
            }

		vsl::Config&  cfg;

        PLUG_IOBJECT2

		void input(const std::optional<sf::Event>&  event) override
		{
			if (event->is<sf::Event::KeyPressed>())
            {   if (ISKEYPRESSED(Escape))
                {   using E = vsl::ScenesSwitcher;
					cfg.scenesSwitcher.doSwitcher(E::E_TUNE);
					cfg.musicGame.stop();
                }
            }
		}

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
        {   target.setView(*cfg.camFon);
            target.draw   (fon, states);

            target.setView(*cfg.camGui   );
            target.draw   (tmess1, states);

            target.setView(*cfg.camFon );
            target.draw   (dice, states);
        }
    };
}

#endif // SCENE_GAME_H
