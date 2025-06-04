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
    struct  AnimationFieldStart
    {       AnimationFieldStart(const vsl::Config& cfg) : cfg(cfg)
            {
            }

        const vsl::Config& cfg;

        bool isActive{ true};

        inline static const float H{10'000.f};

        const float        speed  { 3.f };
        const sf::Vector2f szEnd  { 3045, 1823 };
        const sf::Vector2f szStart{ H * szEnd.x / szEnd.y, H };

        void go(sf::View&  cam)
        {   cam.zoom(1.f - speed * cfg.deltaTime.asSeconds());
            const auto& sz{cam.getSize()};
            if(sz.x <= szEnd.x)
            {   cam.setSize(szEnd);
                isActive = false;
            }
        }

        void reInit(sf::View& cam)
        {   isActive = true;
            cam.setSize(szStart);
        }
    };

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

                camMove.setSize  (animationFieldStart.szStart);
                camMove.setCenter(figField.getCenter());

                cfg.uiUpLog.fooFon = [this](){fooFon();};

                dice.init();

                camDice = camFon;
            }

        vsl::Config& cfg;

        AnimationFieldStart animationFieldStart{cfg};

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

                     if (ISKEYPRESSED(Up  )) {   a.y += S; }
                else if (ISKEYPRESSED(Down)) {   a.y -= S; }
                else if (ISKEYPRESSED(Left)) {   a.x += S; }
                else if (ISKEYPRESSED(Right)){   a.x -= S; }

                camMove.move(a);

            /// l(camMove.getSize())
            }
        }

        ///-------------------------------|
        /// Переключение фонов winGame.   |
        ///-------------------------------:
        myl::SwitcherData<const char*, 7> buttonFon
        {   "res/img/ground/01.jpg",
            "res/img/ground/02.jpg",
            "res/img/ground/03.jpg",
            "res/img/ground/04.jpg",
            "res/img/ground/05.jpg",
            "res/img/ground/06.jpg",
            "..."
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

            figField.setFigurePos2Pos(idCell);
        }

        void setCellColor(unsigned idCell, unsigned idColor)
        {   figField.setColor(     idCell,          idColor);
        }

        void reStart()
        {   for(unsigned id = 0; id < 3; ++id) setPositionChip(id, 0, false);
            isDiceHide   = true ;
            isUiCellInfo = false;

            animationFieldStart.reInit(camMove);

            figField.clear();
        }

    private:
        sf::View         camFon;
        sf::View        camMove;
        sf::View        camDice;
        sf::RectangleShape  fon;
        FigureField    figField;
        FigureChips figureChips;
        ShaderDice         dice;
        bool       isUiCellInfo;

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
            auto p = const_cast<WinGame*>(this);

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

            if(isUiCellInfo){ cfg.uiCellInfo.show(); }

            if( animationFieldStart.isActive)
            {   p->animationFieldStart.go(p->camMove);
            }
        }

        friend struct SceneGame;
    };
}

#endif // WIN_GAME_H
