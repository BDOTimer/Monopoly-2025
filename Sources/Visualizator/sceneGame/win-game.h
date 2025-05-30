#ifndef WIN_GAME_H
#define WIN_GAME_H
///----------------------------------------------------------------------------|
/// "win-game.h"
///----------------------------------------------------------------------------:
#include "../common.h"
#include "fig-field.h"
#include "fig-chips.h"

namespace vsl
{
    ///------------------------------------------------------------------------|
    /// WinGame
    ///---------------------------------------------------------------- WinGame:
    struct  WinGame : vsl::IObject
    {       WinGame(vsl::Config& cfg)
                :   cfg         (cfg)
                ,   figField    (cfg)
                ,	dice        (cfg)
            {
                const auto& rect    = cfg.markupSG.getWinBase();
                const auto& border  = cfg.markupSG.border;
                const auto& border2 = border + border;

                const float x = rect.size.x * cfg.szfWin.x;
                const float y = rect.size.y * cfg.szfWin.y;

                camFon.setViewport(rect);
                camFon.setSize  ({x, y}); camDice = camFon;
                camFon.setCenter({0, 0});

                fon.setSize  ({x-border2, y-border2});
              //fon.setPosition({border, border});
                pr::setOrigin(fon);
              //fon.setFillColor({  0, 0, 255,31});
                fon.setOutlineColor({64,64,128});
                fon.setOutlineThickness(border);

                fon.setTexture(&HolderTexture::get(buttonFon.get()));

                camMove = camFon;

            /// camMove.setCenter({1260, 1100});
                camMove.setSize  ({3750, 2410});
                camMove.setCenter(figField.getCenter());

                cfg.uiUpLog.fooFon = [this](){fooFon();};

                dice.init();

                camDice = camFon;
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

        ///-------------------------------|
        /// Переключение фонов winGame.   |
        ///-------------------------------:
        myl::SwitcherData<const char*, 4> buttonFon
        {   "res/img/ground_01.jpg",
            "res/img/ground_02.jpg",
            "res/img/ground_03.jpg",
            "res/img/ground_04.jpg"
        };

        bool isFon{false};
        bool isDiceHide  ;

        void fooFon() /// <--- вешается на кнопку "Фон" в winGame.
        {
            buttonFon.next();

            if(isFon = buttonFon.n != buttonFon.m.size() - 1, isFon)
            {   fon.setTexture(&HolderTexture::get(buttonFon.get()), true);
            }
        }

        void setPositionChip(unsigned idPlayer,
                             unsigned idCell,
                             bool     isSnd = true)
        {
            figureChips.setPosition(
                idPlayer, figField[idCell].getPosition(), isSnd
            );
        }

        void reStart()
        {   for(unsigned id = 0; id < 3; ++id) setPositionChip(id, 0, false);
            isDiceHide = true ;
        }

    private:
        sf::View         camFon;
        sf::View        camMove;
        sf::View        camDice;
        sf::RectangleShape  fon;
        FigureField    figField;
        FigureChips figureChips;
        ShaderDice         dice;

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
            objectTest4.update(cfg.dt());

            target.setView(camFon);
if(isFon) { target.draw   (fon,         states); }

            target.setView(camMove);
            target.draw   (figField,    states);
            target.draw   (objectTest4, states);

            target.draw   (figureChips, states);

            if(!isDiceHide)
            {   target.setView(camDice);
                target.draw(dice, states);
            }
        }

        friend struct SceneGame;
    };
}

#endif // WIN_GAME_H
