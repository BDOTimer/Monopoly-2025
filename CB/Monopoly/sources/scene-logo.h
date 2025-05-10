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
    struct  SceneLogo   :  vsl::IObject
    {       SceneLogo     (vsl::Config& cfg)
				:	cfg   (cfg)
				,	fon   (cfg.szfWin)
            {
                vsl::Config::setOrigin(fon);

                fon.setTexture(&HolderTexture::get("res/logo.jpg"));

                for(unsigned i = 1; i < Data4Sprites::get().size(); ++i)
                {   m.emplace_back(Player(Data4Sprites::get()[i]));
                }

                tmess1.setString(mess1);
            }

		vsl::Config& cfg;

        PLUG_IOBJECT2

		void input(const std::optional<sf::Event>&  event) override
		{
			if (event->is<sf::Event::KeyPressed>())
            {   if (ISKEYPRESSED(Space))
                {	cfg.scenesSwitcher.next();
					cfg.musicLogo    .pause();
                }
            }

			if (auto p = event->getIf<sf::Event::MouseButtonPressed>())
            {   if ( p->button  == sf::Mouse::Button::Left)
                {   
					using E = sf::SoundSource::Status;
					cfg.musicLogo.getStatus() == E::Playing
						? cfg.musicLogo.pause()
                        : cfg.musicLogo.play ();
                }
		    }
		}

    private:
        sf::RectangleShape fon;
        std::vector<Player>  m;

        std::wstring mess1{L"ЛОГО\nЖмакай ПРОБЕЛ ..."};
        TextStyleA  tmess1;

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {                           target.draw(fon, states);
            target.setView(*cfg.camFon);
            for(const auto& pl : m) target.draw(pl , states);

            target.setView(*cfg.camGui);
            target.draw(tmess1, states);
        }
    };
}

#endif // SCENE_LOGO_H
