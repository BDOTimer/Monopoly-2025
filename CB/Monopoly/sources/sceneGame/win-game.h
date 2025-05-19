#ifndef WIN_GAME_H
#define WIN_GAME_H
///----------------------------------------------------------------------------|
/// "win-game.h"
///----------------------------------------------------------------------------:
#include "../common.h"
#include "fig-field.h"

namespace vsl
{
    struct  WinGame : vsl::IObject
    {       WinGame(vsl::Config& cfg)
                :   cfg         (cfg)
                ,   figField    (cfg)
            {
                const auto& rect    = cfg.markupSG.getWinBase();
                const auto& border  = cfg.markupSG.border;
                const auto& border2 = border + border;

                const float x = rect.size.x * cfg.szfWin.x;
                const float y = rect.size.y * cfg.szfWin.y;

                cam.setViewport(rect);
                cam.setSize  ({x, y});
                cam.setCenter({0, 0});
                fon.setSize  ({x-border2, y-border2});
              //fon.setPosition({border, border});
                vsl::Config::setOrigin(fon);
              //fon.setFillColor({  0, 0, 255,31});
                fon.setOutlineColor({64,64,128});
                fon.setOutlineThickness(border);

                fon.setTexture(&HolderTexture::get("res/img/ground_01.jpg"));

                camMove = cam;

            /// camMove.setCenter({1260, 1100});
                camMove.setSize  ({3750, 2410});
                camMove.setCenter(figField.getCenter());
            }

        vsl::Config& cfg;

        PLUG_IOBJECT2

        void input(const std::optional<sf::Event>&  event) override
		{
			if (event->is<sf::Event::KeyPressed>())
            {   if (ISKEYPRESSED(W))
                {   camMove.zoom(0.97f);
                }
                if (ISKEYPRESSED(S))
                {   camMove.zoom(1.03f);
                }

                sf::Vector2f a{};
                const float  S{30.f};

                if (ISKEYPRESSED(Up))
                {   a.y += S;
                }
                else if (ISKEYPRESSED(Down))
                {   a.y -= S;
                }
                else if (ISKEYPRESSED(Left))
                {   a.x += S;
                }
                else if (ISKEYPRESSED(Right))
                {   a.x -= S;
                }

                camMove.move(a);

            /// l(camMove.getCenter())
            }
        }

    private:
        sf::View           cam;
        sf::View       camMove;
        sf::RectangleShape fon;
        FigureField   figField;

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
            objectTest4.update(cfg.dt());

            target.setView(cam);
        /// target.draw   (fon,         states);

            target.setView(camMove);
            target.draw   (figField,    states);
            target.draw   (objectTest4, states);
        }
    };
}

#endif // WIN_GAME_H
