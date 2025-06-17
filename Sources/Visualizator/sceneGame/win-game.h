#ifndef WIN_GAME_H
#define WIN_GAME_H
///----------------------------------------------------------------------------|
/// "win-game.h"
///----------------------------------------------------------------------------:
#include "../common.h"
#include "fig-field.h"
#include "fig-chips.h"
#include "move-chip.h"

namespace vsl
{
    struct  AnimationFieldStart
    {       AnimationFieldStart(const vsl::Config& cfg) : cfg(cfg)
            {
            }

        const vsl::Config& cfg;

        bool isActive{ true};

        void go(sf::View&  cam)
        {   cam.zoom(1.f - speed * cfg.deltaTime.asSeconds());
            const auto& sz{cam.getSize()};
            if(sz.y <= szEndFig.y || sz.x <= szEndFig.x)
            {
                isActive = false;
            }
        }

        void reInit(sf::View& cam, const sf::Vector2f szFig)
        {   isActive   = true ;
            szEndFig   = szFig;
            szStartCam = {cam.getSize().x * H, cam.getSize().y * H};
            cam.setSize(szStartCam);
        }

        const sf::Vector2f& getSizeStart() const { return szStartCam; };

    private:
        inline static
        const float  H      {20.f};
        const float  speed  { 3.f };
        sf::Vector2f szStartCam;
        sf::Vector2f szEndFig  ;
    };

    ///------------------------------------------------------------------------|
    /// WinGame
    ///---------------------------------------------------------------- WinGame:
    struct  WinGame : vsl::IObject
    {       WinGame(vsl::Config& cfg)
                :   cfg         (cfg)
                ,   figField    (cfg)
                ,   dice        (cfg)
                ,   moveChip    (cfg, figField)

            {   const auto& rect    = cfg.markupSG.getWinBase();
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
                cfg.uiUpLog.fooFon = [this](){fooFon();};

                dice.init();

                camDice = camFon;

                figField.fooRePosition = [this]()
                {   //this->camMove.setSize  (animationFieldStart.getSizeStart());
                    this->camMove.setCenter(figField.getCenter());
                    this->animationFieldStart.reInit
                    (   this->camMove, figField.getSize()
                    );
                    this->updateSetPositionChip();
                };

            /// figField.fooRePosition(); /// UB!!!
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
        myl::SwitcherData<const char*, 9> buttonFon
        {   "res/img/ground/01.jpg",
            "res/img/ground/02.jpg",
            "res/img/ground/03.jpg",
            "res/img/ground/04.jpg",
            "res/img/ground/05.jpg",
            "res/img/ground/06.jpg",
            "res/img/ground/07.jpg",
            "res/img/ground/08.jpg",
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
                             bool     isSnd )
        {
/**
            const auto& ps{ figField[idCell].getPosition() };
            figureChips[idPlayer].setPosition( ps, idCell, isSnd );
**/
            figField.setFigurePos2Pos(idCell);

            /// TODO: ... MoveChip ...
            moveChip.buildRoute(&figureChips[idPlayer], idCell);
        }

        void updateSetPositionChip()
        {   for(auto& chip : figureChips)
            {   chip.setPosition(figField[chip.idCell].getPosition(), false);
            }
        }

        void setCellColor(unsigned idCell, unsigned idColor)
        {   figField.setColor(     idCell,          idColor);
        }

        void reStart()
        {   for(unsigned id =  0; id < 3; ++id) setPositionChip(id, 0, false);
            isDiceHide   = true ;
            isUiCellInfo = false;

            figField.clear();
            figField.fooRePosition();
        }

        ///-------------------------------|
        /// Крутить кубик.                |
        ///-------------------------------:
        void doDice()
        {    dice.resetDice  ();
             dice.isRot  = true;
             isDiceHide = false;
        }

        ///-------------------------------|
        /// Остановить кубик.             |
        ///-------------------------------:
        void stopDice()
        {    dice.isRot = false;
             isDiceHide = true;
        }

    private:
        sf::View         camFon;
        sf::View        camMove;
        sf::View        camDice;
        sf::RectangleShape  fon;
        FigureField    figField;
        ShaderDice         dice;
        FigureChips figureChips;
        bool       isUiCellInfo;

        MoveChip       moveChip;

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
            auto p = const_cast<WinGame*>(this);

            if(moveChip.isMove) p->moveChip.move();

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

            ///-------------------------------|
            /// Панель продажи ячеек.         |
            ///-------------------------------:
            cfg.uiSellPanel.show();
        }

        friend struct SceneGame;
    };
}

#endif // WIN_GAME_H
