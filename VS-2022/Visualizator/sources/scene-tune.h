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
    {       SceneTune     (vsl::Config& cfg)
				:	cfg   (cfg)
				,	fon   (cfg.szfWin)
            {
                fon.setTexture(&HolderTexture::get("res/tune.jpg"));
                vsl::Config::setOrigin(fon);

                tmess1.setString(L"Настройки...\nЖми ENTER");
            }

		vsl::Config& cfg;

        PLUG_IOBJECT2

		void input(const std::optional<sf::Event>&  event) override
		{
			if (event->is<sf::Event::KeyPressed>())
            {   if (ISKEYPRESSED(Enter))
                {   cfg.scenesSwitcher.next();
					cfg.musicGame.play();
                }
            }
		}

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
            target.setView(*cfg.camFon);
            target.draw   (fon,      states);

            target.setView(*cfg.camGui);
            target.draw   (tmess1,   states);
        }
    };
}

#endif // SCENE_TUNE_H
