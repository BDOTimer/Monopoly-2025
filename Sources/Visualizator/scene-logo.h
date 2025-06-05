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
                pr::setOrigin(fon);

                fon.setTexture(&HolderTexture::get("res/logo.jpg"));

                for(unsigned i = 1; i < Data4Sprites::get().size(); ++i)
                {   m.emplace_back(Player(Data4Sprites::get()[i]));
                }

                std::wstringstream ss;

                ss  << "Compilator: " << win::COMPILATOR_NAME << '\n'
                    << L"Старт "      << LOGO_VISUALIZATOR    << "\n\n"
                    << mess1;

                tmess1.setString(ss.str());
            }

		vsl::Config& cfg;

        PLUG_IOBJECT2

		void input(const std::optional<sf::Event>&  event) override
		{
			if (event->is<sf::Event::KeyPressed>())
            {   if (ISKEYPRESSED(Space))
                {	cfg.scenesSwitcher.next();
					cfg.musics       .pause();
                }
            }

			if (auto p = event->getIf<sf::Event::MouseButtonPressed>())
            {   if ( p->button  == sf::Mouse::Button::Left)
                {
					using E = sf::SoundSource::Status;
					cfg.musics.getStatus  () == E::Playing
						? cfg.musics.pause()
                        : cfg.musics.play(1);

                /// isRot = !isRot;
                }
		    }
		}

    private:
        sf::RectangleShape fon;
        std::vector<Player>  m;

        bool isRot{false};

        std::wstring mess1{L"ЛОГО.\nНастройки: ПРОБЕЛ ..."};
        primitive::TextStyleA  tmess1;

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {                           target.draw(fon, states);
            target.setView(*cfg.camFon);

            if(isRot)
            {   auto p = const_cast<Player*>(&m[2]);
                     p->sp.rotate(sf::degrees(1.f));
            }

            for(const auto& pl : m)
            {   target.draw(pl , states);
            }

            target.setView(*cfg.camGui);
            target.draw(tmess1, states);
        }
    };
}

#endif // SCENE_LOGO_H
