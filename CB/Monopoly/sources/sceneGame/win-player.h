﻿#ifndef WIN_PLAYER_H
#define WIN_PLAYER_H
///----------------------------------------------------------------------------|
/// "win-player.h"
///----------------------------------------------------------------------------:
#include "../common.h"

namespace vsl
{
    struct  WinPlayer : vsl::IObject
    {       WinPlayer(vsl::Config& cfg, unsigned id) : cfg(cfg)
            {
                const auto& rect    = cfg.markupSG.winPlayer[id];
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
                fon.setFillColor({  255, 0,  0, 31});
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
        }
    };
}

#endif // WIN_PLAYER_H
