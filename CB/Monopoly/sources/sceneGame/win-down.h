#ifndef WIN_DOWN_H
#define WIN_DOWN_H
///----------------------------------------------------------------------------|
/// "win-down.h"
///----------------------------------------------------------------------------:
#include "../common.h"
#include "markup.h"

namespace vsl
{
    struct  WinDown : vsl::IObject
    {       WinDown(vsl::Config& cfg) : cfg(cfg)
            {
                const auto& rect    = MarkupSceneGame::get(cfg).winDown;
                const auto& border  = MarkupSceneGame::get(cfg).border;
                const auto& border2 = border + border;

                const float x = rect.size.x * cfg.szfWin.x;
                const float y = rect.size.y * cfg.szfWin.y;

                cam.setViewport(rect);
                cam.setSize  ({x, y});
                cam.setCenter({0, 0});
                fon.setSize  ({x-border2, y-border2});
                //fon.setPosition({border, border});
                vsl::Config::setOrigin(fon);
                fon.setFillColor({  0, 0, 255,31});
                fon.setOutlineColor({64,64,128});
                fon.setOutlineThickness(border);
            }

        vsl::Config& cfg;

        PLUG_IOBJECT

    private:
        sf::View           cam;
        sf::RectangleShape fon;

        ///------------------------------------|
        /// На рендер.                         |
        ///------------------------------------:
        virtual void draw(sf::RenderTarget& target,
                          sf::RenderStates  states) const
        {
            target.setView(cam);
            target.draw   (fon, states);
            target.draw   (objectTest2, states);
        }
    };
}

#endif // WIN_DOWN_H
