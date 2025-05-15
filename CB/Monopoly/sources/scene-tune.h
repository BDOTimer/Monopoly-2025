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

                tmess1.setString(L"ТЮНИНГ.\nДля игры: ЖМИ \"1\"");

                const auto r = tmess1.getGlobalBounds();

                tmess1.setPosition
                ({  cfg.szfWin.x - r.size.x - 30,
                    r.position.y
                });
            }

		vsl::Config& cfg;

        PLUG_IOBJECT2

		void input(const std::optional<sf::Event>&  event) override
		{
			if (event->is<sf::Event::KeyPressed>())
            {   if (ISKEYPRESSED(Num1))
                {   cfg.scenesSwitcher.next();
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

            cfg.uiTune.show();
        }
    };
}

#endif // SCENE_TUNE_H
